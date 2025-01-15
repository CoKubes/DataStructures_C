#include <linked_list.h>

/**
 * @brief Default Compare Function. Should be used if NULL specified for cmp_fun
 * in createion
 *
 * @param value_to_find
 * @param node
 * @return void*
 */
void *default_compare(void *value_to_find, void *node)
{
    void *retval = NULL;
    if (*(int *)((list_node_t *)node)->data == *(int *)value_to_find)
    {
        retval = (list_node_t *)node;
    }
    
    return retval;
}

/**
 * @brief creates a new list
 *
 * @param customfree pointer to the free function to be used with that list
 * @param compare_function pointer to the compare function to be used with that
 *        list. must
 * @returns pointer to allocated list on success or NULL on failure
 */
list_t *list_new(FREE_F customfree, CMP_F compare_function)
{
    list_t *new_list = (list_t*)malloc(sizeof(list_t));
    if (NULL != new_list)
    {
        new_list->size = 0;
        new_list->head = NULL;
        new_list->tail = NULL;
        new_list->customfree = customfree ? customfree : free;
        new_list->compare_function = compare_function ? compare_function : default_compare;
    }
    return new_list;
}

/**
 * @brief pushes a new node onto the head of list
 *
 * @param list list to push the node into
 * @param data data to be pushed into node
 * @returns 0 on success, non-zero value on failure
 */
int list_push_head(list_t *list, void *data)
{
    int error = SUCCESS;

    if (NULL == list)
    {
        error = -NULL_POINTER;
    }
    else
    {
        list_node_t *new_node = (list_node_t *)malloc(sizeof(list_node_t));
        if (NULL == new_node)
        {
            error = -MEM_ALLOCATION_ERROR;
        }
        else
        {
            // Populate node parameters
            new_node->position = 0;
            new_node->data = data;
            new_node->next = list->head;

            // Update list parameters
            list->head = new_node;
            list->size++;

            // If the list was previously empty, the new node is both head and tail
            if (NULL == list->tail)
            {
                list->tail = new_node;
            }

            // Update position for all nodes
            list_node_t *current = new_node->next;
            uint32_t pos = 1;
            while (NULL != current)
            {
                current->position = pos++;
                current = current->next;
            }
        }
    }

    return error;
}


/**
 * @brief pushes a new node onto the tail of list
 *
 * @param list list to push the node into
 * @param data data to be pushed into node
 * @return 0 on success, non-zero value on failure
 */
int list_push_tail(list_t *list, void *data)
{
    int error = SUCCESS;

    if (NULL == list)
    {
        error = -NULL_POINTER;
    }
    else
    {
        list_node_t * new_node = (list_node_t*)malloc(sizeof(list_node_t));
        if (NULL == new_node)
        {
            error = -MEM_ALLOCATION_ERROR;
        }
        else
        {
            //Populate node parameters
            new_node->position = list->size;
            new_node->data = data;
            //update next
            new_node->next = NULL;

            //update list parameters
            list->size++;
            if (NULL != list->tail)
            {
                list->tail->next = new_node;
            }
            list->tail = new_node;

            if (NULL == list->head)
            {
                list->head = new_node;
            }
        }
    }

    return error;
}

/**
 * @brief checks if the list object is empty
 *
 * @param list pointer to linked list object to be checked
 * @returns non-zero if list is empty, 0 value if not empty
 */
int list_emptycheck(list_t *list)
{
    int retval = SUCCESS;
    if (NULL == list)
    {
        retval = -NULL_POINTER;
    }
    return retval;
}

/**
 * @brief pops the head node out of the list
 *
 * @param list list to pop the node out of
 * @return pointer to popped node on success, NULL on failure
 */
list_node_t *list_pop_head(list_t *list)
{
    list_node_t *popped_node = NULL;
    //Check if the list is empty
    if (NULL != list && NULL != list->head) 
    {
        popped_node = list->head;
        list->head = list->head->next;

        //Update position for all nodes
        list_node_t *current = list->head;
        uint32_t pos = 0;
        while (NULL != current)
        {
            current->position = pos;
            current = current->next;
            pos++;
        }

        //update list parameters
        list->size--;
        
        if (0 == list->size)
        {
            list->head = NULL;
            list->tail = NULL;
        }
    }
    
    return popped_node;
}

/**
 * @brief pops the tail node out of the list
 *
 * @param list list to pop the node out of
 * @return pointer to popped node on success, NULL on failure
 */
list_node_t *list_pop_tail(list_t *list)
{
    int error = SUCCESS;
    list_node_t *popped_node = NULL;

    if (NULL == list)
    {
        error = -NULL_POINTER;
    }
    else if (NULL == list->head)
    {
        error = -EMPTY;
    }
    else
    {
        popped_node = list->tail;

        if (1 == list->size)
        {
            list->head = NULL;
            list->tail = NULL;
        }
        else
        {
            // Find the second to last node
            list_node_t *current = list->head;
            while (NULL != current->next && current->next != list->tail)
            {
                current = current->next;
            }
            list->tail = current;
            list->tail->next = NULL;
        }

        list->size--;
    }

    if (error != 0)
    {
        popped_node = NULL;
    }
    else if (popped_node != NULL)
    {
        popped_node->next = NULL;
    }

    return popped_node;
}


/**
 * @brief get the data from the node at the head of the list without
 *        popping
 *
 * @param list list to pop the node out of
 * @return pointer to head node on success, NULL on failure
 */
list_node_t *list_peek_head(list_t *list)
{
    list_node_t *peeked_node = NULL;
    if (NULL != list && NULL != list->head)
    {
        peeked_node = list->head;
    }
    
    return peeked_node;
}

/**
 * @brief get the data from the node at the tail of the list without
 *        popping
 *
 * @param list list to pop the node out of
 * @return pointer to tail node on success, NULL on failure
 */
list_node_t *list_peek_tail(list_t *list)
{
    list_node_t *peeked_node = NULL;
    if (NULL != list && NULL != list->tail)
    {
        peeked_node = list->tail;
    }

    return peeked_node;
}

/**
 * @brief remove a specific node from the list based on the data stored
 *        in that node
 *
 * @param list list to remove the node from
 * @param item_to_remove the data object to be searched for
 * @return 0 on success, non-zero value on failure
 */
int list_remove(list_t *list, void **item_to_remove)
{
    int error = SUCCESS;

    if (NULL == list)
    {
        error = -NULL_POINTER; // NULL list pointer
    }
    else if (NULL == list->head)
    {
        error = -EMPTY; //Empty List
    }
    else if (NULL == item_to_remove || NULL == *item_to_remove)
    {
        error = -NULL_DATA; //NULL data
    }
    else
    {
        list_node_t *current = list->head;
        list_node_t *previous = NULL;
        
        //find occurance
        int found = 0;
        while (NULL != current)
        {

            if (0 == (*(int *)item_to_remove - *(int *)current->data))
            {
                found = 1;
                break;
            }
            else
            {
                previous = current;
                current = current->next;
            }
        }

        if (found) //edges item is head, item is tail, item is head and tail, else
        {
            if (current == list->head && current == list->tail) //Item to remove is the head and tail
            {
                list->head = NULL;
                list->tail = NULL;
            }
            else if (current == list->head) //Item to remove is head
            {
                list->head = current->next;
            }
            else if (current == list->tail) //Item to remove is tail
            {
                list->tail = previous;
            }
            else
            {
                previous->next = current->next;
            }

            // Update the positions of the remaining nodes
            list_node_t *node = list->head;
            int pos = 0;
            while (node != NULL)
            {
                node->position = pos;
                node = node->next;
                pos++;
            }

            list->size--;
            free(current);
        }
        else
        {
            error = -ITEM_NOT_FOUND; //item not found
        }
    }

    return error;
}

/**
 * @brief perform a user defined action on the data contained in all of the
 *        nodes in list
 *
 * @param list list to perform actions on
 * @param action_function pointer to user defined action function
 * @return 0 on success, non-zero value on failure
 */
int list_foreach_call(list_t *list, ACT_F action_function)
{
    int error = SUCCESS;

    if (NULL == list)
    {
        error = -NULL_POINTER; //Invalid list pointer
    }
    else if (NULL == action_function)
    {
        error = -NULL_DATA; //NULL action_function
    }
    else if (NULL == list->head)
    {
        error = -EMPTY; //Empty list
    }
    else
    {
        list_node_t *current = list->head;
        while (NULL != current)
        {
            action_function(current);
            current = current->next;
        }
    }

    return error;
}

/**
 * @brief find the first occurrance of a node containing the search_data as
 *        found by the user defined compare function
 *
 * @param list list to search through
 * @param search_data is the pointer to the address of the data to be searched
 *                    for
 * @return pointer to node found on success, NULL on failure
 */
list_node_t *list_find_first_occurrence(list_t *list, void **search_data)
{
    list_node_t *retval = NULL;
    if (NULL != list && NULL != list->head && NULL != search_data && NULL != *search_data)
    {
        list_node_t *current = list->head;
        int found = 0;
        while (NULL != current)
        {
            if (0 == (*(int *)current->data - *(int *)search_data))
            {
                found = 1;
                break;
            }
            current = current->next;
        }

        if (found)
        {
            retval = current;
        }
    }

    return retval;
}

/**
 * @brief find the first occurrance of a node containing the search_data as
 *        found by the user defined compare function
 *
 * @param list list to search through
 * @param receiving_list pointer to receiving list
 * @param search_data is the pointer to the address of the data to be searched
 *                    for
 * @return pointer to list of all found occurrecnces on success, NULL on failure
 */
list_t *list_find_all_occurrences(list_t *list, void **search_data)
{
    list_t *new_list = NULL;
    if ((NULL != list) && (NULL != search_data) && (NULL != *search_data) && (list->size != 0))
    {
        int error = 0;
        new_list = list_new((FREE_F)custom_free, (CMP_F)default_compare);

        list_node_t *current = list->head;
        while (current != NULL && 0 == error)
        {
            if (0 == (*(int *)current->data - *(int *)search_data))
            {
                // Push the data of the found node into the new list
                error = list_push_head(new_list, current->data);
            }
            current = current->next;
        }

        // Cleanup if there was an error
        if (error != 0)
        {
            new_list = NULL;
        }
    }

    return new_list;
}

/**
 * @brief sort list as per user defined compare function
 *
 * @param list pointer to list to be sorted
 * @return 0 on success, non-zero value on failure
 */
int list_sort(list_t *list)
{
    int error = SUCCESS;
    if (NULL == list)
    {
        error = -NULL_POINTER;
    }
    else if (NULL == list->head)
    {
        error = -EMPTY; 
    }
    else
    {
        int swapped;
        list_node_t *current = NULL;
        list_node_t *last_sorted = NULL;

        do
        {
            swapped = 0;
            current = list->head;

            while (current->next != last_sorted)
            {
                if (current->data > current->next->data)
                {
                    void* tmp_data = current->data;
                    current->data = current->next->data;
                    current->next->data = tmp_data;
                    swapped = 1;
                }
                current = current->next;
            }
            last_sorted = current;

        } while (swapped);
    }

    return error;
}

/**
 * @brief clear all nodes out of a list
 *
 * @param list list to clear out
 * @return 0 on success, non-zero value on failure
 */
int list_clear(list_t *list)
{
    int error = SUCCESS;
    if (NULL == list)
    {
        error = -NULL_POINTER;
    }
    else if (NULL == list->head)
    {
        error = -EMPTY;
    }
    else
    {
        list_node_t *current = list->head;
        list_node_t *next_node = NULL;
        while (NULL != current)
        {
            next_node = current->next;
            free(current);
            current = next_node;
        }

        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
    }

    return error;
}

/**
 * @brief delete a list
 *
 * @param list_address pointer to list pointer
 * @return 0 on success, non-zero value on failure
 */
int list_delete(list_t **list_address)
{
    int error = SUCCESS;

    if (NULL == list_address || NULL == *list_address)
    {
        error = -NULL_POINTER; //NULL List pointer
    }
    else
    {
        list_node_t *current = (*list_address)->head;
        list_node_t *next_node = NULL;
        while (NULL != current)
        {
            next_node = current->next;
            free(current);
            current = next_node;
        }

        free(*list_address);
        *list_address = NULL;
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

