#include "hashtable_adt.h"

/*********************************************************** Data Definitions */

/*
 * A hash table `Entry` is a list in which each element is:
 * + The key size.
 * + A copy of the key the client used at insertion.
 * + A void pointer to the item held.
 * + A self-referential pointer.
 */
typedef struct entry 
{
    size_t keysize;
    char* key;
    Element item;
    struct entry *next;
} Entry;

/*
 * # Datatype completion
 *
 * A `HashTableADT` is:  
 *  + The hash table' nbuckets.
 *  + A pointer to a hash function with a compatible signature.
 *  + A dynamically allocated array of `Entry` structures.
 */
struct hash_table_type
{
    size_t nbucketsinitial;
    size_t nbuckets;
    size_t nelems;
    HashFunction *hash;
    Entry **entries;
};

/********************************************************** Private Functions */ 

/*
 * Returns the hash index for a given key using the hash function specified by
 * the client when creating the hash table.
 */
static inline size_t calculate_key_hash(HashTableADT *ht, 
                                     const void *key, size_t keysize)
{
    size_t index;

    index = (ht->hash(key, keysize)) % ht->nbuckets;

    return index;
}

/*
 * Primality test
 */
static inline bool is_prime(size_t n)
{
    size_t i; 

    if (n == 2 || n == 3)
    {
        return true;
    }

    if (n <= 1 || n % 2 == 0 || n % 3 == 0)
    {
        return false;
    }

    for (i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
        {
            return false;
        }
    }

    return true;
}

/*
 * Returns the closest prime greater than or equal to `n`.
 */
static inline size_t get_next_prime(size_t n)
{
    while (is_prime(n) == false)
    {
        n++;
    }
    return n;
}

/***************************************************** Public Implementations */

/*
 * Create a hash table 
 */
HashTableADT *cadthashtable_new(size_t nbuckets, HashFunction *fp)
{
    HashTableADT *new;

    if (nbuckets == 0 || fp == NULL)
    {
        errno = EINVAL;
        return NULL;
    }
    
    if ((new = malloc(sizeof(struct hash_table_type))) == NULL)
    { 
        perror("cadthashtable_new malloc failed allocating struct hash_table_type");
        errno = ENOMEM;
        return NULL;
    }
    
    if ((new->entries = calloc(nbuckets, sizeof(Entry*))) == NULL)
    {
        perror("cadthashtable_new calloc failed allocating entry array");
        free(new);
        errno = ENOMEM;
        return NULL;
    }

    nbuckets = get_next_prime(nbuckets);

    new->nbucketsinitial = nbuckets;
    new->nbuckets = nbuckets;
    new->nelems = 0;
    new->hash = fp;

    return new;
}
    
/*
 * Destroy hash table
 */
void cadthashtable_destroy(HashTableADT *ht) 
{
    free(ht->entries);
    free(ht);
    return;
}

/*
 * Insert operation
 */
Element cadthashtable_insert(HashTableADT *ht, const void *key, size_t keysize, 
                            Element e)
{
    size_t index;
    Entry* new;

    if (ht == NULL || key == NULL || keysize == 0 || e == NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    if (cadthashtable_lookup(ht, key, keysize) != NULL)
    {
        errno = EEXIST;
        return NULL;
    }

    if ((new = malloc(sizeof(Entry))) == NULL)
    { 
        perror("cadthashtable_insert malloc failed allocating struct entry");
        errno = ENOMEM;
        return NULL;
    }
    if ((new->key = malloc(keysize)) == NULL)
    {
        perror("cadthashtable_insert malloc failed allocating new->key");
        free(new);
        errno = ENOMEM;
        return NULL;
    }

    index = calculate_key_hash(ht, key, keysize);
    memcpy(new->key, key, keysize);
    new->keysize = keysize; 
    new->item = e; 

    /* Insert entry */
    new->next = ht->entries[index];
    ht->entries[index] = new;
    ht->nelems++;

    return e;
}

/*
 * Lookup and return item, no removal.
 */
Element cadthashtable_lookup(HashTableADT *ht, const void *key, size_t keysize)
{
    size_t index;
    Entry *e;

    if (ht == NULL || key == NULL || keysize == 0)
    {
        errno = EINVAL;
        return NULL;
    }

    index = calculate_key_hash(ht, key, keysize);
    e = ht->entries[index];

    if (e == NULL)
    {
        return NULL;
    }

    while (e != NULL) 
    {
        if (e->keysize == keysize && memcmp(e->key, key, keysize) == 0)
        {
            break;
        }
        e = e->next;
    }

    if (e == NULL)
    {
        return NULL;
    }
    else
    {
        return e->item;
    }
}

/*
 * Removes a (possible chained) entry from the entries array returning the
 * entry's item on success.
 */
Element cadthashtable_delete(HashTableADT *ht, void *key, size_t keysize, 
                            Element e)
{
	size_t index;
    Entry **pp;
    Element deleted_item;

    if (ht == NULL || key == NULL || keysize == 0 || e == NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    index = calculate_key_hash(ht, key, keysize);
    pp = &(ht->entries[index]);

	while (*pp)
    {
        if ((*pp)->keysize == keysize && memcmp((*pp)->key, key, keysize) == 0)
        {
            Entry *temp = *pp;
            *pp = temp->next;
            deleted_item = temp->item;

            free(temp);
            ht->nelems--;

            return deleted_item;
        }
        pp = &((*pp)->next);
    }

	return NULL;
}

