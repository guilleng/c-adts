#include "minunit.h"
#include "../src/hashtable_adt.c"

/* A structure to manipulate its members directly */ 
static HashTableADT *mock_hash_table;   

/*
 * Hashes a null-terminated string. 
 * Returns (size_t) key[0] + strlen(string)
 */
static HashFunction dummy_hash;

void test_setup(void)
{   
    if ((mock_hash_table = malloc(sizeof(struct hash_table_type))) == NULL)
    {
        fprintf(stderr, "test_hash malloc"); 
        exit(EXIT_FAILURE);
    }

    return;
}

void test_teardown(void)
{
    free(mock_hash_table);
    return;
}

MU_TEST(test_dummy_hash)
{
    mu_assert_int_eq(('H' + 5), (int) dummy_hash("Hello", sizeof("Hello")));
}

MU_TEST(test_calculate_key_hash)
{
    mock_hash_table->nbuckets = 31;
    mock_hash_table->hash = dummy_hash;
    
    mu_assert_int_eq((('H' + 5) % 31), 
                    (int) calculate_key_hash(mock_hash_table, "Hello", sizeof("Hello")));
    mu_assert_int_eq((('H' + 4) % 31), 
                    (int) calculate_key_hash(mock_hash_table, "Hope", sizeof("Hope")));
    /*key2 and key3 hash to the same value */
    mu_assert_int_eq((int) calculate_key_hash(mock_hash_table, "Hope", sizeof("Hope")),
                     (int) calculate_key_hash(mock_hash_table, "Hogs", sizeof("Hogs")));
}

MU_TEST(test_get_next_prime)
{
    mu_assert_int_eq((int) get_next_prime(1), 2);
    mu_assert_int_eq((int) get_next_prime(2), 2);
    mu_assert_int_eq((int) get_next_prime(10000), 10007);
    mu_assert_int_eq((int) get_next_prime(524287), 524287);
    mu_assert_int_eq((int) get_next_prime(2147483630), 2147483647);
}

/*
 * Test object creation and corner cases.
 */
MU_TEST(test_cadthashtable_new)
{
    errno = 0;
    mu_check(cadthashtable_new(0, NULL) == NULL);
    mu_check(errno == EINVAL);

    errno = 0;
    mu_check(cadthashtable_new(1, NULL) == NULL);
    mu_check(errno == EINVAL);

    errno = 0;
    mu_check(cadthashtable_new(0, dummy_hash) == NULL);
    mu_check(errno == EINVAL);
    errno = 0;

    free(mock_hash_table);
    mu_check((mock_hash_table = cadthashtable_new(1, dummy_hash)) != NULL);
    mu_check(mock_hash_table->nbucketsinitial == 2);
    mu_check(mock_hash_table->nbuckets == 2);
    mu_check(mock_hash_table->nelems == 0);
    mu_check(mock_hash_table->hash == dummy_hash);
    free(mock_hash_table->entries);

    free(mock_hash_table);
    mu_check((mock_hash_table = cadthashtable_new(31, dummy_hash)) != NULL);
    mu_check(mock_hash_table->nbucketsinitial == 31);
    mu_check(mock_hash_table->nbuckets == 31);
    mu_check(mock_hash_table->nelems == 0);
    mu_check(mock_hash_table->hash == dummy_hash);
    free(mock_hash_table->entries);

    free(mock_hash_table);
    mu_check((mock_hash_table = cadthashtable_new(9999, dummy_hash)) != NULL);
    mu_check(mock_hash_table->nbucketsinitial == 10007);
    mu_check(mock_hash_table->nbuckets == 10007);
    mu_check(mock_hash_table->nelems == 0);
    mu_check(mock_hash_table->hash == dummy_hash);
    free(mock_hash_table->entries);
}

/*
 * Test insertion, "Hope" and "Hogs" collide thus are chained.  
 */
MU_TEST(test_cadthashtable_insert)
{
    size_t nbuckets = 31;  /* Arbitrarily chosen */

    size_t index1 = dummy_hash("Hello", sizeof("Hello")) % nbuckets;
    /* The following hashes are equal */
    size_t index2 = dummy_hash("Hope", sizeof("Hope")) % nbuckets;
    size_t index3 = dummy_hash("Hogs", sizeof("Hogs")) % nbuckets;

    if ((mock_hash_table->entries = calloc(nbuckets, sizeof(Entry*))) == NULL)
    {
        perror("test_cadthashtable_insert calloc failed allocating entry array");
        exit(EXIT_FAILURE);
    }
    mock_hash_table->nbucketsinitial = nbuckets;
    mock_hash_table->nbuckets = nbuckets;
    mock_hash_table->nelems = 0;
    mock_hash_table->hash = dummy_hash;

    /* Sanity checks */
    errno = 0;
    mu_check(cadthashtable_insert(NULL, "key", 1, "obj") == NULL);
    mu_check(errno == EINVAL);
    errno = 0;
    mu_check(cadthashtable_insert(mock_hash_table, NULL, 1, "obj") == NULL);
    mu_check(errno == EINVAL);
    errno = 0;
    mu_check(cadthashtable_insert(mock_hash_table, "key", 0, "obj") == NULL);
    mu_check(errno == EINVAL);
    errno = 0;
    mu_check(cadthashtable_insert(mock_hash_table, "key", 1, NULL) == NULL);
    mu_check(errno == EINVAL);
    errno = 0;

    /* Check insertions go smoothly */
    mu_check((cadthashtable_insert(mock_hash_table, "Hello", sizeof("Hello"), "Hello")) != NULL);
    mu_check((cadthashtable_insert(mock_hash_table, "Hope", sizeof("Hope"), "Hope")) != NULL);
    mu_check((cadthashtable_insert(mock_hash_table, "Hogs", sizeof("Hogs"), "Hogs")) != NULL);

    /* Validate table structure */
    mu_assert_string_eq(mock_hash_table->entries[index1]->item, "Hello");
    mu_assert_string_eq(mock_hash_table->entries[index2]->item, "Hogs");
    mu_assert_string_eq(mock_hash_table->entries[index2]->next->item, "Hope");
    mu_assert(mock_hash_table->nelems == 3, "nelems should be 3");

    /* Check re-insertion of element */
    errno = 0;
    mu_check((cadthashtable_insert(mock_hash_table, "Hello", sizeof("Hello"), "Hello")) == NULL);
    mu_check(errno == EEXIST);

    /* Cleanup */
    free(mock_hash_table->entries[index2]->next->key);
    free(mock_hash_table->entries[index2]->next);
    free(mock_hash_table->entries[index2]->key);
    free(mock_hash_table->entries[index2]);
    free(mock_hash_table->entries[index1]->key);
    free(mock_hash_table->entries[index1]);
    free(mock_hash_table->entries);
}

/*
 * Testing lookup function.
 */
MU_TEST(test_cadthashtable_lookup)
{
    size_t nbuckets = 31;  /* Arbitrarily chosen */
    size_t index1 = dummy_hash("Hello", sizeof("Hello")) % nbuckets;
    /* The following hashes are equal */
    size_t index2 = dummy_hash("Hope", sizeof("Hope")) % nbuckets;
    size_t index3 = dummy_hash("Hogs", sizeof("Hogs")) % nbuckets;
    Element e;

    if ((mock_hash_table->entries = calloc(nbuckets, sizeof(struct entry*))) == NULL)
    {
        perror("test_cadthashtable_lookup calloc failed allocating entry array");
        exit(EXIT_FAILURE);
    }
    mock_hash_table->nbucketsinitial = nbuckets;
    mock_hash_table->nbuckets = nbuckets;
    mock_hash_table->nelems = 0;
    mock_hash_table->hash = dummy_hash;

    /* Allocate entries */
    Entry entry1 = { sizeof("Hello"), "Hello", "Hello", NULL };
    Entry entry2 = { sizeof("Hope"), "Hope", "Hope", NULL };
    Entry entry3 = { sizeof("Hogs"), "Hogs", "Hogs", NULL };
    /* Build array of entries */
    mock_hash_table->entries[index1] = &entry1;
    mock_hash_table->entries[index2] = &entry2;
    entry2.next = &entry3;

    /* Sanity checks */
    errno = 0;
    mu_check(cadthashtable_lookup(NULL, "key", 1) == NULL);
    mu_check(errno == EINVAL);
    errno = 0;
    mu_check(cadthashtable_lookup(mock_hash_table, NULL, 1) == NULL);
    mu_check(errno == EINVAL);
    errno = 0;
    mu_check(cadthashtable_lookup(mock_hash_table, "key", 0) == NULL);
    mu_check(errno == EINVAL);

    /*
     * Pictorial representation of the table used for testing purposes.
     *
     * | Hello |
     * | Hope  |-->| Hogs |
     */

    /* Test lookups */
    mu_check((e = cadthashtable_lookup(mock_hash_table, "Hello", sizeof("Hello"))) != NULL);
    mu_check(e == entry1.item);
    mu_check((e = cadthashtable_lookup(mock_hash_table, "Hope", sizeof("Hope"))) != NULL);
    mu_check(e == entry2.item);
    mu_check((e = cadthashtable_lookup(mock_hash_table, "Hogs", sizeof("Hogs"))) != NULL);
    mu_check(e == entry3.item);
    mu_check(cadthashtable_lookup(mock_hash_table, "NotInTable", sizeof("NotInTable")) == NULL);

    free(mock_hash_table->entries);
}

/*
 * Testing deletion function.
 */
MU_TEST(test_cadthashtable_delete)
{
    size_t nbuckets = 31;  /* Arbitrarily chosen */
    size_t index1 = dummy_hash("Hello", sizeof("Hello")) % nbuckets;
    size_t index2 = dummy_hash("Hope", sizeof("Hope")) % nbuckets;
    size_t index3 = dummy_hash("Hogs", sizeof("Hogs")) % nbuckets;
    size_t index4 = dummy_hash("Holy", sizeof("Holy")) % nbuckets;
    Element e;

    mu_check(index2 == index3); /* "Hope" and "Hogs" should collide */
    mu_check(index2 == index4); /* "Hope" and "Holy" should collide */

    if ((mock_hash_table->entries = calloc(nbuckets, sizeof(struct entry*))) == NULL)
    {
        perror("test_cadthashtable_delete calloc failed allocating entry array");
        exit(EXIT_FAILURE);
    }
    mock_hash_table->nbucketsinitial = nbuckets;
    mock_hash_table->nbuckets = nbuckets;
    mock_hash_table->nelems = 0;
    mock_hash_table->hash = dummy_hash;

    /* Allocate and initialize entries */
    Entry *entry1, *entry2, *entry3, *entry4;
    if ((entry1 = malloc(sizeof(Entry))) == NULL
            || (entry2 = malloc(sizeof(Entry))) == NULL
            || (entry3 = malloc(sizeof(Entry))) == NULL
            || (entry4 = malloc(sizeof(Entry))) == NULL)
    { 
        perror("test_cadthashtable_lookup malloc failed allocating struct entry");
        exit(EXIT_FAILURE);
    }
    entry1->keysize = sizeof("Hello");
    entry1->key = "Hello";
    entry1->item = "Hello";
    entry1->next = NULL;
    entry2->keysize = sizeof("Hope");
    entry2->key = "Hope";
    entry2->item = "Hope";
    entry2->next = NULL;
    entry3->keysize = sizeof("Hogs");
    entry3->key = "Hogs";
    entry3->item = "Hogs";
    entry3->next = NULL;
    entry4->keysize = sizeof("Holy");
    entry4->key = "Holy";
    entry4->item = "Holy";
    entry4->next = NULL;

    mock_hash_table->nelems = 4;

    /* Build array of entries */
    mock_hash_table->entries[index1] = entry1;
    mock_hash_table->entries[index2] = entry2;
    mock_hash_table->entries[index2]->next = entry3;
    mock_hash_table->entries[index2]->next->next = entry4;

    /* Sanity checks */
    errno = 0;
    mu_check(cadthashtable_delete(NULL, "key", 1, "elm") == NULL);
    mu_check(errno == EINVAL);
    errno = 0;
    mu_check(cadthashtable_delete(mock_hash_table, NULL, 1, "elm") == NULL);
    mu_check(errno == EINVAL);
    errno = 0;
    mu_check(cadthashtable_delete(mock_hash_table, "key", 0, "elm") == NULL);
    mu_check(errno == EINVAL);
    errno = 0;
    mu_check(cadthashtable_delete(mock_hash_table, "key", 1, NULL) == NULL);
    mu_check(errno == EINVAL);

    /*
     * Pictorial representation of the table used for testing purposes.
     * | Hello |
     * | Hogs  | -> | Hope | -> | Holy |
     */

    /* Delete from __not__ the head of a bucket */
    e = cadthashtable_delete(mock_hash_table, "Hope", sizeof("Hope"), "Hope");
    mu_check(e != NULL);
    mu_assert_string_eq(e, "Hope");
    /* Test structure */
    mu_assert_string_eq(mock_hash_table->entries[index1]->item, "Hello");
    mu_assert_string_eq(mock_hash_table->entries[index2]->item, "Hogs");
    mu_assert(mock_hash_table->entries[index2]->next == entry4, "Node deletion error");
    mu_assert(mock_hash_table->nelems == 3, "nelems should be 3");

    /* Delete the __tail__ of a bucket */
    e = cadthashtable_delete(mock_hash_table, "Holy", sizeof("Holy"), "Holy");
    mu_check(e != NULL);
    mu_assert_string_eq(e, "Holy");
    /* Test structure */
    mu_assert_string_eq(mock_hash_table->entries[index1]->item, "Hello");
    mu_assert_string_eq(mock_hash_table->entries[index2]->item, "Hogs");
    mu_assert(mock_hash_table->entries[index2]->next == NULL, "Node deletion error");
    mu_assert(mock_hash_table->nelems == 2, "nelems should be 2");

    /* Delete the __head__ of a bucket */
    e = cadthashtable_delete(mock_hash_table, "Hello", sizeof("Hello"), "Hello");
    mu_check(e != NULL);
    mu_assert_string_eq(e, "Hello");
    /* Test structure */
    mu_assert(mock_hash_table->entries[index1] == NULL, "Node deletion error");
    mu_assert_string_eq(mock_hash_table->entries[index2]->item, "Hogs");
    mu_assert(mock_hash_table->nelems == 1, "nelems should be 1");

    free(mock_hash_table->entries);
    free(entry3);
}

MU_TEST_SUITE(test_suite) 
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);
    MU_RUN_TEST(test_dummy_hash);
	MU_RUN_TEST(test_calculate_key_hash);
    MU_RUN_TEST(test_get_next_prime);
	MU_RUN_TEST(test_cadthashtable_new);
	MU_RUN_TEST(test_cadthashtable_insert);
	MU_RUN_TEST(test_cadthashtable_lookup);
	MU_RUN_TEST(test_cadthashtable_delete);
}

int main(int argc, char *argv[]) 
{
	MU_RUN_SUITE(test_suite);
	MU_REPORT();

	return MU_EXIT_CODE;
}

static size_t dummy_hash(const void* key, size_t key_len)
{
    const char *p = key;
    return (size_t) p[0] + key_len - 1;
}
