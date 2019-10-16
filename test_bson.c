#include <stdio.h>

#define ABC_BSON_IMPL
#include "ABC_bson.h"

// Example program

int main(void) {

  // Create the queue
  ABC_BSON_DOCUMENT doc;

  ABC_BSON_TYPE type = ABC_BSON_getType(&doc, "data.path");
  printf("%i\n", type);
  // ABC_BSON_TYPE type = ABC_BSON_readString(file, "data.path");

  return 0;
}

