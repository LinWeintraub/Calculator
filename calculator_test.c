/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Author        Lin Weintraub                                   ~
 * Version       1.0.0                                           *
 ~ Date          20/11/22                                        ~
 * Description   Calculator                                      *
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#include <stdio.h> /* printf */

#include "calculator.h"

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define CYAN "\033[1;36m"
#define WHITE "\033[0m"

#define TEST(name, actual, expected) \
    printf("%s: %s\n" , name, actual == expected ? GREEN"PASS"WHITE : RED"FAIL"WHITE)
#define TESTNOT(name, actual, expected) \
    printf("%s: %s\n" , name, actual != expected ? GREEN"PASS"WHITE : RED"FAIL"WHITE)
    
int main(void)
{
	char expression[100] = "1 + 2/2/2^3";
	double result = 0;
	calculator_status_t status = 0;
	char str[100] = "7+8";   
	char str2[100] = "8+83+-2^5"; 
	char str3[100] = "8+83-2^";        
    char str4[100] = "2/0";         
	char str5[100] = "8++8((3-2)5)";  
    char str6[100] = "3-2)5";   
	char str7[100] = "(3-2)5+ 5(4+4+4";
	char str8[100] = "1+	2";
	char str9[100] = "1++ 2"; 
	char str10[100] = "2^2^3";
	char str11[100] = "3-2)+5";   
	char str12[100] = "(3+5))";
	char str13[100] = "15-3/0";
	char str14[100] = "2^3^2*2";
	char str15[100] = "6/3/2/4";
	char str16[100] = "2*((3+5)";
	char str17[100] = "1-2+3"; 
	char str18[100] = "1++2   "; 
	char str19[100] = "1-3+4";
	char str20[100] = "(1-(3*9)+3+4)";
	char str21[100] = "0^-0.5";


	printf("---------------------\n");
	status = Calculator(expression, &result);
	printf("the status: %d\n", status);
	printf("the result: %f\n", result);
	
	printf("\n---------------------\n");
	status = Calculator(str, &result);
    TEST("the result: 7+8", result, 15);
	TEST("the status", status, SUCCESS);

	printf("\n---------------------\n"); 
	status = Calculator(str2, &result);
    TEST("the result: 8+83+-2^5", result, 59);
	TEST("the status", status, SUCCESS);

	printf("\n---------------------\n"); 
	status = Calculator(str3, &result);
    TEST("the result: 8+83-2^", (int)result, 0);
	TEST("the status", status, SYNTAX_ERROR);

	printf("\n---------------------\n"); 
	status = Calculator(str4, &result);
    TEST("the result 2/0", (int)result, 0);
	TEST("the status", status, MATH_ERROR);	

	printf("\n---------------------\n");        
	status = Calculator(str5, &result);
    TEST("the result 8++8((3-2)5)", result, 0);
	TEST("the status", status, SYNTAX_ERROR);

	printf("\n---------------------\n"); 
	status = Calculator(str6, &result);
    TEST("the result 3-2)5", result, 0);
	TEST("the status", status, SYNTAX_ERROR);

	printf("\n---------------------\n"); 
	status = Calculator(str7, &result);
    TEST("the result (3-2)5+ 5(4+4+4", result, 0);
	TEST("the status", status, SYNTAX_ERROR);

	printf("\n---------------------\n"); 
	status = Calculator(str8, &result);
    TEST("the result 1+		2", result, 0);
	TEST("the status", status, SYNTAX_ERROR);

	printf("\n---------------------\n"); 
	status = Calculator(str9, &result);
    TEST("the result 1++ 2", result, 0);
	TEST("the status", status, SYNTAX_ERROR);

	printf("\n---------------------\n"); 
	status = Calculator(str10, &result);
    TEST("the result 2^2^3", result, 256);
	TEST("the status", status, SUCCESS);

	printf("\n---------------------\n"); 
	status = Calculator(str11, &result);
    TEST("the result 3-2)+5", result, 0);
	TEST("the status", status, SYNTAX_ERROR);

	printf("\n---------------------\n"); 
	status = Calculator(str12, &result);
    TEST("the result (3+5))", result, 0);
	TEST("the status", status, SYNTAX_ERROR);

	printf("\n---------------------\n"); 
	status = Calculator(str13, &result);
    TEST("the result 15-3/0", result, 0);
	TEST("the status", status, MATH_ERROR);

	printf("\n---------------------\n"); 
	status = Calculator(str14, &result);
    TEST("the result 2^3^2*2", result, 1024);
	TEST("the status", status, SUCCESS);

	printf("\n---------------------\n"); 
	status = Calculator(str15, &result);
    TEST("the result 6/3/2/4", result, 0.25);
	TEST("the status", status, SUCCESS);

	printf("\n---------------------\n"); 
	status = Calculator(str16, &result);
    TEST("the result 2*((3+5)", result, 0);
	TEST("the status", status, SYNTAX_ERROR);

	printf("\n---------------------\n"); 
	status = Calculator(str17, &result);
    TEST("the result 1-2+3", result, 2);
	TEST("the status", status, SUCCESS);

	printf("\n---------------------\n"); 
	status = Calculator(str18, &result);
    TEST("the result 1++2   ", result, 3);
	TEST("the status", status, SUCCESS);

	printf("\n---------------------\n"); 
	status = Calculator(str19, &result);
    TEST("the result 1-3+4", result, 2);
	TEST("the status", status, SUCCESS);

	printf("\n---------------------\n"); 
	status = Calculator(str20, &result);
    TEST("the result (1-(3*9)+3+4)", result, -19);
	TEST("the status", status, SUCCESS);

	printf("\n---------------------\n"); 
	status = Calculator(str21, &result);
    TEST("the result 0^-0.5", result, 0);
	TEST("the status", status, MATH_ERROR);

	return 0;
}
