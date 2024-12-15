#include "webSearch.h"

int main(int argc, char **argv) {
  if (argc != 4) {
    fprintf(stderr, "USAGE: %s URLFile maxNumPages randSeed\n", argv[0]);
    return -1;
  }

  char *urlPath = argv[1];
  const int MAX_N_PAGES = atoi(argv[2]);
  int seed = atoi(argv[3]);

  srand(seed);

  printf("Indexing...\n");

  PageNode *pageList = NULL;

  int nPagesRead = crawl(MAX_N_PAGES, &pageList, urlPath);
  if (nPagesRead) {
    freePageList(pageList);
  }

  printf("Exiting the program\n");
  return 0;
}
