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
  std::string id = "http://example.org/id#1" ;
  rdf::URI uri(id) ;
  BOOST_WARN(id != uri.to_string()) ;
  }
