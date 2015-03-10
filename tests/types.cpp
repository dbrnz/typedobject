#include "types.h"

#include <iostream>

const Namespace TEST::NS("test", "http://example.org/properties#") ;
const URI TEST::string = TEST::NS.make_URI("string") ;
const URI TEST::decimal = TEST::NS.make_URI("decimal") ;
const URI TEST::integer = TEST::NS.make_URI("integer") ;
const URI TEST::object = TEST::NS.make_URI("object") ;


int main(void)
/*----------*/
{
  test::Object object("http://example.org/object") ;

  object.set_string("Some string") ;
  object.set_integer(3) ;

  object.set_investigator(rdf::URI("http://example.org/investigator")) ;

  //  PROPERTY_DATETIME(starttime, DCT::created)
  //  PROPERTY_DURATION(duration, DCT::extent)

  std::cout << object.serialise_metadata(rdf::Graph::Format::TURTLE) << std::endl ;
  }
