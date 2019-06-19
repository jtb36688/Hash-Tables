#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
  Basic hash table key/value pair
 ****/
typedef struct Pair {
  char *key;
  char *value;
} Pair;

/****
  Basic hash table
 ****/
typedef struct BasicHashTable {
  int capacity;
  Pair **storage;
} BasicHashTable;

/****
  Create a key/value pair to be stored in the hash table.
 ****/
Pair *create_pair(char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair));
  pair->key = strdup(key);
  pair->value = strdup(value);

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(Pair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/****
  djb2 hash function

  Do not modify this!
 ****/
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char * u_str = (unsigned char *)str;

  while ((c = *u_str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}


/****
  Fill this in.

  All values in storage should be initialized to NULL
  (hint: look up `calloc`)
 ****/
BasicHashTable *create_hash_table(int capacity)
{
  BasicHashTable *ht = malloc(sizeof(BasicHashTable));
  ht->storage = calloc(capacity, sizeof(Pair *));
  ht->capacity = capacity;

  return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
int index = hash(key, ht->capacity);
// Index integer which is created by hashing the provided key argument

Pair *pair = create_pair(key, value);
// Creating a key/value pair using the Pair struct and provided key/value

Pair *stored_pair = ht->storage[index];
// Creating a key/value pair using the Pair struct and an existing pointer in the hashtable 

if(stored_pair != NULL) {
  if(strcmp(key, stored_pair->key) != 0) {
    // If the provided key is NOT the same as the key on the resulting hashed index.
    printf("WARNING: overwriting value '%s' '/%s' with '%s' '/%s'\n",
    stored_pair->key, stored_pair->value, pair->key, pair->value);
  }
  destroy_pair(stored_pair);
}
ht->storage[index] = pair;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  int index = hash(key, ht->capacity);

  if(ht->storage[index] == NULL || strcmp(ht->storage[index]->key, key) != 0)
  // return; If the element at hashed index is NULL, OR if the
  // contained key is not the same as the provided key.
  {
    printf("Hash table does not contain provided key\n");
    return;
  }
  destroy_pair(ht->storage[index]);
  ht->storage[index] = NULL;
  // set hash table index to be NULL, as all others are through calloc()
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  int index = hash(key, ht->capacity);

  if (ht->storage[index] != NULL && strcmp(ht->storage[index]->key, key) == 0) {
    // return element; If the element at hashed index is not NULL, AND if the
    // contained key is the same as the provided key. 
    return ht->storage[index]->value;
  }
  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
for (int i = 0; i < ht->capacity; i++) {
  destroy_pair(ht->storage[i]);
  // Free the memory for each individual element
}
free(ht->storage);
// Free the memory for the array of elements
free(ht);
// Free the memory for the ht struct
}


#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);

  hash_table_insert(ht, "line", "Here today...\n");

  printf("%s", hash_table_retrieve(ht, "line"));

  hash_table_remove(ht, "line");

  if (hash_table_retrieve(ht, "line") == NULL) {
    printf("...gone tomorrow. (success)\n");
  } else {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0;
}
#endif
