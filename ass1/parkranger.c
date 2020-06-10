/* * * * * * *
 * Park Ranger module for Assignment 1
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Lucas Fern
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "parkranger.h"
#include "util.h"
#include "deque.h"

// This function must read in a ski slope map and determine whether or not
// it is possible for the park ranger to trim all of the trees on the ski slope
// in a single run starting from the top of the mountain.
//
// The ski slope map is provided via stdin in the following format:
//
//   n m
//   from to
//   from to
//   ...
//   from to
//
// Here n denotes the number of trees that need trimming, which are labelled
// {1, ..., n}. The integer m denotes the number "reachable pairs" of trees.
// There are exactly m lines which follow, each containing a (from, to) pair
// which indicates that tree `to` is directly reachable from tree `from`.
// `from` and `to` are integers in the range {0, ..., n}, where {1, ..., n}
// denote the trees and 0 denotes the top of the mountain.
//
// For example the following input represents a ski slope with 3 trees and
// 4 reachable pairs of trees.
//
// input:            map:          0
//   3 4                          / \
//   0 1                         /  2
//   0 2                        / /
//   2 1                        1
//   1 3                          \
//                                 3
//
// In this example your program should return `true` as there is a way to trim
// all trees in a single run. This run is (0, 2, 1, 3).
//
// Your function should:
//  - Read in this data from stdin
//  - Store this data in an appropriate data structure
//  - Run the algorithm you have designed to solve this problem
//  - Do any clean up required (e.g., free allocated memory)
//  - Return `true` or `false` (included in the stdbool.h library)
//
// For full marks your algorithm must run in O(n + m) time.
bool is_single_run_possible() {
  Deque **adj_lists, *linearisation = new_deque();
  int vert, edges, x, y, i, j, **adj_matrix;
  bool status;

  // Read values from stdin and create adjacency list AND matrix
  scanf("%d %d", &vert, &edges);

  adj_lists = (Deque**)malloc((vert+1)*sizeof(Deque*));
  adj_matrix = (int**)malloc((vert+1)*sizeof(int*));
  
  for (i=0; i<=vert; i++) {
    adj_lists[i] = new_deque();
    adj_matrix[i] = (int*)malloc((vert+1)*sizeof(int));
    for (j=0; j<(vert+1); j++) {
      adj_matrix[i][j] = 0;
    }
  }

  while (scanf("%d %d", &x, &y)==2) {
    deque_insert(adj_lists[x], y);
    adj_matrix[x][y] = 1;
  }

  // Create a linearisation of the graph and check to see if every neighbouring
  // node in the linearisation is connected.
  linearise(vert, adj_lists, linearisation);
  status = validate_linearisation(adj_matrix, linearisation);

  // Free all malloc'ed data structures
  for (i=0; i<=vert; i++) {
    free_deque(adj_lists[i]);
    free(adj_matrix[i]);
  }
  free(adj_matrix);
  free_deque(linearisation);

  return status;
}

// Creates a linearisation of a DAG from an adjacency list made of deuqes
void linearise(int vert, Deque **adj_lists, Deque *linearisation) {
  int *marks, i, count;
  count = 0;

  marks = (int*)malloc((vert+1)*sizeof(int));

  for (i=0; i<=vert; i++) marks[i] = 0;

  for (i=0; i<=vert; i++) {
    if (!marks[i]) {
      dfs_explore(adj_lists, linearisation, marks, &count, i);
    }
  }
  free(marks);
}

// Performs a recursive depth first search and pushes any element popped from 
// the DFS onto a deque to create a linearisation
void dfs_explore(Deque **adj_lists, Deque *linearisation, int 
                 *marks, int *count, int i) {
  Node *current = adj_lists[i]->top;

  (*count)++;
  marks[i] = *count;

  // Explore all unexplored children of the current node
  while (current) {
    if (!marks[current->data]) {
      dfs_explore(adj_lists, linearisation, marks, count, current->data);
    }
    current = current->next;
  }
  deque_push(linearisation, i);
}

// Takes a linearisation of a DAG and an adjacency matrix and returns whether
// all consecutive nodes in the linearisation can be connected in the original
// graph, uses adjacency matrix to return in O(n) time.
bool validate_linearisation(int **adj_matrix, Deque *linearisation) {
  Node *current_node = linearisation->top, *next_node = current_node->next;

  // Check if current node has an edge to the next node
  while (next_node) {
    if (!adj_matrix[current_node->data][next_node->data]) {
      return false;
    }
    current_node = current_node->next;
    next_node = next_node->next;
  }
  return true;
}
