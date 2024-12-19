// Copyright 2024 Bernie Liu
// Author: Bernie Liu
// UWNetID: bliu29
// Homework 6
// 20/11/2024
// Manages blocks of memory dynamically

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include "mem.h"  // outward facing functions
#include "mem_internal.h"  // private functions

// Default values for us to use later on
#define NODESIZE sizeof(freeNode)
#define MINCHUNK 16        // smallest allowable chunk of memory
#define BIGCHUNK 16000     // default of a very large chunk size

// Global variables for convenience
// these are static so outside code can't use them.
static freeNode* freeBlockList;  // points to list of available memory blocks
static uintptr_t totalMalloc;  // keeps track of memory allocated with malloc

/* The following functions need to be defined to meet the interface
   specified in mem.h.  These functions return or take the 'usable'
   memory addresses that a user would deal with.  They are called
   in the bench code.
*/

/* getmem returns the address of a usable block of memory that is
   at least size bytes large.  This code calls the helper function
   'get_block'
   Pre-condition: size is a positive integer
*/
void* getmem(uintptr_t size) {
  assert(size > 0);

  // make sure size is a multiple of MINCHUNK (16):
  if (size % MINCHUNK != 0) {
    size = size + MINCHUNK -(size % MINCHUNK);
  }

  uintptr_t block = get_block(size);
  if (block == 0) {
    return NULL;
  }

  return((void*)(block+NODESIZE));  // offset to get usable address
}

/* freemem uses the functions developed to add blocks to the 
   list of available free blocks to return a node to the list.
   The pointer 'p' is the address of usable memory, allocated using getmem
*/
void freemem(void* p) {
  if (!p) {
    return;
  }

  freeNode* node=(freeNode*)((uintptr_t)p-NODESIZE);

  return_block((uintptr_t)node);
}

// Gets a block from the free list
// Parameters:
//  uintptr_t size: the size requested
// Returns:
//  uintptr_t: a block with at least size space
uintptr_t get_block(uintptr_t size) {
  freeNode* curr=freeBlockList;
  freeNode* prev=NULL;

  // loop through free list to find a large enough block, splitting if necessary
  // mallocs a new block if there isn't one large anough and runs the loop again
  while (curr!=NULL) {
    if (curr->size>=size) {
      if (curr->size >= (size*2)+MINCHUNK+NODESIZE) {
        split_node(curr, size);
      }
      if (prev) {
        prev->next=curr->next;
      } else {
        freeBlockList = curr->next;
      }
      check_heap();
      return (uintptr_t)curr;
    }
    prev=curr;
    curr=curr->next;
  }
  freeNode* newNode=new_block(size);
  if (!newNode) {
    return 0;
  }
  check_heap();
  return get_block(size);
}

// Mallocs a new block of a requested size
freeNode* new_block(int size) {
  uintptr_t total=size+NODESIZE;

  freeNode* node=(freeNode*)malloc(total);

  if (!node) {
    return NULL;
  }

  node->size=size;
  node->next=NULL;

  return_block((uintptr_t)node);

  totalMalloc+=total;
  return node;
}

// Splits a node into two smaller nodes
// The first node is the node that has the requested size
void split_node(freeNode* n, uintptr_t size) {
  freeNode* newNode=(freeNode*)((uintptr_t)n+NODESIZE+size);
  uintptr_t newSize=n->size-size-NODESIZE;
  newNode->size = newSize;
  newNode->next = n->next;

  n->size = size;
  n->next = newNode;
}

// Returns a block to the free list
void return_block(uintptr_t node) {
  freeNode* block=(freeNode*)node;

  freeNode* curr=freeBlockList;
  freeNode* prev=NULL;
  // find the right position
  while (curr!=NULL && (uintptr_t)curr < node) {
    prev=curr;
    curr=curr->next;
  }

  block->next=curr;

  if (prev==NULL) {
    freeBlockList=block;
  } else {
    prev->next=block;
  }


  // check adjacency constraints
  if (curr && adjacent(block)) {
    block->size+=curr->size + NODESIZE;
    block->next=curr->next;
  }

  if (prev && adjacent(prev)) {
    prev->size+=block->size+NODESIZE;
    prev->next=block->next;
  }
  check_heap();
}

// Checks if a block is next to or overlaps another block
int adjacent(freeNode* node) {
  if (node->next==NULL) {
    return 0;
  }
  return (uintptr_t)node+node->size+NODESIZE>=(uintptr_t)node->next;
}

/* The following are utility functions that may prove useful to you.
   They should work as presented, so you can leave them as is.
*/
void check_heap() {
  if (!freeBlockList) return;
  freeNode* currentNode = freeBlockList;
  uintptr_t minsize = currentNode->size;

  while (currentNode != NULL) {
    if (currentNode->size < minsize) {
      minsize = currentNode->size;
    }
    if (currentNode->next != NULL) {
      assert((uintptr_t)currentNode <(uintptr_t)(currentNode->next));
      assert((uintptr_t)currentNode + currentNode->size + NODESIZE
              <(uintptr_t)(currentNode->next));
    }
    currentNode = currentNode->next;
  }
  // go through free list and check for all the things
  if (minsize == 0) print_heap( stdout);
  assert(minsize >= MINCHUNK);
}

void get_mem_stats(uintptr_t* total_size, uintptr_t* total_free,
                   uintptr_t* n_free_blocks) {
  *total_size = totalMalloc;
  *total_free = 0;
  *n_free_blocks = 0;

  freeNode* currentNode = freeBlockList;
  while (currentNode) {
    *n_free_blocks = *n_free_blocks + 1;
    *total_free = *total_free + (currentNode->size + NODESIZE);
    currentNode = currentNode->next;
  }
}

void print_heap(FILE *f) {
  printf("Printing the heap\n");
  freeNode* currentNode = freeBlockList;
  while (currentNode !=NULL) {
    fprintf(f, "%" PRIuPTR, (uintptr_t)currentNode);
    fprintf(f, ", size: %" PRIuPTR, currentNode->size);
    fprintf(f, ", next: %" PRIuPTR, (uintptr_t)currentNode->next);
    fprintf(f, "\n");
    currentNode = currentNode->next;
  }
}
