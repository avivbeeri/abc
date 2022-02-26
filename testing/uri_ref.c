#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/**
  uri-ref.c - v1.0.0 - GFDL License
  RFC3986 compliant URI parser

  Author: Aviv Beeri, 2022

  License:

  Copyright (c) 2022 Aviv Beeri
  Permission is granted to copy, distribute and/or modify this document
  under the terms of the GNU Free Documentation License, Version 1.2
  or any later version published by the Free Software Foundation;
  with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
  Texts.  A copy of the license is included in the section entitled "GNU
  Free Documentation License".

  A copy of GFDL 2.0 can be found in the source repository, or
  here: https://www.gnu.org/licenses/old-licenses/fdl-1.2.html

*/


/* Raw struct - don't use this directly */
typedef struct ABC_URI_t {
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

} ABC_URI;
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

  int index = 0;
  int strIndex = 0;

  index = ABC_indexOf(uri, "//");
  if (index >= 0 && ABC_countOf(uri, ":", index) == 1) {
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
    if (ABC_countOf(result->authority, "@", result->authorityLen) > 0) {
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
    } else if (ABC_countOf(result->authority, ":", result->authorityLen) > 0 &&
        ABC_countOf(result->authority, "[", result->authorityLen) == 0 &&
        ABC_countOf(result->authority, "]", result->authorityLen) == 0) {
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
    } else if (ABC_countOf(result->authority, "[", result->authorityLen) > 0 &&
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

  int queryIndex = ABC_indexOfN(result->path, "?", result->pathLen);
  int fragmentIndex = ABC_indexOfN(result->path, "#", result->pathLen);
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

  if (result->scheme != NULL) {
    result->schemeIndex = result->scheme - result->uri;
  }
  if (result->authority != NULL) {
    result->authorityIndex = result->authority - result->uri;
  }
  if (result->cred != NULL) {
    result->credIndex = result->cred - result->uri;
  }
  if (result->user != NULL) {
    result->userIndex = result->user - result->uri;
  }
  if (result->pass != NULL) {
    result->passIndex = result->pass - result->uri;
  }
  if (result->host != NULL) {
    result->hostIndex = result->host - result->uri;
  }
  if (result->path != NULL) {
    result->pathIndex = result->path - result->uri;
  }
  if (result->query != NULL) {
    result->queryIndex = result->query - result->uri;
  }
  if (result->fragment != NULL) {
    result->fragmentIndex = result->fragment - result->uri;
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

int main(int argc, char** argv) {
  ABC_URI result;
  if (argc > 1) {
    ABC_URI_parse(&result, argv[1]);
    ABC_URI_print(&result);
  } else {
    ABC_URI_parse(&result, "gemini://test:8000");
    ABC_URI_print(&result);
    ABC_URI_parse(&result, "foo://example.com:8042/over/there?name=ferret#nose");
    ABC_URI_print(&result);
    ABC_URI_parse(&result, "urn:example:animal:ferret:nose");
    ABC_URI_print(&result);
    ABC_URI_parse(&result, "jdbc:mysql://test_user:ouupppssss@localhost:3306/sakila?profileSQL=true");
    ABC_URI_print(&result);
    ABC_URI_parse(&result, "ftp://ftp.is.co.za/rfc/rfc1808.txt");
    ABC_URI_print(&result);
    ABC_URI_parse(&result, "http://www.ietf.org/rfc/rfc2396.txt#header1");
    ABC_URI_print(&result);
    ABC_URI_parse(&result, "ldap://[2001:db8::7]/c=GB?objectClass=one&objectClass=two");
    ABC_URI_print(&result);
    ABC_URI_parse(&result, "mailto:John.Doe@example.com");
    ABC_URI_print(&result);
    ABC_URI_parse(&result, "news:comp.infosystems.www.servers.unix");
    ABC_URI_print(&result);
    ABC_URI_parse(&result, "tel:+1-816-555-1212");
    ABC_URI_print(&result);
    ABC_URI_parse(&result, "telnet://192.0.2.16:80/");
    ABC_URI_print(&result);
    ABC_URI_parse(&result, "urn:oasis:names:specification:docbook:dtd:xml:4.1.2");
    ABC_URI_print(&result);
    ABC_URI_parse(&result, "ssh://alice@example.com");
    ABC_URI_print(&result);
    ABC_URI_parse(&result, "https://bob:pass@example.com/place");
    ABC_URI_print(&result);
    ABC_URI_parse(&result, "http://example.com/?a=1&b=2+2&c=3&c=4&d=%%65%%6e%%63%%6F%%64%%65%%64");
    ABC_URI_print(&result);

    size_t len = 7;
    char buf[len];
    ABC_URI_parse(&result, "https://bob:pass@example.com/place?query&test=home#fragments");

    size_t copySize = ABC_URI_get_uri(&result, NULL, 0);
    char* newBuf = malloc(copySize);
    if (ABC_URI_get_uri(&result, newBuf, copySize) > 0) {
      printf("%s\n", newBuf);
    }

    if (ABC_URI_getstr_scheme(&result, newBuf, buf, len) > 0) {
      printf("%s\n", buf);
    }

    free(newBuf);
    if (ABC_URI_get_user(&result, buf, len) > 0) {
      printf("%s\n", buf);
    }
    if (ABC_URI_get_pass(&result, buf, len) > 0) {
      printf("%s\n", buf);
    }
    if (ABC_URI_get_host(&result, buf, len) > 0) {
      printf("%s\n", buf);
    }
    if (ABC_URI_get_path(&result, buf, len) > 0) {
      printf("%s\n", buf);
    }
    if (ABC_URI_get_query(&result, buf, len) > 0) {
      printf("%s\n", buf);
    }
    if (ABC_URI_get_fragment(&result, buf, len) > 0) {
      printf("%s\n", buf);
    }
  }
}
