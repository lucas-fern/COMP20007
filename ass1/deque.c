/* * * * * * *
 * Deque module (i.e., double ended queue) for Assignment 1
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Lucas Fern
 */

// You must not change any of the code already provided in this file, such as
// type definitions, constants or functions.
//
// You may, however, add additional functions and/or types which you may need
// while implementing your algorithms and data structures.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "deque.h"
#include "util.h"

// Create a new empty Deque and return a pointer to it
Deque *new_deque() {
	Deque *deque = malloc(sizeof(*deque));
	assert(deque);

	deque->top = NULL;
	deque->bottom = NULL;
	deque->size = 0;

	return deque;
}

// Free the memory associated with a Deque
void free_deque(Deque *deque) {
  // Remove (and thus free) all of the nodes in the Deque.
  while (deque->size > 0) {
    deque_remove(deque);
  }

	// Free the deque struct itself
	free(deque);
}

// Create a new Node with a given piece of data
Node *new_node(Data data) {
  Node *node = malloc(sizeof(*node));
  assert(node);

  node->next = NULL;
  node->prev = NULL;
  node->data = data;

  return node;
}

// Free the memory associated with a Node
void free_node(Node *node) {
  free(node);
}

// Add an element to the top of a Deque
void deque_push(Deque *deque, Data data) {
  Node *new = new_node(data);

  if (deque->size > 0) {
    new->next = deque->top;
    deque->top->prev = new;
  } else {
    // If the Deque was initially empty then new is both the top and bottom
    deque->bottom = new;
  }

  deque->top = new;
  deque->size++;
}

// Add an element to the bottom of a Deque
void deque_insert(Deque *deque, Data data) {
  Node *new = new_node(data);

  if (deque->size > 0) {
    new->prev = deque->bottom;
    deque->bottom->next = new;
  } else {
    // If the Deque was initially empty then new is both the top and bottom
    deque->top = new;
  }

  deque->bottom = new;
  deque->size++;
}

// Remove and return the top element from a Deque
Data deque_pop(Deque *deque) {
  if (deque->size == 0) {
    exit_with_error("can't pop from empty Deque");
  }

  Data data = deque->top->data;
  Node *old_top = deque->top;

  if (deque->size == 1) {
    deque->top = NULL;
    deque->bottom = NULL;
  } else {
    deque->top = old_top->next;
    deque->top->prev = NULL;
  }

  deque->size--;

  free(old_top);

  return data;
}

// Remove and return the bottom element from a Deque
Data deque_remove(Deque *deque) {
  if (deque->size == 0) {
    exit_with_error("can't remove from empty Deque");
  }

  Data data = deque->bottom->data;
  Node *old_bottom = deque->bottom;

  if (deque->size == 1) {
    deque->top = NULL;
    deque->bottom = NULL;
  } else {
    deque->bottom = old_bottom->prev;
    deque->bottom->next = NULL;
  }

  deque->size--;

  free(old_bottom);

  return data;
}

// Return the number of elements in a Deque
int deque_size(Deque *deque) {
  return deque->size;
}

// Print the Deque on its own line with the following format:
//   [x_1, x_2, ..., x_n]
//     ^              ^
//    top           bottom
void print_deque(Deque *deque) {
  Node *current = deque->top;
  int i = 0;

  printf("[");

  while (current) {
    printf("%d", current->data);
    // Print a comma unless we just printed the final element
    if (i < deque->size - 1) {
      printf(", ");
    }
    current = current->next;
    i++;
  }

  printf("]\n");
}

// Reverse the Deque using an iterative approach
void iterative_reverse(Deque *deque) {
  // Switch the pointers to the top and bottom
	Node *temp = deque->top, *current;
	deque->top = deque->bottom;
	deque->bottom = temp;

  // Swap the previous and next pointers in each node
	current = deque->top;
	while (current) {
		temp = current->prev;
		current->prev = current->next;
		current->next = temp;
		current = current->next;
	}
}

// Reverse the Deque using a recursive approach
void recursive_reverse(Deque *deque) {
  // Switch the pointers to the top and bottom
  Node *temp = deque->top;
  deque->top = deque->bottom;
  deque->bottom = temp;

  recursive_reverse_from_end(deque->top);
}

void recursive_reverse_from_end(Node *end) {
  if (!end){
    return;
  } else { // Swap the previous and next pointers in the node
    Node *temp = end->prev;
    end->prev = end->next;
    end->next = temp;
    recursive_reverse_from_end(end->next);
  }
}

// Split the Deque given a critical value k, such that the Deque contains
// all elements greater than equal to k above (i.e., closer to the top)
// the elements less than k.
//
// Within the two parts of the array (>= k and < k) the elements should
// be in their original order.
//
// This function must run in linear time.
void split_deque(Deque *deque, int k) {
  Node *current = deque->top, *next, *start_gt = NULL, *start_lt = NULL,
   *end_gt = NULL, *end_lt = NULL;

  while (current) {
    next = current->next;

    // Link all the nodes that have data greater than or equal to k
    if (current->data >= k) {
      if (!start_gt) {
        start_gt = current;
        start_gt->prev = NULL;
        start_gt->next = NULL;
        end_gt = start_gt;
      } else {
        end_gt->next = current;
        current->prev = end_gt;
        current->next = NULL;
        end_gt = current;
      }
    // Link all the nodes that have data less than k
    } else if (current->data < k) {
      if (!start_lt) {
        start_lt = current;
        start_lt->prev = NULL;
        start_lt->next = NULL;
        end_lt = start_lt;
      } else {
        end_lt->next = current;
        current->prev = end_lt;
        current->next = NULL;
        end_lt = current;
      }
    }
    current = next;
  }

  // Link the chains of greater than and less than nodes and point the
  // deque to the head and tail
  if (end_gt && start_lt) {
    end_gt->next = start_lt;
    start_lt->prev = end_gt;
    deque->top = start_gt;
    deque->bottom = end_lt;
  } else if (start_gt && !start_lt) {
    deque->top = start_gt;
    deque->bottom = end_gt;
  } else if (!start_gt && start_lt) {
    deque->top = start_lt;
    deque->bottom = end_lt;
  }
}

// Check if a piece of data is present in a deque
bool in_deque(Deque *deque, Data data) {
  if (!deque) return false;

  Node *current = deque->top;

  while (current) {
    if (current->data == data) {
      return true;
    }
    current = current->next;
  }
  return false;
}
