/*
  ABC_bson.h - v0.0.1 - Public Domain
  Author: Aviv Beeri, 2018

  How To Use:


  #define ABC_BSON_IMPL
  #include "ABC_bson.h"

  Version History:

  v0.0.1 - Initial Release, be careful

  License:

  This is free and unencumbered software released into the public domain.

  Anyone is free to copy, modify, publish, use, compile, sell, or
  distribute this software, either in source code form or as a compiled
  binary, for any purpose, commercial or non-commercial, and by any
  means.

  In jurisdictions that recognize copyright laws, the author or authors
  of this software dedicate any and all copyright interest in the
  software to the public domain. We make this dedication for the benefit
  of the public at large and to the detriment of our heirs and
  successors. We intend this dedication to be an overt act of
  relinquishment in perpetuity of all present and future rights to this
  software under copyright law.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
  OTHER DEALINGS IN THE SOFTWARE.

  For more information, please refer to <http://unlicense.org/>
*/

#ifndef ABC_BSON_H
#define ABC_BSON_H



// Public API:
// utility methods
//  ABC_BSON_TYPE ABC_BSON_getType(ABC_BSON_DOC* doc, char* path);

// Will deep-free the memory
// void ABC_BSON_DOC_free(ABC_BSON_DOC *doc);

// builders


// getters
// ABC_BSON_DOC* ABC_BSON_getDoc(ABC_BSON_DOC* doc, char* path);
// double ABC_BSON_getDouble(ABC_BSON_DOC* doc, char* path);
// char* ABC_BSON_getBoolean(ABC_BSON_DOC* doc, char* path);
// char* ABC_BSON_getString(ABC_BSON_DOC* doc, char* path);
// ABC_INT32 ABC_BSON_getInt32(ABC_BSON_DOC* doc, char* path);
// ABC_INT64 ABC_BSON_getInt64(ABC_BSON_DOC* doc, char* path);
// ABC_UINT64 ABC_BSON_getUint64(ABC_BSON_DOC* doc, char* path);
// ABC_BYTE* ABC_BSON_getBytes(ABC_BSON_DOC* doc, char* path);

// setters



#endif // ABC_BSON_H


#ifdef ABC_BSON_IMPL
#define ABC_BSON_IMPL

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
typedef int8_t ABC_BYTE;
typedef int32_t ABC_INT32;
typedef int64_t ABC_INT64;
typedef uint64_t ABC_UINT64;

// Initial configuration

typedef enum {
  ABC_BSON_TYPE_NONE = 0x00,
  ABC_BSON_TYPE_DOUBLE = 0x01,
  ABC_BSON_TYPE_STRING = 0x02,
  ABC_BSON_TYPE_DOCUMENT = 0x03,
  ABC_BSON_TYPE_ARRAY = 0x04,
  ABC_BSON_TYPE_BINARY = 0x05,
  ABC_BSON_TYPE_UNDEFINED = 0x06, // DEPRECIATED

  ABC_BSON_TYPE_OBJECT_ID = 0x07,
  ABC_BSON_TYPE_BOOLEAN = 0x08,
  ABC_BSON_TYPE_DATETIME = 0x09,
  ABC_BSON_TYPE_NULL = 0x0A,
  ABC_BSON_TYPE_REGEX = 0x0B,
  ABC_BSON_TYPE_DB_POINTER = 0x0C, //DEPRECIATED
  ABC_BSON_TYPE_JS_CODE = 0x0D,
  ABC_BSON_TYPE_SYMBOL = 0x0E, // DEPRECIATED
  ABC_BSON_TYPE_JS_CODE_WITH_SCOPE = 0x0F,
  ABC_BSON_TYPE_INT_32 = 0x10,
  ABC_BSON_TYPE_TIMESTAMP = 0x11,
  ABC_BSON_TYPE_INT_64 = 0x12,
  ABC_BSON_TYPE_DECIMAL_128 = 0x13, // We can't support this

  ABC_BSON_TYPE_MIN_KEY = 0xFF,
  ABC_BSON_TYPE_MAX_KEY = 0x7F
} ABC_BSON_TYPE;

typedef enum {
  ABC_BSON_SUBTYPE_GENERIC = 0x00,
  ABC_BSON_SUBTYPE_FUNCTION = 0x01,
  ABC_BSON_SUBTYPE_BINARY_OLD = 0x02, // OLD
  ABC_BSON_SUBTYPE_UUID_OLD = 0x03, // OLD
  ABC_BSON_SUBTYPE_UUID = 0x04,
  ABC_BSON_SUBTYPE_MD5 = 0x05,
  ABC_BSON_SUBTYPE_ENCRYPTED = 0x06,
  ABC_BSON_SUBTYPE_USER_DEFINED = 0x80
} ABC_BSON_SUBTYPE;


// Internal BSON elements
// For easy manipulation, we operate on these as distinct chunks
// rather than as compact binary data.
struct ABC_BSON_DOC_t;
typedef struct ABC_BSON_ELEMENT_t {
  ABC_BSON_TYPE elementType;
  size_t size;
  char* elementName;
  struct ABC_BSON_DOC_t* doc;
  struct ABC_BSON_ELEMENT_t* prev;
  struct ABC_BSON_ELEMENT_t* next;
  void* data;
  char space[];
} ABC_BSON_ELEMENT;

typedef struct ABC_BSON_DOC_t {
  ABC_BSON_ELEMENT* head;
} ABC_BSON_DOC;

typedef enum {
  ABC_BSON_RESULT_SUCCESS,
  ABC_BSON_RESULT_FAILURE
} ABC_BSON_RESULT;


bool isnumber (const char *c) {
  while (*c) {
  	if (*c<'0' || *c>'9') return false;
   	c++;
  }
  return true;
}

void ABC_BSON_DOC_init(ABC_BSON_DOC* doc) {
  doc->head = NULL;
}

/*
ABC_BSON_RESULT ABC_BSON_DOC_init(ABC_BSON_DOC* doc) {
  ABC_BSON_ELEMENT* el = malloc(sizeof(ABC_BSON_ELEMENT));
  doc->head = ;
  return ;
}
*/

ABC_BSON_ELEMENT* ABC_BSON_ELEMENT_init(ABC_BSON_DOC* doc, char* segment) {
    ABC_BSON_ELEMENT* element = (ABC_BSON_ELEMENT*)malloc(sizeof(ABC_BSON_ELEMENT) + (strlen(segment)+1));
    element->next = NULL;
    element->prev = NULL;
    element->doc = doc;
    element->elementName = element->space;
    strcpy(element->elementName, segment);
    element->data = NULL;
    element->elementType = ABC_BSON_TYPE_UNDEFINED;
    return element;
}

void* ABC_BSON_ELEMENT_realloc(ABC_BSON_ELEMENT* element, size_t nameSize, size_t dataSize) {
  ABC_BSON_ELEMENT* newPtr = realloc(element, sizeof(ABC_BSON_ELEMENT) + nameSize + dataSize);
  newPtr->data = newPtr->space + nameSize;
  if (newPtr->prev != NULL) {
    newPtr->prev->next = newPtr;
  } else {
    newPtr->doc->head = newPtr;
  }
  return newPtr;
}

ABC_BSON_ELEMENT* ABC_BSON_get(ABC_BSON_DOC* doc, char* path) {
  printf("Getting: %s\n", path);
  ABC_BSON_ELEMENT* prev = NULL;
  ABC_BSON_ELEMENT* node = doc->head;
  char* delim = strchr(path, '.');
  char* remainder = NULL;
  char* segment = path;
  bool needFree = false;
  bool found = false;
  if (delim != NULL) {
    size_t size = delim - path;
    segment = (char*)malloc(size + 1);
    // This might not be safe
    // But we need a mutable copy to split the path
    strncpy(segment, path, size);
    segment[size] = '\0';
    remainder = delim + 1;
    needFree = true;
  }

  do {
    if (node == NULL) {
      node = ABC_BSON_ELEMENT_init(doc, segment);
      doc->head = node;
    }

    if (strcmp(segment, node->elementName) == 0) {
      // This key matches the element
      if (remainder != NULL) {
        // We want to go another layer down.
        if (node->elementType == ABC_BSON_TYPE_UNDEFINED) {
          node->elementType = isnumber(remainder) ? ABC_BSON_TYPE_ARRAY : ABC_BSON_TYPE_DOCUMENT;
          node = ABC_BSON_ELEMENT_realloc(node, strlen(segment)+1, sizeof(ABC_BSON_DOC));
        }
        if (node->elementType == ABC_BSON_TYPE_DOCUMENT ||
            node->elementType == ABC_BSON_TYPE_ARRAY) {
          printf("Descending into %s\n", remainder);
          return ABC_BSON_get((ABC_BSON_DOC*)(node->data), remainder);
        } else {
          // We want to go deeper here, but the node isn't a document or array
          assert(false);
        }
      } else {
        printf("found %s\n", segment);
        found = true;
        break;
      }
    }
    // next token of path
    prev = node;
    node = node->next;
    // compare the name of the element to the segment.
  } while (found == false && node != NULL);

  if (found == false) {
    ABC_BSON_ELEMENT* element = ABC_BSON_ELEMENT_init(doc, segment);
    element->prev = prev;
    if (prev != NULL) {
      prev->next = element;
    } else {
      doc->head = element;
    }
    node = element;
  }

  if (needFree) {
    free(segment);
  }
  return node;
}

ABC_BSON_TYPE ABC_BSON_getType(ABC_BSON_DOC* doc, char* path) {
  ABC_BSON_ELEMENT* element = ABC_BSON_get(doc, path);
  if (element == NULL) {
    return ABC_BSON_TYPE_UNDEFINED;
  }
  return element->elementType;
}


void ABC_BSON_setDouble(ABC_BSON_DOC* doc, char* path, double value) {
  ABC_BSON_ELEMENT* element = ABC_BSON_get(doc, path);
  if (element->elementType == ABC_BSON_TYPE_UNDEFINED) {
    // TODO: This doesn't work for nested documents
    size_t len = strlen(path) + 1;
    element = ABC_BSON_ELEMENT_realloc(element, len, 8);
    element->elementType = ABC_BSON_TYPE_DOUBLE;
  } else if (element->elementType != ABC_BSON_TYPE_DOUBLE) {
    // TODO: reallocate size of element
    element->elementType = ABC_BSON_TYPE_DOUBLE;
  }
  *(double*)(element->data) = value;
}

char* ABC_BSON_printType(ABC_BSON_TYPE type) {
  switch (type) {
    default: return "undefined";
  }
}

double ABC_BSON_getDouble(ABC_BSON_DOC* doc, char* path) {
  ABC_BSON_ELEMENT* element = ABC_BSON_get(doc, path);
  assert(element != NULL);
  assert(element->elementType == ABC_BSON_TYPE_DOUBLE);
  return *(double*)(element->data);
}

#endif // ABC_BSON_IMPL
