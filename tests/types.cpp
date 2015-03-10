#include "types.h"

#include <iostream>


int main(void)
/*----------*/
{
  test::Object object("http://example.org/object") ;

  object.set_label("Some label") ;

  object.set_investigator(rdf::URI("http://example.org/investigator")) ;

  //  PROPERTY_DATETIME(starttime, DCT::created)
  //  PROPERTY_DURATION(duration, DCT::extent)

  std::cout << object.serialise_metadata(rdf::Graph::Format::TURTLE) << std::endl ;
  }
