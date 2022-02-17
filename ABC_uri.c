#include <stdio.h>
#include <string.h>
typedef struct {
  const char* uri;
  size_t len;

  const char* scheme;
  size_t schemeLen;

} ABC_URI;

size_t indexOf(char* str, char* substr) {
  char* c = strstr(str, substr);
  if (c == NULL) {
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
  char* start = uri;
  result.uri = uri;
  result.len = strlen(uri);

  size_t index = 0;

  index = indexOf(uri, "//");
  if (index >= 0 && countOf(uri, ":", index) == 1) {
    result.scheme = uri;
    result.schemeLen = index - 1;

    start = uri + index;




  } else {
    index = indexOf(uri, ":");
    if (index != -1) {
      result.scheme = uri;
      result.schemeLen = index;
    }
  }
  return result;
}

void printUri(ABC_URI* uri) {
  printf("URI      = %s\n", uri->uri);
  printf("Scheme   = %.*s\n", uri->schemeLen, uri->scheme);
  printf("%.*s\n", uri->len, uri->scheme + uri->schemeLen);
}

int main() {
  ABC_URI result;

  result = parse("foo://example.com:8042/over/there?name=ferret#nose");
  printUri(&result);
  result = parse("tel:+1-816-555-1212");
  printUri(&result);
  result = parse("gemini://test:8000");
  printUri(&result);
}
