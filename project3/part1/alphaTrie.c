#include "alphaTrie.h"

TrieNode *createTrieNode() {
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
    node = createTrieNode();

    // If the node is still NULL the allocation failed.
    if (node == NULL)
      return 1;

    // Set the index to the current location.
    root->branches[index] = node;
  }

  // Recurse though the tree
  return addWordOccurrence(word + 1, wordLength - 1, node);
}

void freeTrie(TrieNode *root) {
  if (root == NULL)
    return;

  // For each child call freeTrie recursively
  for (int i = 0; i < ALPHABET_SIZE; ++i) {
    freeTrie(root->branches[i]);
  }

  // Free after each child has also been freed.
  free(root);
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
