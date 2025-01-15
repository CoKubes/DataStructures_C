#include <stack.h>

/**
 * @brief creates a new stack
 *
 * @param capacity max number of nodes the stack will hold
 * @param customfree pointer to the free function to be used with that list
 * @returns pointer to allocated stack on SUCCESS, NULL on failure
 */
stack_t *stack_init(uint32_t capacity, FREE_F customfree)
{
    int error = SUCCESS;
    stack_t * p_new_stack = NULL;

    //validate input
    if (0 == capacity || NULL == customfree)
    {
        error = -INVALID_ARGUMENT;
    }
    else
    {
        //Allocate memory for the stack structure
        p_new_stack = (stack_t *)calloc(1, sizeof(stack_t));
        if (NULL == p_new_stack)
        {
            error = -MEM_ALLOCATION_ERROR;
        }
    }

    if (SUCCESS == error)
    {
        //allocate memory for the array
        p_new_stack->arr = (stack_node_t **)calloc(capacity, sizeof(stack_node_t *));
        if (NULL == p_new_stack->arr)
        {
            error = -MEM_ALLOCATION_ERROR;
            free(p_new_stack);
            p_new_stack = NULL;
        }
        else
        {
            p_new_stack->capacity = capacity;
            p_new_stack->currentsz = 0;
            p_new_stack->customfree = customfree;
        }
    }

    return p_new_stack;
}

/**
 * @brief verifies that stack isn't full
 *
 * @param stack pointer stack object
 * @return int exit code of the function
 */
int stack_fullcheck(stack_t *stack)
{
    int status = SUCCESS;
    if (NULL == stack)
    {
        status = -NULL_POINTER;
    }
    else
    {
        status = (stack->currentsz >= stack->capacity);
    }
    return status;
}

/**
 * @brief verifies that stack isn't empty
 *
 * @param stack pointer stack object
 * @return int exit code of the function
 */
int stack_emptycheck(stack_t *stack)
{
    int status = SUCCESS;
    if (NULL == stack)
    {
        status = -NULL_POINTER;
    }
    else
    {
        status = (0 == stack->currentsz);
    }
    return status;
}

/**
 * @brief pushes a new node into the stack
 *
 * @param stack pointer to stack pointer to push the node into
 * @param data data to be pushed into node
 * @return 0 on success, non-zero value on failure
 */
int stack_push(stack_t *stack, void *data)
{
    int error = SUCCESS;

    if (NULL == stack)
    {
        error = -NULL_POINTER;
    }
    else if (NULL == data)
    {
        error =-NULL_DATA;
    }
    else if (stack_fullcheck(stack))
    {
        error = -OVERFLOW;
    }
    else
    {
        stack_node_t *new_node = (stack_node_t *)malloc(sizeof(stack_node_t));
        if (NULL != new_node)
        {
            new_node->data = data;
            stack->arr[stack->currentsz] = new_node;  
            stack->currentsz++;
        }
        else
        {
            error = -MEM_ALLOCATION_ERROR;
        }
    }

    return error;
}

/**
 * @brief pops the front node out of the stack
 *
 * @param stack pointer to stack pointer to pop the node off of
 * @return pointer to popped node on SUCCESS, NULL on failure
 */
stack_node_t *stack_pop(stack_t *stack)
{
    stack_node_t *popped_node = NULL;
    if (NULL != stack && !stack_emptycheck(stack))
    {
        popped_node = stack->arr[stack->currentsz - 1];
        stack->currentsz--;
    }
    
    return popped_node;
}

/**
 * @brief get the data from the node at the front of the stack without popping
 *
 * @param stack pointer to stack pointer to peek
 * @return pointer to peeked node on SUCCESS, NULL on failure
 */
stack_node_t *stack_peek(stack_t *stack)
{
    stack_node_t *peeked_node = NULL;
    if (NULL != stack && !stack_emptycheck(stack))
    {
        peeked_node = stack->arr[stack->currentsz - 1];
    }
    
    return peeked_node;
}

/**
 * @brief clear all nodes out of a stack
 *
 * @param stack pointer to stack pointer to clear out
 * @return 0 on success, non-zero value on failure
 */
int stack_clear(stack_t *stack)
{ 
    int error = SUCCESS;

    if (NULL == stack)
    {
        error = -NULL_POINTER;
    }
    else if (stack_emptycheck(stack)) 
    {
        error = -UNDERFLOW;
    }
    else
    {
        //free the memory
        while (!stack_emptycheck(stack))
        {
            //pop node data
            stack_node_t *node = stack_pop(stack);
            if (NULL != node)
            {
                //free node itself
                free(node);
            }
        } 
    }
    
    return error;
}

/**
 * @brief delete a stack
 *
 * @param stack pointer to stack pointer to be destroyed
 * @return 0 on success, non-zero value on failure
 */
int stack_destroy(stack_t **stack)
{
    int error = SUCCESS;
    if (NULL == stack || NULL == *stack)
    {
        error = -NULL_POINTER;
    }
    else
    {
        //free the memory
        while (!stack_emptycheck(*stack))
        {
            //pop node data
            stack_node_t *node = stack_pop(*stack);
            if (NULL != node)
            {
                //free node itself
                free(node);
            }
        }

        free((*stack)->arr);
        free(*stack);
        *stack = NULL;
    }
    
    return error;
}

/**
 * @brief frees an item and its associated memory
 *
 * @param mem_addr pointer of the item to be free'd
 */
void custom_free(void *mem_addr)
{
    free(mem_addr);
}