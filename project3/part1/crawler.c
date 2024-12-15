#include "crawler.h"

int crawl(const int MAX_N, PageNode **root, char *dirPath) {
  char startAddr[MAX_ADDR_LENGTH];
  char destAddr[MAX_ADDR_LENGTH];
  int page = 0;

  FILE *fp = fopen(dirPath, "r");
  if (fp == NULL) {
    fprintf(stderr, "File not found: %s\n", dirPath);
    return page;
  }

  while (page < MAX_N && fgets(startAddr, sizeof(startAddr), fp)) {
    if (*startAddr == '\0')
      continue; // Skip empty lines

    // split the line after the url and fetch the number of hops
    char *urlSplit = strchr(startAddr, ' ');
    if (urlSplit == NULL) {
      fprintf(stderr, "Invalid line (no space found): %s\n", startAddr);
      continue;
    }

    *urlSplit = '\0';
    // get the number of hops
    int hops = atoi(urlSplit + 1) + 1;

    // Taken from the first project
    for (int hop = 0; (hop < hops) && (page < MAX_N); ++hop) {

      if (!isAddressIndexed(*root, startAddr)) {

        PageNode *node = createPageNode();
        if (node == NULL) {
          fclose(fp);
          return page;
        }

        strncpy(node->addr, startAddr, MAX_ADDR_LENGTH - 1);
        node->addr[MAX_ADDR_LENGTH - 1] = '\0';

        if (indexPage(node)) {
          freePageList(node);
          fclose(fp);
          return page; // Stop on indexing error
        }

        if (*root == NULL) {
          *root = node;
        } else {
          insertBackPage(*root, node);
        }

        ++page;
      }
      if (!getLink(startAddr, destAddr, MAX_ADDR_LENGTH)) {
        fprintf(stderr, "No more links found from: %s\n", startAddr);
        break; // Stop crawling if no links are found
      }
      strncpy(startAddr, destAddr, MAX_ADDR_LENGTH - 1);
      startAddr[MAX_ADDR_LENGTH - 1] = '\0';
    }
  }

  fclose(fp);
  return page;
}

// returns 1 if it cant add a word occurrence
int indexPage(PageNode *node) {
  char buffer[MAX_BUFSIZE];
  int bytesRead = getText(node->addr, buffer, sizeof(buffer));

  printf("%s\n", node->addr);

  if (bytesRead <= 0) // error: no words in the file
  {
    printf("No text found from url: %s\n", node->addr);
    return 1;
  }

  char *token = strtok(buffer, " \n1234567890\\-\t,_.!?;:\"'()[]{}<>");

  for (; token; ++node->nTerms) {
    for (int i = 0; token[i]; ++i) {
      if (token[i] >= 'A' && token[i] <= 'Z') {
        token[i] += 32; // convert to lowercase
      }
    }

    int error;

    // If this function returns anything but an error
    if ((error = addWordOccurrence(token, strlen(token), node->terms))) {
      return error;
    }
    printf("\t%s\n", token);

    // TODO: look into this
    token = strtok(NULL, " \n1234567890\\-\t,_.!?;:\"'()[]{}<>"); // separators
  }
  return 0;
}

PageNode *createPageNode() {

  PageNode *node = malloc(sizeof(PageNode));

  if (node == NULL)
    return NULL;

  node->terms = createTrieNode();

  node->addr[0] = '\0'; // Make the string empty
  node->nTerms = 0;

  node->next = NULL;

  return node;
}

int isAddressIndexed(const PageNode *node, const char *addr) {

  if (node == NULL) {
    return 0;
  }

  if (!strcmp(node->addr, addr)) {
    return 1;
  }

  return isAddressIndexed(node->next, addr);
}

void insertBackPage(PageNode *root, PageNode *node) {
  if (node == NULL || root == NULL)
    return;
  if (root->next != NULL)
    return insertBackPage(root->next, node);

  root->next = node;
}

void freePageList(PageNode *node) {
  if (node == NULL)
    return;

  freeTrie(node->terms);
  freePageList(node->next);

  free(node);
}

int getLink(const char *srcAddr, char *link, const int maxLinkLength) {
  const int bufSize = 1000;
  char buffer[bufSize];

  int numLinks = 0;

  FILE *pipe;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getLinks.py", srcAddr);

  pipe = popen(buffer, "r");
  if (pipe == NULL) {
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n", buffer);
    return 0;
  }

  fscanf(pipe, "%d\n", &numLinks);

  if (numLinks > 0) {
    int linkNum;
    double r = (double)rand() / ((double)RAND_MAX + 1.0);

    for (linkNum = 0; linkNum < numLinks; linkNum++) {
      fgets(buffer, bufSize, pipe);

      if (r < (linkNum + 1.0) / numLinks) {
        break;
      }
    }

    /* copy the address from buffer to link */
    strncpy(link, buffer, maxLinkLength);
    link[maxLinkLength - 1] = '\0';

    /* get rid of the newline */
    {
      char *pNewline = strchr(link, '\n');
      if (pNewline != NULL) {
        *pNewline = '\0';
      }
    }
  }

  pclose(pipe);

  if (numLinks > 0) {
    return 1;
  } else {
    return 0;
  }
}
