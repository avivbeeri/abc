/**
  * ABC_uri.c
  * version 0.0.1
  *
  * RFC3986 compliant URI parser
  */
#include <stdio.h>
#include <string.h>

#define ABC_URI_IMPLEMENTATION

/* Public API Declaration */
typedef struct ABC_URI_t ABC_URI;
void ABC_URI_print(ABC_URI* uri);
ABC_URI parse(char* uri);
int ABC_URI_scheme(ABC_URI* uri, char* buf, size_t* len);

/* Raw struct - don't use this */
struct ABC_URI_t {
  int initialized;
  int valid;

  char* uri;
  size_t len;

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

void printUri(ABC_URI* uri) {
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

ABC_URI parse(char* uri) {
  ABC_URI result;
  memset(&result, 0, sizeof(ABC_URI));
  char* str = uri;
  result.uri = uri;
  result.len = strlen(uri);

  size_t index = 0;
  size_t strIndex = 0;

  index = ABC_indexOf(uri, "//");
  if (index >= 0 && countOf(uri, ":", index) == 1) {
    result.scheme = uri;
    result.schemeLen = index - 1;
    strIndex = index + 2;

    str = uri + strIndex;
    // parse the domain
    index = ABC_indexOf(str, "/");
    result.authority = str;
    if (index == -1) {
      result.authorityLen = result.len - strIndex;
    } else {
      result.authorityLen = index;
    }

    // check for credentials and port
    if (countOf(result.authority, "@", result.authorityLen) > 0) {
      result.cred = result.authority;
      index = ABC_indexOf(str, "@");
      if (index == -1) {
        result.credLen = result.len - strIndex;
      } else {
        result.credLen = index;
      }
      result.user = result.cred;
      result.userLen = result.credLen;
      index = ABC_indexOfN(result.user, ".", result.credLen);
      if (index != -1) {
        result.userLen = index;
        result.pass = result.user + index + 1;
        result.passLen = result.credLen - (index + 1);
      } else {
        index = ABC_indexOfN(result.user, ":", result.credLen);
        if (index != -1) {
          result.userLen = index;
          result.pass = result.user + index + 1;
          result.passLen = result.credLen - (index + 1);
        }
      }
      str = result.cred + result.credLen + 1;
      index = ABC_indexOf(str, "/");
      result.host = str;
      if (index == -1) {
        result.hostLen = result.authorityLen - result.credLen;
      } else {
        result.hostLen = index;
      }
      index = ABC_indexOfN(str, ":", result.hostLen);
      if (index != -1) {
        result.hostLen = index;
      }
    } else if (countOf(result.authority, ":", result.authorityLen) > 0 &&
        countOf(result.authority, "[", result.authorityLen) == 0 &&
        countOf(result.authority, "]", result.authorityLen) == 0) {
      str = result.authority;
      index = ABC_indexOfN(str, ":", result.authorityLen);
      if (index != -1) {
        result.host = str;
        result.hostLen = index;
        result.port = result.host + (index + 1);
        result.portLen = result.authorityLen -  result.hostLen - 1;
      } else {
        result.host = str;
        result.hostLen = result.authorityLen;
      }
    } else if (countOf(result.authority, "[", result.authorityLen) > 0 &&
        ABC_indexOfN(result.authority, "]:", result.authorityLen) != -1) {
      str = result.authority;
      index = ABC_indexOfN(str, "]", result.authorityLen);
      result.host = str + 1;
      result.hostLen = index;
      result.port = str + index + 1;
      result.portLen = result.authorityLen - (index + 1);
      // ipV6 with host
    } else {
      // no credentials
      result.host = result.authority;
      result.hostLen = result.authorityLen;
    }
    str = result.authority + result.authorityLen;
  } else {
    index = ABC_indexOf(uri, ":");
    str = uri;
    if (index != -1) {
      result.scheme = uri;
      result.schemeLen = index;
      str = result.scheme + index + 1;
    }
  }
  // parse the path;
  result.path = str;
  result.pathLen = result.len - (str - uri);

  size_t queryIndex = ABC_indexOfN(result.path, "?", result.pathLen);
  size_t fragmentIndex = ABC_indexOfN(result.path, "#", result.pathLen);
    if (queryIndex != -1 && fragmentIndex == -1) {

    result.query = result.path + queryIndex + 1;
    result.queryLen = result.pathLen - queryIndex;

    result.pathLen = queryIndex;
  } else if (queryIndex == -1 && fragmentIndex != -1) {

    result.fragment = result.path + fragmentIndex + 1;
    result.fragmentLen = result.pathLen - fragmentIndex;

    result.pathLen = fragmentIndex;
  } else if (queryIndex != -1 &&
      fragmentIndex != -1 &&
      queryIndex < fragmentIndex) {

    result.query = result.path + queryIndex + 1;
    result.queryLen = fragmentIndex - queryIndex - 1;

    result.fragment = result.query + result.queryLen + 1;
    result.fragmentLen = result.pathLen - queryIndex;

    result.pathLen = queryIndex;
  }

  result.initialized = 1;
  result.valid = (result.scheme != NULL && result.path != NULL);
  if (result.authority != NULL) {
    result.valid = (result.pathLen == 0 || result.path[0] == '/');
  } else {
    result.valid = !((result.path[0] == '/') && (result.path[1] == '/'));
  }
  printUri(&result);
  return result;
}

#define ABC_URI_GETTER(field) \
int ABC_URI_##field(ABC_URI* uri, char* buf, size_t* len) { \
  if (uri->initialized != 1) { \
    return -1; \
  } \
  if (uri->field != NULL) { \
    memcpy(buf, uri->field, uri->field##Len); \
    buf[uri->field##Len] = '\0'; \
    return 1; \
  } \
  return 0; \
}

ABC_URI_GETTER(scheme)
ABC_URI_GETTER(pass)
ABC_URI_GETTER(user)
ABC_URI_GETTER(host)
ABC_URI_GETTER(path)
ABC_URI_GETTER(query)
ABC_URI_GETTER(fragment)

#undef ABC_URI_GETTER

#endif


int main() {
  ABC_URI result;

  result = parse("gemini://test:8000");
  parse("foo://example.com:8042/over/there?name=ferret#nose");
  parse("urn:example:animal:ferret:nose");
  parse("jdbc:mysql://test_user:ouupppssss@localhost:3306/sakila?profileSQL=true");
  parse("ftp://ftp.is.co.za/rfc/rfc1808.txt");
  parse("http://www.ietf.org/rfc/rfc2396.txt#header1");
  parse("ldap://[2001:db8::7]/c=GB?objectClass=one&objectClass=two");
  parse("mailto:John.Doe@example.com");
  parse("news:comp.infosystems.www.servers.unix");
  parse("tel:+1-816-555-1212");
  parse("telnet://192.0.2.16:80/");
  parse("urn:oasis:names:specification:docbook:dtd:xml:4.1.2");
  parse("ssh://alice@example.com");
  parse("https://bob:pass@example.com/place");
  parse("http://example.com/?a=1&b=2+2&c=3&c=4&d=\%65\%6e\%63\%6F\%64\%65\%64");

  char buf[1024];
  size_t len;
  result = parse("https://bob:pass@example.com/place?query&test=home#fragments");
  if (ABC_URI_scheme(&result, buf, &len) > 0) {
    printf("%s\n", buf);
  }
  if (ABC_URI_user(&result, buf, &len) > 0) {
    printf("%s\n", buf);
  }
  if (ABC_URI_pass(&result, buf, &len) > 0) {
    printf("%s\n", buf);
  }
  if (ABC_URI_host(&result, buf, &len) > 0) {
    printf("%s\n", buf);
  }
  if (ABC_URI_path(&result, buf, &len) > 0) {
    printf("%s\n", buf);
  }
  if (ABC_URI_query(&result, buf, &len) > 0) {
    printf("%s\n", buf);
  }
  if (ABC_URI_fragment(&result, buf, &len) > 0) {
    printf("%s\n", buf);
  }
}
