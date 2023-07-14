#ifndef __STACK__
#define __STACK__

#include <stddef.h>

typedef struct stack stack_t;         

stack_t *StackCreate(size_t size, size_t element_size);

void StackPush(stack_t *stack, void *value);

void StackPop(stack_t *stack);

void *StackPeek(const stack_t *stack);

int StackIsEmpty(const stack_t *stack);

size_t StackCapacity(const stack_t *stack);

size_t StackSize(const stack_t *stack);

void StackDestroy(stack_t *stack);


#endif /* __STACK__ */
