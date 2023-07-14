# Calculator
The Calculator is a C program that implements a calculator functionality using a state machine approach. It allows users to perform arithmetic operations by inputting expressions in the form of mathematical formulas.

# Directory Structure
The relevant files for the Calculator are:
* **calculator.c:** Source file containing the implementation of the Calculator State Machine.
* **calculator.h:** Header file defining the interface and function prototypes for the Calculator.
* **calculator_test.c:** Test file for unit testing the Calculator.
* **stack.c:** Source file containing the implementation of the Stack data structure used by the Calculator.
* **stack.h:** Header file defining the interface and function prototypes for the Stack data structure.

# Usage
To use the Calculator, follow these steps:

1. Include the calculator.h header file in your C program.
2. Link the ```calculator.c``` and ```stack.c``` source files with your program.
3. Build and compile your program.

For example, to use the Calculator:
```
#include <stdio.h>
#include "calculator.h"

int main() {
	double result = 0;
	calculator_status_t status = 0;
	char expression[100] = "7+8";   

	status = Calculator(expression, &result);

	printf("the status: %d\n", status);
	printf("the result: %f\n", result);

    return 0;
}
```

# Supported Expressions
The Calculator supports various mathematical expressions, including:
* Addition (+), subtraction (-), multiplication (*), and division (/) operations.
* Parentheses for grouping expressions.
* Exponentiation (^) operation.

# Examples
Here are some example expressions that can be evaluated using the Calculator:
1. Expression: "1 + 2"
Result: 3

2. Expression: "8 - 3 * (2 + 4)"
Result: -10

3. Expression: "(2 ^ 3) * 4"
Result: 32

4. Expression: "10 / (4 - 2)"
Result: 5

5. Expression: "5 + 2 * 3 ^ 2 - 4"
Result: 17

Feel free to modify the main() function in calculator_test.c to test your own expressions.

# Contributing
Contributions to the Calculator State Machine implementation are welcome. If you find any issues or want to improve the existing implementation, feel free to open a pull request.
