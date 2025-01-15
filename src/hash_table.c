#include <hash_table.h>

/**
 * @brief initializes hash table
 *
 * @param size number indexes in the table
 *
 * @return hash_table_t pointer to allocated table
 */
hash_table_t *hash_table_init(uint32_t size, FREE_F customfree)
{
    hash_table_t *hash_table = (hash_table_t *)malloc(sizeof(hash_table_t));
    if (NULL != hash_table)
    {
        hash_table->size = size;
        if (NULL != customfree)
        {
            customfree = free;
        }
        hash_table->table = (node_t **)calloc(size, sizeof(node_t *));
        if (NULL == hash_table->table)
        {
            free(hash_table);
            hash_table = NULL;
        }
    }

    return hash_table;
}

/**
 * @brief hash function for hash table indexing
 * @param key The key to hash
 * @param table_size The size of the hash_table array
 * 
 * @return index
 */
static uint32_t hash_function(const char *key, uint32_t table_size) {
    uint32_t hash = 0;
    uint32_t prime = 31; // A small prime number
    while (NULL != *key) 
    {
        hash = (hash * prime) + *key++;
    }
    return hash % table_size;
}

/**
 * @brief adds an item to the table
 *
 * @param table pointer to table address
 * @param data data to be stored at that key value
 * @param key key for data to be stored at
 *
 * @return int exit code
 */
int hash_table_add(hash_table_t *table, void *data, char *key)
{
    int status = SUCCESS;

    if (NULL == table || NULL == data || NULL == key)
    {
        status = FAILURE;
    }
    else
    {
        node_t *new_node = (node_t *)malloc(sizeof(node_t));
        if (NULL != new_node)
        {
            uint32_t hashkey = hash_function(key, table->size);
            new_node->key = strdup(key);
            new_node->data = data;
            new_node->next = NULL;
            if (NULL == new_node->key)
            {
                free(new_node);
                status = FAILURE;
            }

            if (SUCCESS == status)
            {
                if (NULL == table->table[hashkey])
                {
                    table->table[hashkey] = new_node;
                }
                else
                {
                    node_t *current = table->table[hashkey];
                    while(NULL != current->next)
                    {
                        current = current->next;
                    }
                    current->next = new_node;
                }
            }
        }
        else
        {
            status = FAILURE;
        }
        
    }

    return status;
}

/**
 * @brief looks up an item in the table by key
 *
 * @param table pointer to table address
 * @param key key for data being searched for
 *
 * @return void * data
 */
void *hash_table_lookup(hash_table_t *table, char *key)
{
    void *node_data = NULL;

    if (NULL != table)
    {
        uint32_t index = hash_function(key, table->size);
        if (NULL != table->table[index]) 
        {
            node_t *current = table->table[index];
            while (current != NULL)
            {
                if (strcmp(key, current->key) == 0)
                {
                    node_data = current->data;
                    break;
                }
                current = current->next; 
            }
        }
    }

    return node_data;
}

/**
 * @brief removes an item from the hash table
 *
 * @param table pointer to table address
 * @param key key of data to be removed
 *
 * @return int
 */
int hash_table_remove(hash_table_t *table, char *key)
{
    int status = SUCCESS;

    if (NULL == table)
    {
        status = FAILURE;
    }
    else
    {
        uint32_t index = hash_function(key, table->size);
        if (NULL == table->table[index])
        {
            status = FAILURE;
        }
        else
        {
            node_t *previous = NULL;
            node_t *current = table->table[index];
            while(NULL != current && strcmp(key, current->key) != 0)
            {
                previous = current;
                current = current->next; 
            }
            
            //free the node
            if (NULL == previous)
            {
                table->table[index] = current->next;
                free(current->key);
                free(current);
            }
            else
            {
                previous->next = current->next;
                free(current->key);
                free(current);
            }
        }
    }

    return status;
}

/**
 * @brief clears all data from hash table
 *
 * @param table_addr pointer to address of table to be cleared out
 *
 * @return int
 */
int hash_table_clear(hash_table_t *table_addr)
{
    int status = FAILURE;

    if (NULL != table_addr)
    {
        for (uint32_t x = 0; x < table_addr->size; x++)
        {
            node_t *current = table_addr->table[x];
            while (current != NULL)
            {
                node_t *node_to_free = current;
                current = current->next;
                free(node_to_free->key);
                free(node_to_free);
            }
            table_addr->table[x] = NULL;
        }
        status = SUCCESS;
    }

    return status;
}

/**
 * @brief destroys hash table
 *
 * @param table_addr pointer to table address
 * @return int
 */
int hash_table_destroy(hash_table_t **table_addr)
{
    int status = FAILURE;

    if (NULL != table_addr && NULL != *table_addr)
    {
        hash_table_clear(*table_addr);
        free((*table_addr)->table);
        free(*table_addr);
        *table_addr = NULL;

        status = SUCCESS;
    }

    return status;
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