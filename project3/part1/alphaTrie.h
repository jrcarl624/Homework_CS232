#ifndef INDEX_PAGE_H_
#define INDEX_PAGE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define MAX_BUFSIZE 300000

/// Represents a node in a trie structure for storing and counting word occurrences.
typedef struct TrieNode {
  struct TrieNode *branches[ALPHABET_SIZE]; 
  int count; 
} TrieNode;

/// Creates a new TrieNode.
/// @returns Pointer to the newly created TrieNode, or NULL if memory allocation fails.
TrieNode *createTrieNode();

/// Adds an occurrence of a word to the trie.
/// @param word The word to add.
/// @param wordLength The length of the word.
/// @param root Pointer to the root of the trie.
/// @returns 0 if successful, 1 if an error occurs (e.g., root is NULL or memory allocation fails).
int addWordOccurrence(const char *word, const int wordLength, TrieNode *root);

/// Frees the memory of the trie and all its child nodes recursively.
/// @param root Pointer to the root of the trie to free.
void freeTrie(TrieNode *root);

/// Fetches the text content from a web page at the given address.
/// @param srcAddr The source web address.
/// @param buffer A pointer to the buffer where the fetched text will be stored.
/// @param bufSize The maximum size of the buffer.
/// @returns The number of bytes read into the buffer, or 0 if an error occurs.
int getText(const char *srcAddr, char *buffer, const int bufSize);

#endif