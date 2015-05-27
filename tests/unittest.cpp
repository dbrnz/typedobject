#include <typedobject/rdf.h>

#define BOOST_TEST_MODULE unittest
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(node)
{
  rdf::Node invalid ;
  BOOST_WARN(invalid.is_valid()) ;
  }


BOOST_AUTO_TEST_CASE(uri)
{
  std::string id = "http://example.org/id/1" ;
  rdf::URI uri(id) ;
  BOOST_CHECK(id == uri.to_string()) ;
  }

BOOST_AUTO_TEST_CASE(relative_uri)
{
  std::string base = "http://example.org/id/" ;
  std::string id = "2" ;
  rdf::URI uri(id, base) ;
  BOOST_CHECK((base + id) == uri.to_string()) ;
  }

BOOST_AUTO_TEST_CASE(invalid_uri)
{
  rdf::URI uri("invalid") ;
  BOOST_CHECK(uri.is_valid()) ;
  }
