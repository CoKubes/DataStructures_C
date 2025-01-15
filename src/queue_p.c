#include <queue_p.h>

/**
 * @brief creates a new queue
 *
 * @param capacity max number of nodes the queue will hold
 * @param customfree pointer to the free function to be used with that list
 *        list
 * @returns pointer to allocated priority queue on success, NULL on failure
 */
queue_p_t *queue_p_init(uint32_t capacity, FREE_F customfree)
{
    int error = SUCCESS;
    queue_p_t *p_new_queue = NULL;

    //validate input
    if (0 == capacity)      
    {
        error = -INVALID_ARGUMENT;
    }
    else
    {
        p_new_queue = (queue_p_t *)calloc(1, sizeof(queue_p_t));
        if (NULL == p_new_queue)
        {
            error = -MEM_ALLOCATION_ERROR;
        }
    }

    if (0 == error)
    {
        p_new_queue->arr = (queue_p_node_t **)malloc(capacity * sizeof(queue_p_node_t *));
        if (NULL == p_new_queue->arr)
        {
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
 * @return 0 on success, non-zero value on failure
 */
int queue_p_fullcheck(queue_p_t *queue)
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
int queue_p_emptycheck(queue_p_t *queue)
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
 * @param priority of data
 * @return 0 on success, non-zero value on failure
 */
int queue_p_enqueue(queue_p_t *queue, void *data, int priority)
{
    int status = SUCCESS;

    if (NULL == queue)
    {
        status = -NULL_POINTER;
    }
    else if (queue_p_fullcheck(queue))
    {
        status = -OVERFLOW;
    }
    else if (NULL == data)
    {
        status = -NULL_DATA;
    }
    else if (NULL == queue->arr)
    {
        status = -NULL_DATA;
    }
    else
    {
        queue_p_node_t *new_node = (queue_p_node_t *)malloc(sizeof(queue_p_node_t));
        if (NULL == new_node)
        {
            status = -MEM_ALLOCATION_ERROR;
        }
        else
        {
            new_node->data = data;
            new_node->priority = priority;
            queue->arr[queue->currentsz++] = new_node;
        }
    }

    //sort based on priority
    if (0 == status)
    {
        for (int idx = queue->currentsz -1; idx > 0; idx--)
        {
            if (queue->arr[idx]->priority > queue->arr[idx -1]->priority)
            {
                queue_p_node_t *temp = queue->arr[idx];
                queue->arr[idx] = queue->arr[idx - 1];
                queue->arr[idx - 1] = temp;
            }
        }
    }

    return status;
}

/**
 * @brief pops the front node out of the queue
 *
 * @param queue pointer to queue pointer to pop the node off of
 * @return pointer to popped queue node on success, NULL on failure
 */
queue_p_node_t *queue_p_dequeue(queue_p_t *queue)
{
    queue_p_node_t *dequeued_node = NULL;

    if (NULL != queue && !queue_p_emptycheck(queue))
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
 * @return pointer to popped queue node on success, NULL on failure
 */
queue_p_node_t *queue_p_peek(queue_p_t *queue)
{
    queue_p_node_t *node = NULL;

    if (NULL != queue && !queue_p_emptycheck(queue))
    {
        node = queue->arr[0];
    }

    return node;
}

/**
 * @brief clear all nodes out of a queue
 *
 * @param queue pointer to queue pointer to clear out
 * @return 0 on success, non-zero value on failure
 */
int queue_p_clear(queue_p_t *queue)
{
    int error = SUCCESS;

    if (NULL == queue)
    {
        error = -NULL_POINTER;
    }
    else if (queue_p_emptycheck(queue)) 
    {
        error = -UNDERFLOW;
    }
    else
    {
        while(!queue_p_emptycheck(queue))
        {
            queue_p_node_t *node = queue_p_dequeue(queue);
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
 * @param queue pointer to queue pointer to be destroyed
 * @return 0 on success, non-zero value on failure
 */
int queue_p_destroy(queue_p_t **queue)
{
    int error = SUCCESS;

    if (NULL == queue || NULL == *queue)
    {
        error = -NULL_POINTER;
    }  
    else
    {
        if (queue_p_emptycheck(*queue))
        {
            while(!queue_p_emptycheck(*queue))
            {
                queue_p_node_t *node = queue_p_dequeue(*queue);
                if (NULL != node)
                {
                    free(node);
                }
            }
        }
        
        free(((*queue)->arr));
        free(*queue);
        *queue = NULL;
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