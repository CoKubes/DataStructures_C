#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define SUCCESS 0
#define FAILURE 1

/**
 * @brief A function pointer to a custom-defined delete function
 *        required to support deletion/memory deallocation of
 *        arbitrary data types. For simple data types, this function
 *        pointer can simply point to the free function. For more complex data
 *        types, this function should free all of the sub items that are
 *        allocated in the data.
 *
 */
typedef void (*FREE_F)(void *data);

/**
 * @brief structure of a node_t object
 *
 * @param key       pointer to the saved keyvalue string
 * @param data      saved data pointer
 * @param next      pointer to next node_t
 */
typedef struct node_t
{
    char *key;
    void *data;
    struct node_t *next;
} node_t;

/**
 * @brief structure of a hash_table_t object
 *
 * Table will have N slots, with each slot holding a node_t
 * Upon table insertion, hash algo will detmine which slot to store data
 * If that slot is null, insert new node_t there
 * If not null (colision), traverse to end of list and append new node_t
 *
 * @param size          number of positions supported by table
 * @param table         the table of node_t lists
 * @param customfree    pointer to the user defined free function
 */
typedef struct hash_table_t
{
    uint32_t size;
    node_t **table;
    FREE_F customfree;
} hash_table_t;

/**
 * @brief initializes hash table
 *
 * @param size number indexes in the table
 *
 * @return hash_table_t pointer to allocated table
 */
hash_table_t *hash_table_init(uint32_t size, FREE_F customfree);

/**
 * @brief adds an item to the table
 *
 * @param table pointer to table address
 * @param data data to be stored at that key value
 * @param key key for data to be stored at
 *
 * @return int exit code
 */
int hash_table_add(hash_table_t *table, void *data, char *key);

/**
 * @brief looks up an item in the table by key
 *
 * @param table pointer to table address
 * @param key key for data being searched for
 *
 * @return void * data
 */
void *hash_table_lookup(hash_table_t *table, char *key);

/**
 * @brief removes an item from the hash table
 *
 * @param table pointer to table address
 * @param key key of data to be removed
 *
 * @return int
 */
int hash_table_remove(hash_table_t *table, char *key);

/**
 * @brief clears all data from hash table
 *
 * @param table_addr pointer to address of table to be cleared out
 *
 * @return int
 */
int hash_table_clear(hash_table_t *table_addr);

/**
 * @brief destroys hash table
 *
 * @param table_addr pointer to table address
 * @return int
 */
int hash_table_destroy(hash_table_t **table_addr);

/**
 * @brief frees an item and its associated memory
 *
 * @param mem_addr pointer of the item to be free'd
 */
void custom_free(void *mem_addr);

#endif
