#!/bin/bash

function check() {
  CMDA=$(./uri-test $1)

  CMDB=$(./uri-ref $1)

  if [ "$CMDA" == "$CMDB" ];then
      echo "$1 - equal";
    else
      echo "$1 - not equal";
      echo "$CMDA"
      echo ""
  fi
}

check "foo://example.com:8042/over/there?name=ferret#nose"
check "urn:example:animal:ferret:nose"
check "jdbc:mysql://test_user:ouupppssss@localhost:3306/sakila?profileSQL=true"
check "ftp://ftp.is.co.za/rfc/rfc1808.txt"
check "http://www.ietf.org/rfc/rfc2396.txt#header1"
check "ldap://[2001:db8::7]/c=GB?objectClass=one&objectClass=two"
check "mailto:John.Doe@example.com"
check "news:comp.infosystems.www.servers.unix"
check "tel:+1-816-555-1212"
check "telnet://192.0.2.16:80/"
check "urn:oasis:names:specification:docbook:dtd:xml:4.1.2"
