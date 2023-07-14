/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Author        Lin Weintraub                                   ~
 * Version       0.0.1                                           *
 ~ Date          2/10/22                                         ~                                
 * Description   STACK                                           *                              
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */
 
#include <stdlib.h> /* malloc */
#include <stddef.h> /* size_t */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */

#include "stack.h"

struct stack
{
    void *stack_array;
    size_t capacity;
    size_t element_size;
    size_t top;
};


stack_t *StackCreate(size_t size, size_t element_size)
{
	stack_t *stack = malloc(sizeof(stack_t));
	stack->stack_array = (unsigned char *) malloc(element_size * size);
	stack->top = 0;
	stack->capacity = size;
	stack->element_size = element_size;
    
    return stack;
}

void StackPush(stack_t *stack, void *value)
{
	char *next_top = (char *)(stack->stack_array) + ((stack->top) * (stack->element_size));
	
	assert(stack);
	assert (stack->top != stack->capacity);
	
	memcpy(next_top, value, stack->element_size);
	
	++(stack->top);
}

void StackPop(stack_t *stack)
{
	assert(stack);
	assert (0 != stack->top);
	--(stack->top);
}

void *StackPeek(const stack_t *stack)
{
	char *top_ptr = (char *)(stack->stack_array) + ((stack->top - 1) * (stack->element_size));
	assert(stack);
	
	return (void *)top_ptr;
}

int StackIsEmpty(const stack_t *stack)
{
	assert(stack);
	return (0 == stack->top);
}

size_t StackCapacity(const stack_t *stack)
{
	assert(stack);
	return (stack->capacity);
}

size_t StackSize(const stack_t *stack)
{
	assert(stack);
	return (stack->top);
}

void StackDestroy(stack_t *stack)
{
	assert(stack);
	free(stack->stack_array);
	free(stack);
}
