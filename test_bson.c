#include <stdio.h>

#define ABC_BSON_IMPL
#include "ABC_bson.h"

void printType(ABC_BSON_DOC* doc, char* path) {
  ABC_BSON_TYPE type = ABC_BSON_getType(doc, path);
  printf("%s: %s\n", path, ABC_BSON_printType(type));
}
// Example program

int main(void) {

  ABC_BSON_DOC doc;
  ABC_BSON_DOC_init(&doc);
  ABC_BSON_setDouble(&doc, "value.0", 42);

  // based on the type, we can extract it.

  ABC_BSON_TYPE type = ABC_BSON_getType(&doc, "value");
  printf("type: %s\n", ABC_BSON_printType(type));

  ABC_BSON_setBoolean(&doc, "value.1", false);
  double d = ABC_BSON_getDouble(&doc, "value.0");
  printf("value.0: %f\n", d);
  bool b = ABC_BSON_getBoolean(&doc, "value.1");
  printf("value.1: %s\n", b ? "true" : "false");
  ABC_BSON_setNull(&doc, "value.2");
  printType(&doc, "value");
  printType(&doc, "value.0");
  printType(&doc, "value.1");
  printType(&doc, "value.2");
  ABC_BSON_setString(&doc, "value", "Aviv");
  printf("value: %s\n", ABC_BSON_getString(&doc, "value"));
  printType(&doc, "value");
  printType(&doc, "value.aviv");

  // ABC_BSON_setDouble(&doc, "data.path", (double) 5);
  // ABC_BSON_TYPE type = ABC_BSON_readString(file, "data.path");

  ABC_BSON_DOC_free(&doc);

  return 0;
}

