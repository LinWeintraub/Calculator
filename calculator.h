#ifndef __CALCULATOR__
#define __CALCULATOR__

typedef enum
{
    SUCCESS,
    MATH_ERROR,
    MEMORY_ALLOCATION_ERROR,
    SYNTAX_ERROR
} calculator_status_t;

calculator_status_t Calculator(const char *expression, double *result);

#endif /* __CALCULATOR__ */
