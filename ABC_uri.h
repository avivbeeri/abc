/**
  ABC_uri.c - v0.0.1
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
  v0.0.1 - Complete parsing implementation

  License:


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
int ABC_URI_get_uri(ABC_URI* uri, char* buf, size_t len);
int ABC_URI_get_scheme(ABC_URI* uri, char* buf, size_t len);
int ABC_URI_get_user(ABC_URI* uri, char* buf, size_t len);
int ABC_URI_get_pass(ABC_URI* uri, char* buf, size_t len);
int ABC_URI_get_host(ABC_URI* uri, char* buf, size_t len);
int ABC_URI_get_path(ABC_URI* uri, char* buf, size_t len);
int ABC_URI_get_query(ABC_URI* uri, char* buf, size_t len);
int ABC_URI_get_fragment(ABC_URI* uri, char* buf, size_t len);

/* Raw struct - don't use this */
struct ABC_URI_t {
  int initialized;
  int valid;

  char* uri;
  size_t uriLen;

  char* scheme;
  size_t schemeLen;

  char* authority;
  size_t authorityLen;

  char* cred;
  size_t credLen;

  char* user;
  size_t userLen;

  char* pass;
  size_t passLen;

  char* host;
  size_t hostLen;

  char* port;
  size_t portLen;

  char* path;
  size_t pathLen;

  char* query;
  size_t queryLen;

  char* fragment;
  size_t fragmentLen;

};

#ifdef ABC_URI_IMPLEMENTATION

void ABC_URI_print(ABC_URI* uri) {
  printf("URI       = %s\n", uri->uri);
  if (uri->scheme != NULL) {
    printf("scheme    = %.*s\n", uri->schemeLen, uri->scheme);
  }
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

size_t ABC_indexOf(char* str, char* substr) {
  char* c = strstr(str, substr);
  if (c == NULL) {
    return -1;
  }
  return c - str;
}
size_t ABC_indexOfN(char* str, char* substr, size_t len) {
  char* c = strstr(str, substr);
  if (c == NULL) {
    return -1;
  }
  if (c - str > len) {
    return -1;
  }
  return c - str;
}

size_t countOf(char* url, char* str, size_t len) {
  size_t index = 0;
  size_t count = 0;
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

  size_t index = 0;
  size_t strIndex = 0;

  index = ABC_indexOf(uri, "//");
  if (index >= 0 && countOf(uri, ":", index) == 1) {
    result->scheme = uri;
    result->schemeLen = index - 1;
    strIndex = index + 2;

    str = uri + strIndex;
    // parse the domain
    index = ABC_indexOf(str, "/");
    result->authority = str;
    if (index == -1) {
      result->authorityLen = result->uriLen - strIndex;
    } else {
      result->authorityLen = index;
    }

    // check for credentials and port
    if (countOf(result->authority, "@", result->authorityLen) > 0) {
      result->cred = result->authority;
      index = ABC_indexOf(str, "@");
      if (index == -1) {
        result->credLen = result->uriLen - strIndex;
      } else {
        result->credLen = index;
      }
      result->user = result->cred;
      result->userLen = result->credLen;
      index = ABC_indexOfN(result->user, ".", result->credLen);
      if (index != -1) {
        result->userLen = index;
        result->pass = result->user + index + 1;
        result->passLen = result->credLen - (index + 1);
      } else {
        index = ABC_indexOfN(result->user, ":", result->credLen);
        if (index != -1) {
          result->userLen = index;
          result->pass = result->user + index + 1;
          result->passLen = result->credLen - (index + 1);
        }
      }
      str = result->cred + result->credLen + 1;
      index = ABC_indexOf(str, "/");
      result->host = str;
      if (index == -1) {
        result->hostLen = result->authorityLen - result->credLen;
      } else {
        result->hostLen = index;
      }
      index = ABC_indexOfN(str, ":", result->hostLen);
      if (index != -1) {
        result->hostLen = index;
      }
    } else if (countOf(result->authority, ":", result->authorityLen) > 0 &&
        countOf(result->authority, "[", result->authorityLen) == 0 &&
        countOf(result->authority, "]", result->authorityLen) == 0) {
      str = result->authority;
      index = ABC_indexOfN(str, ":", result->authorityLen);
      if (index != -1) {
        result->host = str;
        result->hostLen = index;
        result->port = result->host + (index + 1);
        result->portLen = result->authorityLen -  result->hostLen - 1;
      } else {
        result->host = str;
        result->hostLen = result->authorityLen;
      }
    } else if (countOf(result->authority, "[", result->authorityLen) > 0 &&
        ABC_indexOfN(result->authority, "]:", result->authorityLen) != -1) {
      str = result->authority;
      index = ABC_indexOfN(str, "]", result->authorityLen);
      result->host = str + 1;
      result->hostLen = index;
      result->port = str + index + 1;
      result->portLen = result->authorityLen - (index + 1);
      // ipV6 with host
    } else {
      // no credentials
      result->host = result->authority;
      result->hostLen = result->authorityLen;
    }
    str = result->authority + result->authorityLen;
  } else {
    index = ABC_indexOf(uri, ":");
    str = uri;
    if (index != -1) {
      result->scheme = uri;
      result->schemeLen = index;
      str = result->scheme + index + 1;
    }
  }
  // parse the path;
  result->path = str;
  result->pathLen = result->uriLen - (str - uri);

  size_t queryIndex = ABC_indexOfN(result->path, "?", result->pathLen);
  size_t fragmentIndex = ABC_indexOfN(result->path, "#", result->pathLen);
    if (queryIndex != -1 && fragmentIndex == -1) {

    result->query = result->path + queryIndex + 1;
    result->queryLen = result->pathLen - queryIndex - 1;

    result->pathLen = queryIndex;
  } else if (queryIndex == -1 && fragmentIndex != -1) {

    result->fragment = result->path + fragmentIndex + 1;
    result->fragmentLen = result->pathLen - fragmentIndex - 1;

    result->pathLen = fragmentIndex;
  } else if (queryIndex != -1 &&
      fragmentIndex != -1 &&
      queryIndex < fragmentIndex) {

    result->query = result->path + queryIndex + 1;
    result->queryLen = fragmentIndex - queryIndex - 1;

    result->fragment = result->query + result->queryLen + 1;
    result->fragmentLen = result->pathLen - fragmentIndex - 1;

    result->pathLen = queryIndex;
  }

  result->initialized = 1;
  result->valid = (result->scheme != NULL && result->path != NULL);
  if (result->authority != NULL) {
    result->valid = (result->pathLen == 0 || result->path[0] == '/');
  } else {
    result->valid = !((result->path[0] == '/') && (result->path[1] == '/'));
  }
  return result->valid;
}

#define ABC_URI_GETTER(field) \
int ABC_URI_get_##field(ABC_URI* uri, char* buf, size_t len) { \
  if (uri->initialized != 1) { \
    return -1; \
  } \
  if (uri->field != NULL) { \
    if (buf == NULL) { \
      return uri->field##Len + 1; \
    } else {  \
      size_t copyLen = (len - 1) > uri->field##Len ? uri->field##Len : len - 1; \
      memcpy(buf, uri->field, copyLen); \
      buf[copyLen] = '\0'; \
      return 1; \
    } \
  } \
  return 0; \
}
    // memset(buf + copyLen, 0, (len - copyLen) > 0 ? len - uri->field##Len : 1); \

ABC_URI_GETTER(uri)
ABC_URI_GETTER(scheme)
ABC_URI_GETTER(pass)
ABC_URI_GETTER(user)
ABC_URI_GETTER(host)
ABC_URI_GETTER(path)
ABC_URI_GETTER(query)
ABC_URI_GETTER(fragment)

#undef ABC_URI_GETTER

#endif

#endif


