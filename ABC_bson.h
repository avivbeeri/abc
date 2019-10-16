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

#include <stdint.h>
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

typedef struct {
  ABC_BSON_TYPE elementType;
  union {

  } as;
} ABC_BSON_ELEMENT;

typedef struct {
  ABC_INT32 size; // Document size in bytes
  ABC_BSON_ELEMENT* element;
} ABC_BSON_DOCUMENT;

// Public API:
ABC_BSON_TYPE ABC_BSON_getType(ABC_BSON_DOCUMENT* doc, char* path);

#endif // ABC_BSON_H


#ifdef ABC_BSON_IMPL
#define ABC_BSON_IMPL

// Initialise the BSON parser
ABC_BSON_TYPE ABC_BSON_getType(ABC_BSON_DOCUMENT* doc, char* path) {
  // memset(queue, 0, sizeof(ABC_BSON_PARSER));
  return ABC_BSON_TYPE_UNDEFINED;
}

#endif // ABC_BSON_IMPL
