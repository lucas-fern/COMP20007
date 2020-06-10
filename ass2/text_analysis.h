/* * * * * * *
 * Text Analysis module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Lucas Fern
 */

#ifndef TEXT_ANALYSIS_H
#define TEXT_ANALYSIS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


// Constants
#define ALPHABETSIZE   28   // 26 lowercase letters plus 2 for the '^' and '$'
#define MAXSTRLEN      100  // 99 character limit plus 1 for the '$'
#define MAXPRINTAMOUNT 5    // Maximum amount of values to print in task 2c

// Type Definitions
typedef struct node node_t;

struct node {
  char data;
  int count;
  int depth;
  node_t *children[ALPHABETSIZE];
};

typedef struct {
  node_t *root;
} trie_t;

/* FUNCTION PROTOTYPES */

// Build a character level trie for a given set of words.
//
// The input to your program is an integer N followed by N lines containing
// words of length < 100 characters, containing only lowercase letters.
//
// Your program should built a character level trie where each node indicates
// a single character. Branches should be ordered in alphabetic order.
//
// Your program must output the post-order traversal of the characters in
// the trie, on a single line.
void problem_2_a();

// Reads N lines from stdin and assembles the lines into a character level trie.
trie_t *build_trie(int line_count);

// Inserts a word into a trie character by character.
void char_trie_insert(trie_t *trie, char *word);

// Prints out a trie recursively using preorder traversal.
void print_trie(node_t *root);

// Use postorder traversal to free a trie data structure when given the root.
void free_trie(node_t *root);

// Using the trie constructed in Part (a) this program should output all
// prefixes of length K, in alphabetic order along with their frequencies
// with their frequencies. The input will be:
//   n k
//   str_0
//   ...
//   str_(n-1)
// The output format should be as follows:
//   an 3
//   az 1
//   ba 12
//   ...
//   ye 1
void problem_2_b();

// Recursively assembles a prefix of length K by reading from a trie
void build_prefix(node_t *node, int depth, char *prefix, int prefix_len);

// Again using the trie data structure you implemented for Part (a) you will
// provide a list (up to 5) of the most probable word completions for a given
// word stub.
//
// For example if the word stub is "al" your program may output:
//   0.50 algorithm
//   0.25 algebra
//   0.13 alright
//   0.06 albert
//   0.03 albania
//
// The probabilities should be formatted to exactly 2 decimal places and
// should be computed according to the following formula, for a word W with the
// prefix S:
//   Pr(word = W | stub = S) = Freq(word = W) / Freq(stub = S)
//
// The input to your program will be the following:
//   n
//   stub
//   str_0
//   ...
//   str_(n-1)
// That is, there are n + 1 strings in total, with the first being the word
// stub.
//
// If there are two strings with the same probability ties should be broken
// alphabetically (with "a" coming before "aa").
void problem_2_c();

// Sort the array of frequencies and suffixes using insertion sort; uses 
// insertion sort for stability as the suffixes are already sorted.
void sort_arrays(char **strings, int *ints, int max_len);

// Inserts the strings that branch off of a node in a trie into an array.
void insert_suffixes(node_t *start, node_t *current, char **suffixes, 
                     int *suffix_counts, int *suffix_num, char *word);

// Returns the position a node would go in the children array of another node,
// also works to verify that a piece of data to be inserted in a node is valid.
int child_pos(char data);

// Makes a new node and fills the data with a given character.
node_t *new_node(char data);

// Makes an empty trie.
trie_t *new_trie();

#endif
