/* * * * * * *
 * Hashing module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Lucas Fern
 */

#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MAXCHARS 256
#define BINSTRLEN 6

typedef char strnumber_t[MAXCHARS * BINSTRLEN + 1];

// Implements a solution to Problem 1 (a), which reads in from stdin:
//   N M
//   str_1
//   str_2
//   ...
//   str_N
// And outputs (to stdout) the hash values of the N strings 1 per line.
void problem_1_a();

// Returns the position a string should be placed in a hash table.
int hash(char *string, int mod_num);

// Covert a binary number stored as a character string into a base-10 string.
void strbin_to_strint(strnumber_t bin_string, strnumber_t decimal);

// Double the value of an integer stored in a string. The array must have extra
// space available if the doubling will cause an extra significant figure.
void string_double(strnumber_t a);

// Add 1 to the value of an integer stored in a string. The array must have
// space available if the addition will cause an extra significant figure.
void string_add_1(strnumber_t a);

// Shifts a string one place to the right in an array and fills the empty space
// in the left with a '0'.
void shift_str_right(strnumber_t a);

// Takes an integer a stored in a string and returns an integer a mod m.
// Uses the distributive properties of the modulo operator to work around
// intger limits enforced by C.
//   Addition:         (A+B) %m = (A%m + B%m) %m
//   Multiplication:   (A*B) %m = (A%m * B%m) %m
int mod_string(strnumber_t a, int m);

// Takes an integer and returns a character array containing the binary
// representation of the integer as a string.
char *int_to_bin(int n);

// Takes a letter or number and returns an integer according to the encoding
// specified in the project specification.
int alphanum_to_int(char letter);

// Implements a solution to Problem 1 (b), which reads in from stdin:
//   N M K
//   str_1
//   str_2
//   ...
//   str_N
// Each string is inputed (in the given order) into a hash table with size
// M. The collision resolution strategy must be linear probing with step
// size K. If an element cannot be inserted then the table size should be
// doubled and all elements should be re-hashed (in index order) before
// the element is re-inserted.
//
// This function must output the state of the hash table after all insertions
// are performed, in the following format
//   0: str_k
//   1:
//   2: str_l
//   3: str_p
//   4:
//   ...
//   (M-2): str_q
//   (M-1):
void problem_1_b();

// Insert a string into the hash table, increase the count of values in the
// table. Uses linear probing with step size k and calls double_table() if the 
// until the element fits.
void hash_table_insert(char ***hash_table, char *word, 
                       int *len, int *count, int k);

// Double the size of a hash table and call back to hash_table_insert() to 
// reinsert all of the elements back into the new table calling in the order
// that they appeared in the original. Then insert the new word.
void double_tables(char ***hash_table, char *word, 
                   int *len, int *count, int k);

// Prints out a hash table in the format:
//   0: str_k
//   1:
//   2: str_l
//   3: str_p
//   4:
//   ...
//   (M-2): str_q
//   (M-1):
void print_hash_table(char ***hash_table, int len);

#endif
