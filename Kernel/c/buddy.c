// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef CUSTOM
#include <memoryManager.h>
// #include <buddy.h>

#define HEAP_START 0x800000

#define HEADER_SIZE 8
#define MIN_ALLOC_LOG2 4 // 2^4 = 16
#define MIN_ALLOC ((size_t)1 << MIN_ALLOC_LOG2) // 1 << 4 = 1 * 2^4 = 1 * 16 = 16 (en size_t)

#define MAX_ALLOC_LOG2 21 // = 2MB
#define MAX_ALLOC ((size_t)1 << MAX_ALLOC_LOG2)

#define BUCKET_COUNT (MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2 + 1) // = 21 - 4 + 1 = 18

#define PARENT(i) ((i - 1) / 2)
#define LEFT_CHILD(i) (i)*2 + 1
#define RIGHT_CHILD(i) (i)*2 + 2
#define SIBLING(i) (((i) - 1) ^ 1) + 1

typedef struct list_t {
  struct list_t *prev, *next;
} list_t;

static list_t buckets[BUCKET_COUNT];
static size_t bucket_limit;

static uint8_t node_is_split[(1 << (BUCKET_COUNT - 1)) / 8]; // = 1 * 2^17 / 2^3 = 2^14

static uint8_t *base_ptr;

static size_t freeBytesRemaining = MAX_ALLOC;

/* ===== MANEJO DE LISTAS ===== */

static void list_init(list_t *list) {
  list->prev = list;
  list->next = list;
}

static void list_push(list_t *list, list_t *entry) {
  list_t *prev = list->prev;
  entry->prev = prev;
  entry->next = list;
  prev->next = entry;
  list->prev = entry;
}

static void list_remove(list_t *entry) {
  list_t *prev = entry->prev;
  list_t *next = entry->next;
  prev->next = next;
  next->prev = prev;
}

static list_t *list_pop(list_t *list) {
  list_t *back = list->prev;
  if (back == list) 
    return NULL;
  list_remove(back);
  return back;
}
/* ===== MANEJO DE LISTAS ===== */

static uint8_t *ptr_for_node(size_t index, size_t bucket) {
  return base_ptr + ((index - (1 << bucket) + 1) << (MAX_ALLOC_LOG2 - bucket));
}

static size_t node_for_ptr(uint8_t *ptr, size_t bucket) {
  return ((ptr - base_ptr) >> (MAX_ALLOC_LOG2 - bucket)) + (1 << bucket) - 1;
}

static int parent_is_split(size_t index) {
  index = PARENT(index);
  return (node_is_split[index / 8] >> (index % 8)) & 1;
}

static void flip_parent_is_split(size_t index) {
  index = PARENT(index);
  node_is_split[index / 8] ^= 1 << (index % 8);
}

static size_t bucket_for_request(size_t request) {
  size_t bucket = BUCKET_COUNT - 1;
  size_t size = MIN_ALLOC;

  while (size < request) {
    bucket--;
    size *= 2;
  }

  return bucket;
}

static int lower_bucket_limit(size_t bucket) {
  while (bucket < bucket_limit) {
    size_t root = node_for_ptr(base_ptr, bucket_limit);
    uint8_t *right_child;

    if (!parent_is_split(root)) {
      list_remove((list_t *)base_ptr);
      list_init(&buckets[--bucket_limit]);
      list_push(&buckets[bucket_limit], (list_t *)base_ptr);
      continue;
    }

    right_child = ptr_for_node(root + 1, bucket_limit);

    if (right_child + sizeof(list_t) > base_ptr + MAX_ALLOC) {
      return 0;
    }

    list_push(&buckets[bucket_limit], (list_t *)right_child);
    list_init(&buckets[--bucket_limit]);

    root = PARENT(root);
    if (root != 0) 
        flip_parent_is_split(root);
  }
  return 1;
}

static void initializeHeap() {
    base_ptr = (uint8_t *) HEAP_START;
    bucket_limit = BUCKET_COUNT - 1;
    list_init(&buckets[BUCKET_COUNT - 1]);
    list_push(&buckets[BUCKET_COUNT - 1], (list_t *)base_ptr);
}

void * malloc(size_t request) {
  size_t original_bucket, bucket;

  if (request + HEADER_SIZE > MAX_ALLOC)
    return NULL;

  if (base_ptr == NULL)
    initializeHeap();

  bucket = bucket_for_request(request + HEADER_SIZE);
  original_bucket = bucket;

  while (bucket + 1 != 0) {
    size_t i;
    uint8_t *ptr;

    if (!lower_bucket_limit(bucket)) 
      return NULL;
    
    
    ptr = (uint8_t *)list_pop(&buckets[bucket]);
    if (ptr == NULL) {

      if (bucket != bucket_limit || bucket == 0) {
        bucket--;
        continue;
      }

      if (!lower_bucket_limit(bucket - 1)) 
        return NULL;
      

      ptr = (uint8_t *)list_pop(&buckets[bucket]);
    }

    i = node_for_ptr(ptr, bucket);
    if (i != 0) {
      flip_parent_is_split(i);
    }

    while (bucket < original_bucket) {
      i = LEFT_CHILD(i);
      bucket++;
      flip_parent_is_split(i);
      list_push(&buckets[bucket], (list_t *)ptr_for_node(i + 1, bucket)); // ES EL HIJO DERECHO
    }

    freeBytesRemaining -= (1 << (BUCKET_COUNT + 1 - bucket));

    *(size_t *)ptr = request;
    return ptr + HEADER_SIZE;
  }

  return NULL;
}

void free(void *ptr) {
  size_t bucket, i;

  if (ptr == NULL)
    return;
  
  ptr = (uint8_t *)ptr - HEADER_SIZE;
  bucket = bucket_for_request(*(size_t *)ptr + HEADER_SIZE);
  i = node_for_ptr((uint8_t *)ptr, bucket);

  while (i != 0) {

    flip_parent_is_split(i);

    if (parent_is_split(i) || bucket == bucket_limit)
      break;
    
    list_remove((list_t *)ptr_for_node(SIBLING(i), bucket));
    i = PARENT(i);
    bucket--;
  }
  freeBytesRemaining += (1 << (BUCKET_COUNT + 1 - bucket));
  list_push(&buckets[bucket], (list_t *)ptr_for_node(i, bucket));
}

void mem(){
    print("Memoria total: ");
    printDec(MAX_ALLOC);
    print(" bytes\n");

    print("Memoria en uso: ");
    printDec(MAX_ALLOC - freeBytesRemaining);
    print(" bytes\n");

    print("Memoria libre: ");
    printDec(freeBytesRemaining);
    print(" bytes\n");
}
#endif