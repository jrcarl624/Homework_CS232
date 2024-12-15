
#ifndef CRAWLER_H_
#define CRAWLER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "alphaTrie.h"

#define MAX_ADDR_LENGTH 1000

/// Represents a node in a linked list of pages.
/// Each node contains the page address, the number of terms indexed,
/// a pointer to a trie structure for the terms, and a pointer to the next node.
typedef struct PageNode {
  char addr[MAX_ADDR_LENGTH];
  int nTerms; 
  struct TrieNode *terms; 
    struct PageNode *next; 
} PageNode;

/// Creates and returns a new `PageNode` instance.
/// @return A pointer to the new `PageNode`, or NULL if memory allocation fails.
PageNode *createPageNode();

/// Checks if a given address is already indexed in the list starting at `node`.
/// @param node The starting node of the list.
/// @param addr The address to check.
/// @return 1 if the address is found, 0 otherwise.
int isAddressIndexed(const PageNode *node, const char *addr);

/// Inserts a `PageNode` at the end of the linked list starting at `root`.
/// @param root The root node of the list.
/// @param node The `PageNode` to insert.
void insertBackPage(PageNode *root, PageNode *node);

/// Frees the memory associated with the given `PageNode` and all subsequent nodes in the list.
/// @param node The starting node of the list to free.
void freePageList(PageNode *node);

/// Retrieves a link from the page at the given source address.
/// @param srcAddr The source web address.
/// @param link A pointer to a buffer to store the retrieved link.
/// @param maxLinkLength The maximum length of the buffer.
/// @return 1 if a link was found, 0 otherwise. If a link is found, the `link` buffer is updated.
int getLink(const char *srcAddr, char *link, const int maxLinkLength);

/// Crawls a set of pages starting from a root, up to a maximum number of pages.
/// @param MAX_N The maximum number of pages to crawl.
/// @param root A pointer to the root of the page list.
/// @param dirPath The path to the file containing seed URLs and hop counts.
/// @return The total number of pages crawled.
int crawl(const int MAX_N, PageNode **root, char *dirPath);

/// Indexes the content of a page by extracting terms and adding them to the page's trie.
/// @param node The `PageNode` representing the page to index.
/// @return 0 if successful, or 1 if an error occurs (e.g., no content found or indexing fails).
int indexPage(PageNode *node);

#endif
