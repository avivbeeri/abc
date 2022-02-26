/**
  ABC_uri.c - v1.0.0 -
  RFC3986 compliant URI parser

  Author: Aviv Beeri, 2022

  How To Use:

  ---

  #define ABC_URI_IMPL
  #include "ABC_uri.h"

  ...

  ABC_URI result;
  if (ABC_URI_parse(&result, "https://github.com/avivbeeri/abc")) {
    ABC_URI_print(&result);
  } else {
    // URI is invalid
  }
  ---

  Version History:
  v1.0.0 - Complete parsing implementation

  License:

  Copyright (c) 2022 Aviv Beeri

*/

#include <stdio.h>
#include <string.h>

#ifndef ABC_URI_H
#define ABC_URI_H

/* Public API Declaration */
typedef struct ABC_URI_t ABC_URI;

/*
 * Parses a URI and stores the results in a struct for later retrieval.
 *
 * result - a pointer to the ABC_URI structure.
 * uri    - the uri you want to decode. This must exist in location for the lifetime of result.
 * returns 1 if the URI is valid, otherwise returns 0.
 */
int ABC_URI_parse(ABC_URI* result, char* uri);

/*
 * Prints out the URI and its available components.
 */
void ABC_URI_print(ABC_URI* uri);

/*
 * These fetch a specific componet of the URI, if it exists, and writes it to "buf".
 * the length is written out to "len".
 *
 *
 *
 * returns:
 * -1 if the URI is invalid.
 * 0 if the URI does not have this field.
 * If "buf" and "len" are NULL, it returns the length required to store the component string.
 * otherwise returns 1.
 */
int ABC_URI_get_uri(ABC_URI* uri, char* buf, int len);
int ABC_URI_get_scheme(ABC_URI* uri, char* buf, int len);
int ABC_URI_get_user(ABC_URI* uri, char* buf, int len);
int ABC_URI_get_pass(ABC_URI* uri, char* buf, int len);
int ABC_URI_get_host(ABC_URI* uri, char* buf, int len);
int ABC_URI_get_path(ABC_URI* uri, char* buf, int len);
int ABC_URI_get_query(ABC_URI* uri, char* buf, int len);
int ABC_URI_get_fragment(ABC_URI* uri, char* buf, int len);

// If the original text changed/moved, you can pass it in here
int ABC_URI_getstr_uri(ABC_URI* uri, char* str, char* buf, int len);
int ABC_URI_getstr_scheme(ABC_URI* uri, char* str, char* buf, int len);
int ABC_URI_getstr_user(ABC_URI* uri, char* str, char* buf, int len);
int ABC_URI_getstr_pass(ABC_URI* uri, char* str, char* buf, int len);
int ABC_URI_getstr_host(ABC_URI* uri, char* str, char* buf, int len);
int ABC_URI_getstr_path(ABC_URI* uri, char* str, char* buf, int len);
int ABC_URI_getstr_query(ABC_URI* uri, char* str, char* buf, int len);
int ABC_URI_getstr_fragment(ABC_URI* uri, char* str, char* buf, int len);


/* Raw struct - don't use this directly */
struct ABC_URI_t {
  int initialized;
  int valid;

  char* uri;
  int uriIndex;
  int uriLen;

  char* scheme;
  int schemeIndex;
  int schemeLen;

  char* authority;
  int authorityIndex;
  int authorityLen;

  char* cred;
  int credIndex;
  int credLen;

  char* user;
  int userIndex;
  int userLen;

  char* pass;
  int passIndex;
  int passLen;

  char* host;
  int hostIndex;
  int hostLen;

  char* port;
  int portIndex;
  int portLen;

  char* path;
  int pathIndex;
  int pathLen;

  char* query;
  int queryIndex;
  int queryLen;

  char* fragment;
  int fragmentIndex;
  int fragmentLen;

};

#ifdef ABC_URI_IMPLEMENTATION

#define ABC_true 1
#define ABC_false 0

void ABC_URI_print(ABC_URI* uri) {
  printf("URI       = %s\n", uri->uri);
  if (uri->scheme != NULL) {
    printf("scheme    = %.*s\n", uri->schemeLen, uri->scheme);
  }
  /*
  if (uri->authority != NULL) {
    printf("authority = %.*s\n", uri->authorityLen, uri->authority);
  }
  if (uri->cred != NULL) {
    printf("cred      = %.*s\n", uri->credLen, uri->cred);
  }
  */
  if (uri->user != NULL) {
    printf("user      = %.*s\n", uri->userLen, uri->user);
  }
  if (uri->pass != NULL) {
    printf("pass      = %.*s\n", uri->passLen, uri->pass);
  }
  if (uri->host != NULL) {
    printf("host      = %.*s\n", uri->hostLen, uri->host);
  }
  if (uri->port != NULL) {
    printf("port      = %.*s\n", uri->portLen, uri->port);
  }

  if (uri->path != NULL) {
    printf("path      = %.*s\n", uri->pathLen, uri->path);
  }
  if (uri->query != NULL) {
    printf("query     = %.*s\n", uri->queryLen, uri->query);
  }
  if (uri->fragment != NULL) {
    printf("fragment  = %.*s\n", uri->fragmentLen, uri->fragment);
  }
  if (!uri->valid) {
    printf("URI is invalid.\n");
  }
  printf("\n");
}

int ABC_min(int a, int b) {
  if (a == -1 && b == -1) {
    return -1;
  }
  if (b == -1) {
    return a;
  }
  if (a == -1) {
    return b;
  }
  return (a < b) ? a : b;
}

int ABC_indexOf(char* str, char* substr) {
  char* c = strstr(str, substr);
  if (c == NULL) {
    return -1;
  }
  return c - str;
}
int ABC_indexOfN(char* str, char* substr, int len) {
  char* c = strstr(str, substr);
  if (c == NULL) {
    return -1;
  }
  if ((long int)(c - str) > len) {
    return -1;
  }
  return c - str;
}

int ABC_countOf(char* url, char* str, int len) {
  int index = 0;
  int count = 0;
  while (index < len) {
    if (url[index] == '\0') {
      return -1;
    }
    if (url[index] == str[0]) {
      count++;
    }
    index++;
  }
  return count;
}

int ABC_URI_parse(ABC_URI* result, char* uri) {
  memset(result, 0, sizeof(ABC_URI));
  char* str = uri;
  result->uri = uri;
  result->uriLen = strlen(uri);
  result->valid = ABC_true;

  int index = 0;

  index = ABC_indexOf(str, ":");
  if (index != -1) {
    result->scheme = uri;
    result->schemeLen = index;
    result->schemeIndex = 0;

    str = result->scheme + index + 1;

    // is the authority present? '//'
    if (str[0] == '/' && str[1] == '/') {
      str += 2;
      result->authority = str;
      result->authorityIndex = str - uri;
      int slashIndex = ABC_indexOf(str, "/");
      int queryIndex = ABC_indexOf(str, "?");
      int fragmentIndex = ABC_indexOf(str, "#");

      index = ABC_min(ABC_min(slashIndex, queryIndex), fragmentIndex);
      if (index != -1) {
        str += index;
        if (index == slashIndex) {
          result->authorityLen = slashIndex;
          int end = 0;
          if (queryIndex != -1 && ABC_min(queryIndex, fragmentIndex) == queryIndex) {
            end = queryIndex - slashIndex;
          } else if (fragmentIndex != 1) {
            end = fragmentIndex - slashIndex;
          } else {
            end = result->uriLen - (result->authorityIndex + result->authorityLen);
          }

          if (slashIndex > 0) {
            result->path = str;
            result->pathIndex = str - uri;
            result->pathLen = end;
            str = result->path + end;
          }
        } else {
          result->authorityLen = index;
        }

        if (queryIndex != -1 || fragmentIndex != -1) {
          str++;
          if (queryIndex != -1 && ABC_min(queryIndex, fragmentIndex) == queryIndex) {
            result->query = str;
            result->queryIndex = str - uri;
            index = ABC_indexOf(str, "#");
            if (index != -1) {
              result->queryLen = index;
              str += index + 1;
            } else {
              result->queryLen = result->uriLen - result->queryIndex;
            }
          }
          if (fragmentIndex != -1) {
            result->fragment = str;
            result->fragmentIndex = str - uri;
            result->fragmentLen = result->uriLen - result->fragmentIndex;
          }
        }
      } else {
        result->authorityLen = result->uriLen - result->authorityIndex;
      }
    } else {
      // Otherwise everything from here is the path
      result->path = str;
      result->pathIndex = str - uri;

      int queryIndex = ABC_indexOf(str, "?");
      int fragmentIndex = ABC_indexOf(str, "#");
      int end;
      if (queryIndex != -1 && ABC_min(queryIndex, fragmentIndex) == queryIndex) {
        end = queryIndex;
      } else if (fragmentIndex != 1) {
        end = fragmentIndex;
      } else {
        end = result->uriLen - (result->pathIndex);
      }
      result->pathLen = end;

      if (queryIndex != -1 || fragmentIndex != -1) {
        str += end + 1;
        if (queryIndex != -1 && ABC_min(queryIndex, fragmentIndex) == queryIndex) {
          result->query = str;
          result->queryIndex = str - uri;
          index = ABC_indexOf(str, "#");
          if (index != -1) {
            result->queryLen = index;
            str += index + 1;
          } else {
            result->queryLen = result->uriLen - result->queryIndex;
          }
        }
        if (fragmentIndex != -1) {
          result->fragment = str;
          result->fragmentIndex = str - uri;
          result->fragmentLen = result->uriLen - result->fragmentIndex;
        }
      }
    }
  } else {
    result->authority = uri;
    result->authorityIndex = 0;
    result->authorityLen = 0;
  }

  if (result->authority != NULL) {
    // authority = [ userinfo "@" ] host [ ":" port ]
    str = result->authority;
    int len = result->authorityLen;
    index = ABC_indexOfN(str, "@", len);
    if (index != -1) {
      // process user info
      result->cred = str;
      result->credLen = index;
      result->credIndex = result->authorityIndex;
      // adjust
      str += index + 1;
      len -= index + 1;

      // extract credentials
      result->user = result->cred;
      result->userIndex = result->credIndex;
      index = ABC_indexOfN(result->user, ":", result->credLen);
      if (index != -1) {
        result->userLen = index;
        result->pass = result->user + index + 1;
        result->passIndex = result->pass - uri;
        result->passLen = result->credLen - (index + 1);
      } else {
        result->userLen = result->credLen;
      }
    }

    // Disambiguate between the host, port and ipv6+ hosts.
    result->host = str;
    result->hostIndex = str - uri;
    int hostStartIndex = ABC_indexOfN(str, "[", len);
    int hostEndIndex = ABC_indexOfN(str, "]", len);
    if (hostStartIndex != -1 && hostEndIndex != -1) {
      // str += hostEndIndex - 1;
    } else if (hostStartIndex != hostEndIndex) {
      result->valid = ABC_false;
    } else {
      hostEndIndex = 0;
    }

    index = ABC_indexOfN(str + hostEndIndex, ":", len - hostEndIndex);
    if (index != -1) {
      index += hostEndIndex;
      result->hostLen = index;

      str += index + 1;
      result->port = str;
      result->portLen = len - index - 1;
      result->portIndex = str - uri;
      // port
    } else {
      result->hostLen = len;
    }
  }

  return result->valid;
}

#define ABC_URI_GETTER(field) \
int ABC_URI_get_##field(ABC_URI* uri, char* buf, int len) { \
  if (uri->initialized != 1) { \
    return -1; \
  } \
  if (uri->field != NULL) { \
    if (buf == NULL) { \
      return uri->field##Len + 1; \
    } else {  \
      int copyLen = (len - 1) > uri->field##Len ? uri->field##Len : len - 1; \
      memcpy(buf, uri->uri + uri->field##Index, copyLen); \
      buf[copyLen] = '\0'; \
      memset(buf + copyLen, 0, (len - copyLen) > 0 ? len - copyLen : 1); \
      return 1; \
    } \
  } \
  return 0; \
} \
\
int ABC_URI_getstr_##field(ABC_URI* uri, char* str, char* buf, int len) { \
  if (uri->initialized != 1) { \
    return -1; \
  } \
  if (uri->field != NULL) { \
    if (buf == NULL) { \
      return uri->field##Len + 1; \
    } else {  \
      int copyLen = (len - 1) > uri->field##Len ? uri->field##Len : len - 1; \
      memcpy(buf, str + uri->field##Index, copyLen); \
      buf[copyLen] = '\0'; \
      memset(buf + copyLen, 0, (len - copyLen) > 0 ? len - copyLen : 1); \
      return 1; \
    } \
  } \
  return 0; \
}

ABC_URI_GETTER(uri)
ABC_URI_GETTER(scheme)
ABC_URI_GETTER(pass)
ABC_URI_GETTER(user)
ABC_URI_GETTER(host)
ABC_URI_GETTER(path)
ABC_URI_GETTER(query)
ABC_URI_GETTER(fragment)

#undef ABC_URI_GETTER
#undef ABC_true
#undef ABC_false

#endif

#endif


