/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Author        Lin Weintraub                                   ~
 * Version       1.0.0                                           *
 ~ Date          20/11/22                                        ~
 * Description   Calculator                                      *
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#include <stdlib.h> /* strtod */
#include <assert.h> /* assert */
#include <math.h> /* pow */
#include <string.h> /* memcpy */

#include "stack.h"
#include "calculator.h"

#define ASCII_SIZE 128
#define STACK_SIZE 100

typedef enum
{
    WAITING_FOR_NUM,
    WAITING_FOR_OP,
    END,
    ERROR,
    NUM_OF_STATES
} state_t;

typedef enum
{
    ADDITION = 0,
    SUBTRACTION = 1,
    MULTIPLICATION = 2,
    DIVISION = 3,
	POW = 4,
    BRACKET_OPEN = -1,
    BRACKET_CLOSE = -1
} precedence_op_t;

typedef struct CalculatorFSM
{
	stack_t *numbers;
	stack_t *operators;
	char *expression;
	state_t curr_state;
    calculator_status_t status;

} calculator_t;	

typedef double (*operation_t)(double number_1, double number_2);
typedef void (*action_event_t)(calculator_t *calculator, double *result);

typedef struct 
{
    action_event_t action_func;
    state_t next_state;
} state_table_t;

typedef struct 
{
    precedence_op_t operation;
    operation_t operation_func;
} operation_table_t;

/*----------------Global----------------*/
static state_table_t state_table[NUM_OF_STATES][ASCII_SIZE];
static operation_table_t operations_table[ASCII_SIZE];
static int bracket_count = 0;

/*----------------Function Declarations----------------*/
static void LutGenerator(void);
static void LutOperationGenerator(void);
static void PushNumber(calculator_t *calculator, double *result);
static void PushOperation(calculator_t *calculator, double *result);
static void End(calculator_t *calculator, double *result);
static void Error(calculator_t *calculator, double *result);
static void Space(calculator_t *calculator, double *result);
static void CloseBracket(calculator_t *calculator, double *result);
static void OpenBracket(calculator_t *calculator, double *result);
static void FoldStacks(calculator_t *calculator);

static double Add(double number_1, double number_2);
static double Multiple(double number_1, double number_2);
static double Subtract(double number_1, double number_2);
static double Power(double number_1, double number_2);
static double Divide(double number_1, double number_2);


calculator_status_t Calculator(const char *expression, double *result)
{
	stack_t *stack_numbers = StackCreate(STACK_SIZE, sizeof(double));
	stack_t *stack_operators = StackCreate(STACK_SIZE, sizeof(char));
    calculator_t calculator = {0};
    state_t next_state = WAITING_FOR_NUM;
    bracket_count = 0;

    if (NULL == stack_numbers || NULL == stack_operators)
    {
        return MEMORY_ALLOCATION_ERROR;
    }

    assert(expression);

    LutGenerator();
    LutOperationGenerator();

    calculator.operators = stack_operators;
    calculator.numbers = stack_numbers;
    calculator.expression = (void *)expression;
    calculator.curr_state = WAITING_FOR_NUM;
    calculator.status = SUCCESS;

    while (ERROR != calculator.curr_state && END != calculator.curr_state) 
    {
        next_state = state_table[calculator.curr_state][(int)*(calculator.expression)].next_state;
        state_table[calculator.curr_state][(int)*(calculator.expression)].action_func(&calculator, result); 

        calculator.curr_state = next_state;
    }

    StackDestroy(stack_operators);   
    StackDestroy(stack_numbers);

    return (calculator.status);
}

/*----------------LUT Generators----------------*/
static void LutGenerator(void)
{
    int i = 0;
    int j = 0;

    for (i = 0; i < NUM_OF_STATES; i++)
    {
        for(j = 0; j < ASCII_SIZE; j++)
        {
            state_table[i][j].action_func = Error;
            state_table[i][j].next_state = ERROR;
        }
    }

    for (i = '0'; i <= '9'; ++i)
    {
        state_table[WAITING_FOR_NUM][i].action_func = PushNumber;
        state_table[WAITING_FOR_NUM][i].next_state = WAITING_FOR_OP;
    }

    state_table[WAITING_FOR_NUM]['+'].action_func = PushNumber;
    state_table[WAITING_FOR_NUM]['+'].next_state = WAITING_FOR_OP;

    state_table[WAITING_FOR_OP]['+'].action_func = PushOperation;
    state_table[WAITING_FOR_OP]['+'].next_state = WAITING_FOR_NUM;

    state_table[WAITING_FOR_OP]['-'].action_func = PushOperation;
    state_table[WAITING_FOR_OP]['-'].next_state = WAITING_FOR_NUM;

    state_table[WAITING_FOR_NUM]['-'].action_func = PushNumber;
    state_table[WAITING_FOR_NUM]['-'].next_state = WAITING_FOR_OP;

    state_table[WAITING_FOR_OP]['*'].action_func = PushOperation;
    state_table[WAITING_FOR_OP]['*'].next_state = WAITING_FOR_NUM;

    state_table[WAITING_FOR_OP]['/'].action_func = PushOperation;
    state_table[WAITING_FOR_OP]['/'].next_state = WAITING_FOR_NUM;

    state_table[WAITING_FOR_OP]['\0'].action_func = End;
    state_table[WAITING_FOR_OP]['\0'].next_state = END;

    state_table[WAITING_FOR_OP][' '].action_func = Space;
    state_table[WAITING_FOR_OP][' '].next_state = WAITING_FOR_OP;

    state_table[WAITING_FOR_NUM][' '].action_func = Space;
    state_table[WAITING_FOR_NUM][' '].next_state = WAITING_FOR_NUM;

    state_table[WAITING_FOR_OP]['^'].action_func = PushOperation;
    state_table[WAITING_FOR_OP]['^'].next_state = WAITING_FOR_NUM;
    
    state_table[WAITING_FOR_OP][')'].action_func = CloseBracket;
    state_table[WAITING_FOR_OP][')'].next_state = WAITING_FOR_OP;
 
    state_table[WAITING_FOR_NUM]['('].action_func = OpenBracket;
    state_table[WAITING_FOR_NUM]['('].next_state = WAITING_FOR_NUM;
}

static void LutOperationGenerator(void)
{
    operations_table['+'].operation = ADDITION;
    operations_table['+'].operation_func = Add;   

    operations_table['-'].operation = SUBTRACTION;
    operations_table['-'].operation_func = Subtract; 

    operations_table['/'].operation = DIVISION;
    operations_table['/'].operation_func = Divide;

    operations_table['*'].operation = MULTIPLICATION;
    operations_table['*'].operation_func = Multiple;

    operations_table['^'].operation = POW;
    operations_table['^'].operation_func = Power;

    operations_table['('].operation = BRACKET_OPEN;
    operations_table[')'].operation = BRACKET_CLOSE;
}


/*----------------Static Functions----------------*/
static void PushNumber(calculator_t *calculator, double *result)
{
    char *end_ptr = NULL;
    double number = 0;

    assert(calculator);
    
    number = strtod(calculator->expression, &end_ptr);
    if(calculator->expression == end_ptr)
    {
        calculator->status = SYNTAX_ERROR;
        calculator->curr_state = ERROR;
        Error(calculator, result);
        return;
    }
    calculator->expression = end_ptr;
    
    StackPush(calculator->numbers, &number);  
}

static void PushOperation(calculator_t *calculator, double *result)
{
    char *push_operator = NULL;
    int push_operator_priority = 0;
    char stack_operator = 0;
    int stack_operator_priority = 0;

    (void)result;
    assert(calculator);

    push_operator = calculator->expression;
    push_operator_priority = operations_table[(int)*push_operator].operation;

    if (StackIsEmpty(calculator->operators))
    {
        StackPush(calculator->operators, push_operator);
        ++(calculator->expression);
    }

    else
    {
        stack_operator = *(char *)StackPeek(calculator->operators);
        stack_operator_priority = operations_table[(int)stack_operator].operation;

        if (stack_operator_priority > push_operator_priority)
        {
           FoldStacks(calculator);
        }
        StackPush(calculator->operators, push_operator);
        ++(calculator->expression);
    }
}

static void End(calculator_t *calculator, double *result)
{
    assert(calculator);

    if (0 != bracket_count)
    {
        *result = 0;
        calculator->status = SYNTAX_ERROR;
    }

    FoldStacks(calculator);
    
    if (SUCCESS == calculator->status)
    { 
        memcpy(result, (double *)StackPeek(calculator->numbers), sizeof(double));
    }

    if (!StackIsEmpty(calculator->operators))
    {  
        StackPop(calculator->numbers);
    }
}

static void Error(calculator_t *calculator, double *result)
{
    *result = 0;

    assert(calculator);

    if (MATH_ERROR != calculator->status)
    {
        calculator->status = SYNTAX_ERROR;
    }
}

static void Space(calculator_t *calculator, double *result)
{
    assert(calculator);

    ++(calculator->expression);
    (void)result;
}

static void OpenBracket(calculator_t *calculator, double *result)
{
    char push_operator = 0;

    (void)result;

    assert(calculator);

    push_operator = '(';

    StackPush(calculator->operators, &push_operator);

    ++bracket_count;

    ++calculator->expression;
}

static void CloseBracket(calculator_t *calculator, double *result)
{
    char stack_operator = 0;

    (void)result;

    assert(calculator);

    --bracket_count;

    stack_operator = *(char *)StackPeek(calculator->operators);
    
    if ('(' == stack_operator)
    {
        calculator->status = SYNTAX_ERROR;
        calculator->curr_state = ERROR;
        return;
    }
    
    FoldStacks(calculator);

    if (!StackIsEmpty(calculator->operators))
    {  
        StackPop(calculator->operators);
    }

    ++calculator->expression;
}

static void FoldStacks(calculator_t *calculator)
{
    double number_1 = 0;
    double number_2 = 0;
    char operator = 0;
    double result = 0;

    assert(calculator);
    operator = *(char *)StackPeek(calculator->operators);

    while (!StackIsEmpty(calculator->operators) && StackSize(calculator->numbers) >= 2 && operator != '(')
    {
        number_1 = *(double *)StackPeek(calculator->numbers);
        StackPop(calculator->numbers);

        number_2 = *(double *)StackPeek(calculator->numbers);
        StackPop(calculator->numbers);

        StackPop(calculator->operators);

        if ('/' == operator && '/' == *(char *)StackPeek(calculator->operators))
        {
            operator = '*';
        }

        if (operator == '/' && number_1 == 0)
        {
            result = 0;
            calculator->status = MATH_ERROR;
            calculator->curr_state = ERROR;

            return;
        }

        if (operator == '^' && number_1 < 0 && number_2 == 0)
        {
            result = 0;
            calculator->status = MATH_ERROR;
            calculator->curr_state = ERROR;

            return;
        }
    
        result = operations_table[(int)operator].operation_func(number_1, number_2);
        StackPush(calculator->numbers, &result);
        operator = *(char *)StackPeek(calculator->operators);
    }
}

/*----------------Operation Functions----------------*/
static double Add(double number_1, double number_2)
{
    return (number_2 + number_1);
}

static double Subtract(double number_1, double number_2)
{
    return (number_2 - number_1);
}

static double Multiple(double number_1, double number_2)
{
    return (number_2 * number_1);
}

static double Divide(double number_1, double number_2)
{
    return (number_2 / number_1);
}

static double Power(double number_1, double number_2)
{
    return (pow(number_2, number_1));
}
