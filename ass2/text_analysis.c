/* * * * * * *
 * Text Analysis module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Lucas Fern
 */

#include "text_analysis.h"

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
void problem_2_a() {
  int line_count;
  trie_t *trie;
  
  scanf("%d\n", &line_count); // take N
  trie = build_trie(line_count);

  print_trie(trie->root);

  free_trie(trie->root);
  free(trie);
}

// Reads N lines from stdin and assembles the lines into a character level trie.
trie_t *build_trie(int line_count) {
  int i = 0, j;
  char *word, letter;
  trie_t *trie;

  word = (char*)malloc((MAXSTRLEN + 1) * sizeof(char));
  trie = new_trie();
  assert(word && trie);

  while (i<line_count) {
    j = 0;
    while ((letter = getchar()) != '\n') {
      word[j] = letter;
      j++;
    }
    word[j] = '$';
    word[j+1] = '\0';

    char_trie_insert(trie, word);
    i++;
  }

  return trie;
}

// Inserts a word into a trie character by character.
void char_trie_insert(trie_t *trie, char *word) {
  int i=0;
  char letter;
  node_t *current = trie->root;

  (trie->root->count)++;

  while ((letter = word[i]) != '\0') {
    if (!(current->children)[child_pos(letter)]) {
      (current->children)[child_pos(letter)] = new_node(letter);
      (current->children)[child_pos(letter)]->depth = (current->depth) + 1;
    }
    i++;
    ((current->children)[child_pos(letter)]->count)++;
    current = (current->children)[child_pos(letter)];
  }
}

// Prints out a trie recursively using preorder traversal.
void print_trie(node_t *root) {
  int i;

  if (root) {
    printf("%c\n", root->data, root->count);
    
    for (i=0; i<ALPHABETSIZE; i++) {
      print_trie((root->children)[i]);
    }
  }
}

// Use postorder traversal to free a trie data structure when given the root.
void free_trie(node_t *root) {
  int i;

  if (root) {
    for (i=0; i<ALPHABETSIZE; i++) {
      free_trie((root->children)[i]);
    }
  free(root);  
  }
}

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
void problem_2_b() {
  int line_count, prefix_len;
  char *prefix;
  trie_t *trie;

  scanf("%d %d\n", &line_count, &prefix_len); // take N and K
  trie = build_trie(line_count);
  prefix = (char *)malloc((prefix_len + 1) * sizeof(char));
  assert(prefix);

  build_prefix(trie->root, 0, prefix, prefix_len);

  free(prefix);
  free_trie(trie->root);
  free(trie);
}

// Recursively assembles a prefix of length K by reading from a trie
void build_prefix(node_t *node, int depth, char *prefix, int prefix_len) {
  if (!node) return;  // Oops! we visited an empty node

  int i;
  if (depth-1 >= 0) prefix[depth-1] = node->data;

  if (depth == prefix_len) {
    prefix[depth] = '\0';
    printf("%s %d\n", prefix, node->count);
  } else {
    for (i=2; i<ALPHABETSIZE; i++) {  // 2 is the position of 'a' in the
                                      // children array so we start there
      build_prefix((node->children)[i], depth+1, prefix, prefix_len);
    }
  }
}

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
void problem_2_c() {
  int line_count, total_suffix_count, *suffix_counts, suffix_num = 0, i = 0;
  char prefix[MAXSTRLEN+1], **suffixes, word[MAXSTRLEN+1], letter;
  node_t *prefix_end;
  trie_t *trie;

  scanf("%d\n", &line_count); // take N
  fgets(prefix, MAXSTRLEN, stdin);
  if (prefix[strlen(prefix)-1] == '\n') prefix[strlen(prefix)-1] = '\0';

  trie = build_trie(line_count);

  // Locate the node containing the last letter of the prefix
  prefix_end = trie->root;
  while ((letter = prefix[i]) != '\0') {
    prefix_end = (prefix_end->children)[child_pos(letter)];
    i++;
  }

  // Initialise the suffix and suffix counting arrays
  total_suffix_count = prefix_end->count;
  suffixes = (char **)malloc((total_suffix_count+1) * sizeof(char *));
  suffix_counts = (int *)malloc((total_suffix_count+1) * sizeof(int));
  assert(suffixes && suffix_counts);
  for (i=0; i<(total_suffix_count+1); i++) {
    suffixes[i] = (char *)malloc((MAXSTRLEN+1)*sizeof(char));
    assert(suffixes[i]);
    suffixes[i][0] = '\0';
  }

  // Build the array of suffixes
  insert_suffixes(prefix_end, prefix_end, suffixes, suffix_counts, &suffix_num, 
                  word);

  // Print the output
  sort_arrays(suffixes, suffix_counts, total_suffix_count);
  prefix[strlen(prefix)-1] = '\0';  // Cut the last letter off the prefix
                                    // since it's contained in the suffixes
  for (i = 0; i < MAXPRINTAMOUNT; i++) {
    if (suffixes[i][0]) {
      printf("%.2f %s%s\n", ((double)suffix_counts[i]/total_suffix_count), 
             prefix, suffixes[i]);
    } 
  }

  // Free all the memory
  free_trie(trie->root);
  free(trie);
  for (i = 0; i < total_suffix_count; i++) free(suffixes[i]);
  free(suffixes);
  free(suffix_counts);
}

// Sort the array of frequencies and suffixes using insertion sort; uses 
// insertion sort for stability as the suffixes are already sorted.
void sort_arrays(char **strings, int *ints, int max_len) {
  int temp_num, i=1, j;
  char temp_word[MAXSTRLEN+1];

  // Loop over the arrays and swap elements to achieve decreasing order.
  while (strings[i][0] && (i<max_len)) {
    temp_num = ints[i];
    strcpy(temp_word, strings[i]);
    j = i - 1;
    while (j>=0 && (ints[j] < temp_num)) {
      ints[j+1] = ints[j];
      strcpy(strings[j+1], strings[j]);
      j--;
    }
    ints[j+1] = temp_num;
    strcpy(strings[j+1], temp_word);
    i++;
  }
}

// Inserts the strings that branch off of a node in a trie into an array.
void insert_suffixes(node_t *start, node_t *current, char **suffixes, 
                     int *suffix_counts, int *suffix_num, char *word) {
  int i;
  
  // If we reached the end of the word, record the word and move to the next
  if (current->data == '$') {
    word[current->depth - start->depth] = '\0';
    strcpy(suffixes[*suffix_num], word);
    suffix_counts[*suffix_num] = current->count;
    (*suffix_num)++;
  } else {  // Otherwise add the letter to the current word
    word[current->depth - start->depth] = current->data;
    for (i=1; i<ALPHABETSIZE; i++) {  // Start at 1 because thats where $ goes
      if ((current->children)[i]) {
        insert_suffixes(start, (current->children)[i], suffixes, suffix_counts,
                        suffix_num, word);
      }
    }
  }
}

// Returns the position a node would go in the children array of another node,
// also works to verify that a piece of data to be inserted in a node is valid.
int child_pos(char data) {
  if (data == '^') return 0;
  if (data == '$') return 1;
  if (('a' <= data) && (data <= 'z')) return (data - 'a' + 2);
  
  return -1;
}


/* NODE/TRIE FUNCTIONS */

// Makes a new node and fills the data with a given character.
node_t *new_node(char data) {
  int i;
  node_t *node;

  node = (node_t*)malloc(sizeof(node_t));
  assert(node);

  node->count = 0;
  for (i=0; i<ALPHABETSIZE; i++) (node->children)[i] = NULL;
  if ((i = child_pos(data)) >= 0) {
    node->data = data;
  } else {
    printf("Invalid data '%c' to be inserted into node.", data);
    exit(EXIT_FAILURE);
  }

  return node;
}

// Makes an empty trie.
trie_t *new_trie() {
  trie_t *trie;

  trie = (trie_t*)malloc(sizeof(trie_t));
  assert(trie);
  trie->root = new_node('^');
  trie->root->depth = 0; 

  return trie;
}
