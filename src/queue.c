#include <queue.h>

/**
 * @brief creates a new queue
 *
 * @param capacity max number of nodes the queue will hold
 * @param customfree pointer to user defined free function
 * @note if the user passes in NULL, the queue should default to using free()
 * @returns the 0 on success, non-zero value on failure
 */
queue_t *queue_init(uint32_t capacity, FREE_F customfree)
{
    int error = SUCCESS;
    queue_t *p_new_queue = NULL;

    //validate input
    if (0 == capacity)
    {
        error = -INVALID_ARGUMENT;
    }
    else
    {
        //Allocate memory for queue structure
        p_new_queue = (queue_t*)calloc(1, sizeof(queue_t));
        if (NULL == p_new_queue)
        {
            error = -MEM_ALLOCATION_ERROR;
        }
    }

    if (0 == error)
    {
        //Allocate memory for the array
        p_new_queue->arr = (queue_node_t **)calloc(capacity, sizeof(queue_node_t *));
        if (NULL == p_new_queue->arr)
        {
            error = -MEM_ALLOCATION_ERROR;
            free(p_new_queue);
            p_new_queue = NULL;
        }
        else
        {
            p_new_queue->capacity = capacity;
            p_new_queue->currentsz = 0;
            p_new_queue->customfree = customfree ? customfree : free;
        }
    }

    return p_new_queue;
}

/**
 * @brief verifies that queue isn't full
 *
 * @param queue pointer queue object
 * @return 1 if full
 */
int queue_fullcheck(queue_t *queue)
{
    int status = SUCCESS;
    if (NULL == queue)
    {
        status = -NULL_POINTER;
    }
    else
    {
        status = (queue->currentsz >= queue->capacity);
    }

    return status;
}

/**
 * @brief verifies that queue isn't empty
 *
 * @param queue pointer queue object
 * @return 0 on success, non-zero value on failure
 */
int queue_emptycheck(queue_t *queue)
{
    int status = SUCCESS;
    if (NULL == queue)
    {
        status = -NULL_POINTER;
    }
    else
    {
        status = (0 == queue->currentsz);
    }
    return status;
}

/**
 * @brief pushes a new node into the queue
 *
 * @param queue pointer to queue pointer to push the node into
 * @param data data to be pushed into node
 * @return the 0 on success, non-zero value on failure
 */
int queue_enqueue(queue_t *queue, void *data)
{
    int error = SUCCESS;

    if (NULL == queue)
    {
        error = -NULL_POINTER;
    }
    else if (queue_fullcheck(queue))
    {
        error = -OVERFLOW;
    }
    else if (NULL == data)
    {
        error = -NULL_DATA;
    }
    else if (NULL == queue->arr)
    {
        error = -NULL_DATA;
    }
    else
    {
        queue_node_t *new_node = (queue_node_t *)malloc(sizeof(queue_node_t));
        if (NULL == new_node)
        {
            error = -MEM_ALLOCATION_ERROR;
        }
        else
        {
            new_node->data = data;
            queue->arr[queue->currentsz] = new_node;
            queue->currentsz++;
        }
    }
    
    return error;
}

/**
 * @brief pops the front node out of the queue
 *
 * @param queue pointer to queue pointer to pop the node off of
 * @return the 0 on success, non-zero value on failure
 */
queue_node_t *queue_dequeue(queue_t *queue)
{
    queue_node_t *dequeued_node = NULL;

    if (NULL != queue && !queue_emptycheck(queue))
    {
        dequeued_node = queue->arr[0];
        for (uint32_t idx = 1; idx < queue->currentsz; idx++)
        {
            queue->arr[idx - 1] = queue->arr[idx];
        }
        queue->arr[queue->currentsz - 1] = NULL;
        queue->currentsz--;
    }

    return dequeued_node;
}

/**
 * @brief get the data from the node at the front of the queue without popping
 *
 * @param queue pointer to queue pointer to peek
 * @return the pointer to the head on success or NULL for failure
 */
queue_node_t *queue_peek(queue_t *queue)
{
    queue_node_t *peeked_node = NULL;

    if (!queue_emptycheck(queue))
    {
        peeked_node = queue->arr[0];
    }
 
    return peeked_node;
}

/**
 * @brief clear all nodes out of a queue
 *
 * @param queue pointer to queue pointer to clear out
 * @return the 0 on success, non-zero value on failure
 */
int queue_clear(queue_t *queue)
{
    int error = SUCCESS;

    if (NULL == queue)
    {
        error = -NULL_POINTER;
    }
    else if (queue_emptycheck(queue)) 
    {
        error = -UNDERFLOW;
    }
    else
    {
        while(!queue_emptycheck(queue))
        {
            queue_node_t *node = queue_dequeue(queue);
            if (NULL != node)
            {
                free(node);
            }
        }
    }

    return error;
}

/**
 * @brief delete a queue
 *
 * @param queue_addr pointer to address of queue to be destroyed
 * @return the 0 on success, non-zero value on failure
 */
int queue_destroy(queue_t **queue_addr)
{
    int error = SUCCESS;

    if (NULL == queue_addr || NULL == *queue_addr)
    {
        error = -NULL_POINTER;
    }  
    else
    {
        if (queue_emptycheck(*queue_addr))
        {
            while(!queue_emptycheck(*queue_addr))
            {
                queue_node_t *node = queue_dequeue(*queue_addr);
                if (NULL != node)
                {
                    free(node);
                }
            }
        }
        
        free(((*queue_addr)->arr));
        free(*queue_addr);
        *queue_addr = NULL;
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
