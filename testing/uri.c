#include <stdlib.h>
#include <stdio.h>

#define ABC_URI_IMPLEMENTATION
#include "../ABC_uri.h"

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
