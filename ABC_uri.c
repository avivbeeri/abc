#include <stdio.h>
#include <string.h>
typedef struct {
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

} ABC_URI;

void printUri(ABC_URI* uri) {
  printf("URI       = %s\n", uri->uri);
  if (uri->scheme != NULL) {
    printf("Scheme    = %.*s\n", uri->schemeLen, uri->scheme);
  }
  /*
  if (uri->authority != NULL) {
    printf("Authority = %.*s\n", uri->authorityLen, uri->authority);
  }
  if (uri->cred != NULL) {
    printf("Cred      = %.*s\n", uri->credLen, uri->cred);
  }
  */
  if (uri->user != NULL) {
    printf("User      = %.*s\n", uri->userLen, uri->user);
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

  printf("\n");
//   printf("%.*s\n", uri->len, uri->scheme + uri->schemeLen);
}

size_t indexOf(char* str, char* substr) {
  char* c = strstr(str, substr);
  if (c == NULL) {
    return -1;
  }
  return c - str;
}
size_t indexOfN(char* str, char* substr, size_t len) {
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

  index = indexOf(uri, "//");
  if (index >= 0 && countOf(uri, ":", index) == 1) {
    result.scheme = uri;
    result.schemeLen = index - 1;
    strIndex = index + 2;

    str = uri + strIndex;
    // parse the domain
    index = indexOf(str, "/");
    result.authority = str;
    if (index == -1) {
      result.authorityLen = result.len - strIndex;
    } else {
      result.authorityLen = index;
    }

    // check for credentials and port
    if (countOf(result.authority, "@", result.authorityLen) > 0) {
      result.cred = result.authority;
      index = indexOf(str, "@");
      if (index == -1) {
        result.credLen = result.len - strIndex;
      } else {
        result.credLen = index;
      }
      result.user = result.cred;
      result.userLen = result.credLen;
      index = indexOfN(result.user, ".", result.credLen);
      if (index != -1) {
        result.userLen = index;
        result.pass = result.user + index + 1;
        result.passLen = result.credLen - (index + 1);
      } else {
        index = indexOfN(result.user, ":", result.credLen);
        if (index != -1) {
          result.userLen = index;
          result.pass = result.user + index + 1;
          result.passLen = result.credLen - (index + 1);
        }
      }
      str = result.cred + result.credLen + 1;
      index = indexOf(str, "/");
      result.host = str;
      if (index == -1) {
        result.hostLen = result.authorityLen - result.credLen;
      } else {
        result.hostLen = index;
      }
      index = indexOfN(str, ":", result.hostLen);
      if (index != -1) {
        result.hostLen = index;
      }
    } else if (countOf(result.authority, ":", result.authorityLen) > 0 &&
        countOf(result.authority, "[", result.authorityLen) == 0 &&
        countOf(result.authority, "]", result.authorityLen) == 0) {
      str = result.authority;
      index = indexOfN(str, ":", result.authorityLen);
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
        indexOfN(result.authority, "]:", result.authorityLen) != -1) {
      str = result.authority;
      index = indexOfN(str, "]", result.authorityLen);
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
    index = indexOf(uri, ":");
    str = uri;
    if (index != -1) {
      result.scheme = uri;
      result.schemeLen = index;
      str = result.scheme + index + 1;
    }
  }
  // parse the path;
  result.path = str;
  result.pathLen = result.len - (uri - str);

  size_t queryIndex = indexOfN(result.path, "?", result.pathLen);
  size_t fragmentIndex = indexOfN(result.path, "#", result.pathLen);
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
    result.queryLen = queryIndex;

    result.fragment = result.query + result.queryLen + 1;
    result.fragmentLen = result.pathLen - queryIndex;

    result.pathLen = queryIndex;
  }
  printUri(&result);
  return result;
}


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
}
