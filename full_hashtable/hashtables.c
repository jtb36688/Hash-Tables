#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;

/*  
Singly Linked List
stores a pointer to the head and the tail
the size of the list
*/

// typedef struct LinkedList
// {
//     LinkedPair *head;
//     LinkedPair *tail;
//     int size;
// } LinkedList;

// // constructor function for linked list

// LinkedList *create_list(void)
// {
//     LinkedList *ll = malloc(sizeof(LinkedList));
//     ll->head = NULL;
//     ll->tail = NULL;
//     ll->size = 0;

//     return ll;
// }

// // destructor function for linked list

// void free_list(LinkedList *ll)
// {
//     free(ll);
// }

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/*
  djb2 hash function

  Do not modify this!
 */
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

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable));
  ht->storage = calloc(capacity, sizeof(LinkedPair *));
  ht->capacity = capacity;

  return ht;
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 */
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  int index = hash(key, ht->capacity);
// Index integer which is created by hashing the provided key argument

// LinkedPair *pair = create_pair(key, value);
// // Creating a key/value pair using the LinkedPair struct and provided key/value


// if (ht->storage[index] == NULL) {
//   // if there is no LinkedList at hash table at index, create one and
//   // make its head and tail equal to the provided pair, then return.
//   LinkedList *new_ll = create_list();
//   new_ll->head = pair;
//   new_ll->tail = pair;
//   new_ll->size = 1;
//   ht->storage[index] = new_ll;
//   printf("New stored list created at index %d", index);
//   return;
// }

// LinkedList *stored_list = ht->storage[index];
// // creating variable representing the LinkedList stored at index, which must
// // be present because ht->storage[index] is not null following last conditional

// LinkedPair *current_node = stored_list->head;
// creating variable representing the head node of concerned LinkedList.

// LinkedPair *prev = NULL;
// // creating variable representing previous node

  if (ht->storage[index] != NULL) {
    // If there is a linked list at hashed index (not NULL), create
    // a variable representing the first node there.
    LinkedPair *current_node = ht->storage[index];
    bool found = false;
    while (!found) {
      if (strcmp(key, current_node->key) == 0) {
        // IF the provided key is the same as the current_node's key, overwrite
        // current_node's value with provided value.
        printf("Overwriting item with %s, stored in LL at index %d", key, index);
        current_node->value = value;
        found = true;
      }
      else {
        // If the key is not the same as current_node's key, first check
        // if the next node is NULL. If so, create a pair and add as the next
        // node. Else, set the next node as the current node and repeat.
        if (current_node->next == NULL) {
          LinkedPair *pair = create_pair(key, value);
          current_node->next = pair;
          found = true;
        }
        current_node = current_node->next;
      }
    }
  }
  else {
    // If the bucket at hash index is empty, create a node
    LinkedPair *pair = create_pair(key, value);
    ht->storage[index] = pair;
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key) {
  int index = hash(key, ht->capacity);

  if (ht->storage[index]) {
    LinkedPair *current_node = ht->storage[index];
    // make variable representing the first node at hashed index
    bool found = false;
    while (!found) {
      if (current_node == ht->storage[index] && strcmp(key, current_node->key) == 0) {
        // If found on first node, assign index to the next node and destroy first node. End loop.
          ht->storage[index] = current_node->next;
          destroy_pair(current_node);
          found = true;
        }
        else if (current_node->next && strcmp(key, current_node->next->key) == 0) {
        // If the next node is the matching key, assign current node's next to two nodes past,
        // then destroy the next node. End loop.
          LinkedPair *found_node = current_node->next;
          current_node->next = current_node->next->next;
          destroy_pair(found_node);
          found = true;
        }
        else {
          // If there is no next node, return warning.
          if (!current_node->next) {
            printf("hash_table_remove: no key found matching %s", key);
            break;
          }
          // If there is a next node, change current node to that and repeat loop.
          current_node = current_node->next;
        }
      }
    }
    else {
      // If there is not a node at hashed index, return warning.
      printf("hash_table_remove: no key found matching %s", key);
    }
}


/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
  int index = hash(key, ht->capacity);

  if (ht->storage[index] != NULL) {
    LinkedPair *current_node = ht->storage[index];
    while (current_node != NULL) {
      // Looping through all linked list nodes until we arrive at NULL
      if(strcmp(key, current_node->key) == 0) {
        // If the node's key matches the provided key, return the node's value.
        return current_node->value;
      }
      // Repeat loop with the next node
      current_node = current_node->next;
    }
  }
  return NULL;
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{

  for (int i = 0; i < ht->capacity; i++) {
    if (ht->storage[i]) {
      // If there is a node at hashed index, create a variable
      // which represents the next node.
      LinkedPair *nextnode = ht->storage[i]->next;
      destroy_pair(ht->storage[i]);
      while (nextnode) {
        LinkedPair *currentnode = nextnode;
        LinkedPair *nextnode = currentnode->next;
        destroy_pair(currentnode);
      }
    }
  }
  free(ht->storage);
  free(ht);

}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht = malloc(sizeof(HashTable));
  new_ht->capacity = ht->capacity*2;
  new_ht->storage = calloc(ht->capacity*2, sizeof(LinkedPair *));
  // use calloc to allocate memory for twice the capacity of the original
  // hash table and for sizeof a linkedpair struct

  for (int i = 0; i < ht->capacity; i++) {
    if (ht->storage[i]) {
      hash_table_insert(new_ht, ht->storage[i]->key, ht->storage[i]->value);
      // insert each key/value from original ht to new ht as LinkedPairs
      // use the insert method to do this, which creates LinkedPairs
      LinkedPair *currentnode = ht->storage[i]->next;
      while (currentnode) {
        // If there is a truthy value for each node at the index,
        // loop through each one and insert it into the same index on the new hash table
        hash_table_insert(new_ht, currentnode->key, currentnode->value);
        currentnode = currentnode->next;
      }
    }
  }

  destroy_hash_table(ht);
  return new_ht;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
