/* File: indexPage.c */
/* Author: Britton Wolfe */
/* Date: September 3rd, 2010 */

/* This program indexes a web page, printing out the counts of words on that
 * page */
// git
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define MAX_BUFSIZE 300000
// TODO: structure definitions
typedef struct TrieNode {
  struct TrieNode *branches[ALPHABET_SIZE];
  int count;
} TrieNode;

/// Creates a new node
/// @param data - Takes a non owning reference to char
/// @returns TrieNode - NULL if allocation failed
TrieNode *createNode() {
  TrieNode *newNode = malloc(sizeof(TrieNode));
  // Check if its null, if it is return.
  if (newNode == NULL)
    return newNode;

  // Initialize the array values to null.
  for (int i = 0; i < ALPHABET_SIZE; ++i) {
    newNode->branches[i] = NULL;
  }

  newNode->count = 0;

  return newNode;
}

/*
   NOTE: int return values can be used to indicate errors (typically non-zero)
   or success (typically zero return value)
*/

/* TODO: change this return type */ // changed to int -Conner
struct TrieNode *indexPage(const char *url);

int addWordOccurrence(const char *word, const int wordLength, TrieNode *root);

void printTrieContents(TrieNode *root, char *word, int wordLen);

int freeTrieMemory(TrieNode *head);

int getText(const char *srcAddr, char *buffer, const int bufSize);

int main(int argc, char **argv) {

  // TODO: write the (simple) main function
  // argv[1] will be the URL to index, if argc > 1
  if (argc < 2) {
    printf("no URL was entered.");
    return 1;
  }

  const char *url = argv[1];       // get the url from the terminal
  TrieNode *root = indexPage(url); // set root to root from indexPage
  char word[1000];
  printTrieContents(root, word, 0);
  freeTrieMemory(root);

  return 0;
}

/* TODO: define the functions corresponding to the above prototypes */

/* TODO: change this return type */ // changed return type to int for error
                                    // handling -Conner
struct TrieNode *indexPage(const char *url) {
  printf("%s\n", url);

  // Buffer to hold the fetched text from the URL
  char buffer[MAX_BUFSIZE]; // TODO: Make this a define; done!
  int bytesRead = getText(url, buffer, sizeof(buffer));

  if (bytesRead <= 0) // error: no words in the file
  {
    printf("No text found from url: %s\n", url);
    return NULL;
  }

  TrieNode *root = createNode();

  if (root == NULL) {
    return root;
  }

  // yes, it tokenizes the input into smaller pieces. it is precisely what is
  // needed for this project
  char *token = strtok(buffer, " \n1234567890\t,_.!?;:\"'()[]{}<>");

  while (token) {
    for (int i = 0; token[i]; i++) {
      if (token[i] >= 'A' && token[i] <= 'Z') {
        token[i] += 32; // convert to lowercase
      }
    }

    // successfully added word to the Trie
    if (addWordOccurrence(token, strlen(token), root) == 0) {
      printf("\t%s\n", token); // Changed by Josh
    } else {
      // Return root if error.
      return root;
    }

    token = strtok(NULL, " \n1234567890\t,_.!?;:\"'()[]{}<>"); // seperators
  }
  return root;
}

/// Increments the occurrence of the word passed into the function within the
/// trie
/// \param word - A word to count.
/// \param wordLength - The length of the word to count.
/// \param root - The trie to store the count on.
/// \returns int - NULL if no error, 1 if root is NULL and word length is not.
int addWordOccurrence(const char *word, const int wordLength, TrieNode *root) {

  // Increment the count if at the end of the list.
  if (wordLength == 0) {
    (root->count) += 1;
    return 0;
  }

  if (root == NULL)
    return 1;

  // Offset by a lowercase 'a' to get the index into the chars
  int index = (*word) - 'a';

  TrieNode *node = root->branches[index];

  // Check if it exists
  if (node == NULL) {

    // If it does not exist create the node and set `node` to the new location
    node = createNode();

    // If the node is still NULL the allocation failed.
    if (node == NULL)
      return 1;

    // Set the index to the current location.
    root->branches[index] = node;
  }

  // Recurse though the tree
  return addWordOccurrence(word + 1, wordLength - 1, node);
}

void printTrieContents(TrieNode *root, char *word, int wordLen) {
  if (root == NULL) { // base case
    return;
  }

  if (root->count > 0) {  // word has count, print word and count
    word[wordLen] = '\0'; // null terminate for formatting
    printf("%s: %d\n", word, root->count);
  }

  for (int i = 0; i < ALPHABET_SIZE; i++) { // iterate thru all branches
    if (root->branches[i] !=
        NULL) { // if the branch has any contents in its children
      word[wordLen] = ('a') + i;
      printTrieContents(root->branches[i], word, wordLen + 1);
    }
  }
}

/// Frees the memory in the tree and its leaves recursively
/// @returns int - 1 if error else NULL;; changed to 1
int freeTrieMemory(TrieNode *root) {
  if (root == NULL)
    return 0;

  // For each child call freeTrieMemory recursively
  for (int i = 0; i < ALPHABET_SIZE; i++) {
    freeTrieMemory(root->branches[i]);
  }

  // Free after each child has also been freed.
  free(root);

  return 0; // changed NULL to 0 to indicate success
}

/* You should not need to modify this function */
int getText(const char *srcAddr, char *buffer, const int bufSize) {
  FILE *pipe;
  int bytesRead;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getText.py", srcAddr);

  pipe = popen(buffer, "r");
  if (pipe == NULL) {
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n", buffer);
    return 0;
  }

  bytesRead = fread(buffer, sizeof(char), bufSize - 1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);

  return bytesRead;
}