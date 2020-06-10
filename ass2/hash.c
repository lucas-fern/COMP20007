/* * * * * * *
 * Hashing module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Lucas Fern
 */

#include "hash.h"

// Implements a solution to Problem 1 (a), which reads in from stdin:
//   N M
//   str_1
//   str_2
//   ...
//   str_N
// And outputs (to stdout) the hash values of the N strings 1 per line.
void problem_1_a() {
  int line_count, mod_num, i = 0;
  char **strings;
  
  scanf("%d %d\n", &line_count, &mod_num); // take N and M

  strings = (char **)malloc(line_count * sizeof(char *));

  // Populate an array with all the strings to be hashed
  while (i < line_count) {
    strings[i] = (char *)malloc((MAXCHARS+1) * sizeof(char));
    assert(strings[i]);
    fgets(strings[i], MAXCHARS, stdin);
    
    if (strings[i][strlen(strings[i])-1] == '\n') {
      strings[i][strlen(strings[i]) - 1] = '\0';
    }
    i++;
  }

  // Iteratively generate the hash values for each string
  i=0;
  while (i < line_count) {
    printf("%d\n", hash(strings[i], mod_num));
    i++;
  }

  for (i=0; i<line_count; i++) free(strings[i]);
  free(strings);
}

// Returns the position a string should be placed in a hash table.
int hash(char *string, int mod_num) {
  int i=0, letter, scaled_letter;
  char *bin_char;
  strnumber_t bin_hash, decimal;

  memset(bin_hash, 0, sizeof(bin_hash)); // Empty the array or 
                                         // funky things happen

  // Generate the binary value of all the concatenated chars in the string
  while ((letter = string[i]) != '\0') {
    scaled_letter = alphanum_to_int(letter);
    bin_char = int_to_bin(scaled_letter);
    strcat(bin_hash, bin_char);

    free(bin_char);
    i++;
  }
  // Covert the binary value to a base-10 integer
  strbin_to_strint(bin_hash, decimal);

  // Return the big integer mod M as an int (no longer string)
  return mod_string(decimal, mod_num);
}

// Covert a binary number stored as a character string into a base-10 string.
void strbin_to_strint(strnumber_t bin_string, strnumber_t decimal) {
  int i;

  decimal[0] = bin_string[0];
  decimal[1] = '\0';

  // Using horner's method we can do the conversion using only two operations,
  // doubling and addition of 1.
  for (i=1; i<strlen(bin_string); i++) {
    string_double(decimal);
    if (bin_string[i] == '1') {
      string_add_1(decimal);
    }
  }
}

// Double the value of an integer stored in a string. The array must have extra
// space available if the doubling will cause an extra significant figure.
void string_double(strnumber_t a) {
  int i, temp, carry=0;

  for (i=strlen(a)-1; i>=0; i--) {
    temp = (a[i] - '0') * 2 + carry;
    carry = temp / 10;  // 10 is the base of base-10 numbers (duh)
    a[i] = (char)((temp % 10) + '0');
  }
  if (carry) {
    shift_str_right(a);
    a[0] = carry + '0';
  }
}

// Add 1 to the value of an integer stored in a string. The array must have
// space available if the addition will cause an extra significant figure.
void string_add_1(strnumber_t a) {
  int i;

  for (i=strlen(a)-1; i>=0; i--) {
    if (a[i] != '9') {
      a[i] = (a[i] + 1);
      return;

    } else if (a[i] == '9') {
      a[i] = '0';
      if (i == 0) {
        shift_str_right(a);
        a[i] = '1';
        return;
      }
    }
  }
}

// Shifts a string one place to the right in an array and fills the empty space
// in the left with a '0'.
void shift_str_right(strnumber_t a) {
  int i;

  for (i=strlen(a); i>=0; i--) { // start at strlen(a) to catch the '\0'
    a[i+1] = a[i];
  }
  a[0] = '0';
}

// Takes an integer a stored in a string and returns an integer a mod m.
// Uses the distributive properties of the modulo operator to work around
// intger limits enforced by C.
//   Addition:         (A+B) %m = (A%m + B%m) %m
//   Multiplication:   (A*B) %m = (A%m * B%m) %m
int mod_string(strnumber_t a, int m) {
  int ans = 0, i;

  for (i=0; i<strlen(a); i++) {
    ans = (ans*10 + (a[i] - '0')) % m;
  }
  return ans;
}

// Takes an integer and returns a character array containing the binary
// representation of the integer as a string.
char *int_to_bin(int n) {
  char *out;
  int i;

  out = (char *)malloc((BINSTRLEN + 1) * sizeof(char));
  assert(out);

  for (i=BINSTRLEN-1; i>=0; i--, n/=2) {
    out[i] = "01"[n % 2];
  }

  out[BINSTRLEN] = '\0';
  return out;
}

// Takes a letter or number and returns an integer according to the encoding
// specified in the project specification.
int alphanum_to_int(char letter) {
  if ('a' <= letter && letter <= 'z') {
    return letter - 'a';
  } else if ('A' <= letter && letter <= 'Z') {
    return letter - 'A' + 26; // 26 is the offset of the capital letters from 0
  } else if ('0' <= letter && letter <= '9') {
    return letter - '0' + 52; // 52 is the offset of the numbers in our encoding
  } else {
    return -1; // An invalid character must have been passed in
  }
}

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
void problem_1_b() {
  int line_count, len, count=0, k, i;
  char **hash_table, *word;

  scanf("%d %d %d\n", &line_count, &len, &k); // take N, M and K

  // Initialise the hash table and an array to store the words in the order they
  // are input to NULL
  hash_table = (char **)malloc(len * sizeof(char *));
  assert(hash_table);
  for (i=0; i<len; i++) hash_table[i] = NULL;

  // Read in all the words to the hash table
  for (i=0; i < line_count; i++) {
    word = (char *)malloc((MAXCHARS + 1) * sizeof(char));
    assert(word);
    fgets(word, MAXCHARS, stdin);

    if (word[strlen(word)-1] == '\n') {
      word[strlen(word)-1] = '\0';
    }
    hash_table_insert(&hash_table, word, &len, &count, k);
  }

  print_hash_table(&hash_table, len);
  // Free each of the strings and the array containing them
  for (i = 0; i < len; i++) free(hash_table[i]);
  free(hash_table);
}

// Insert a string into the hash table, increase the count of values in the
// table. Uses linear probing with step size k and calls double_table() if the 
// until the element fits.
void hash_table_insert(char ***hash_table, char *word, 
                       int *len, int *count, int k) {
  int pos, start;

  pos = hash(word, *len); // Get the hash value of the word

  if (!(*hash_table)[pos]) { // If the position it wants to go isn't taken
                             // put the word there...
    (*hash_table)[pos] = word;
    (*count)++;
    return;

  } else { // ...otherwise linear probe.
    start = pos;
    pos = (pos + k) % *len;

    while (pos != start) {
      if (!(*hash_table)[pos]) {
        (*hash_table)[pos] = word;
        (*count)++;
        return;
      }
      pos = (pos + k) % *len;

    } // If linear probing fails, increase the table size and retry;
    double_tables(hash_table, word, len, count, k);
  }
}

// Double the size of a hash table and call back to hash_table_insert() to 
// reinsert all of the elements back into the new table calling in the order
// that they appeared in the original. Then insert the new word.
void double_tables(char ***hash_table, char *word, 
                   int *len, int *count, int k) {
  int i=0;
  char **temp;

  (*len) *= 2;

  // Initialise a double-sized array
  temp = (char **)malloc((*len) * sizeof(char *));
  for (i=0; i<(*len); i++) temp[i] = NULL;
  assert(temp);

  // Fill the new array with the old elements
  (*count) = 0;
  for (i=0; i<(*len)/2; i++) {
    if ((*hash_table)[i]) {
      hash_table_insert(&temp, (*hash_table)[i], len, count, k);
    }
  }

  hash_table_insert(&temp, word, len, count, k);

  *hash_table = temp;  // Reassign the original pointer to the new big table
}

// Prints out a hash table in the format:
//   0: str_k
//   1:
//   2: str_l
//   3: str_p
//   4:
//   ...
//   (M-2): str_q
//   (M-1):
void print_hash_table(char ***hash_table, int len) {
  int i;

  for (i=0; i<len; i++) {
    if ((*hash_table)[i]) {
      printf("%d: %s\n", i, (*hash_table)[i]);
    } else {
      printf("%d:\n", i);
    }
  } 
}
