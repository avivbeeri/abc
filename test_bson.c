#include <stdio.h>

#define ABC_BSON_IMPL
#include "ABC_bson.h"

// Example program

int main(void) {

  ABC_BSON_DOC doc;
  ABC_BSON_DOC_init(&doc);

  // ABC_BSON_TYPE type = ABC_BSON_getType(&doc, "data.path");
  // printf("%s\n", ABC_BSON_printType(type));
  // based on the type, we can extract it.
  ABC_BSON_setDouble(&doc, "value.0", 42);
  ABC_BSON_setDouble(&doc, "value.1", 84);
  double d = ABC_BSON_getDouble(&doc, "value.0");
  printf("%f\n", d);
  d = ABC_BSON_getDouble(&doc, "value.1");
  printf("%f\n", d);
  // ABC_BSON_setDouble(&doc, "data.path", (double) 5);
  // ABC_BSON_TYPE type = ABC_BSON_readString(file, "data.path");

  return 0;
}

