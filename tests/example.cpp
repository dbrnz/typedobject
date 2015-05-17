#include "example.h"

#include <typedobject/rdf.h>
#include <typedobject/xsd.h>

#include <iostream>


const Namespace TEST::NS("test", "http://example.org/properties#") ;
const URI TEST::string = TEST::NS.make_URI("string") ;
const URI TEST::decimal = TEST::NS.make_URI("decimal") ;
const URI TEST::integer = TEST::NS.make_URI("integer") ;
const URI TEST::object = TEST::NS.make_URI("object") ;


int main(void)
/*----------*/
{
  std::string object_uri = "http://example.org/object" ;

  example::Object object(object_uri) ;

  object.set_string("Some string") ;
  object.set_integer(3) ;
  object.set_decimal(3.1415926) ;
  object.set_starttime(xsd::Datetime("2015-05-11 14:47:30")) ;  // T
  object.set_duration(xsd::Duration("PT3H4M5.67S")) ;
  object.set_investigator(rdf::URI("http://example.org/investigator")) ;

  std::string turtle = object.serialise_metadata(rdf::Graph::Format::TURTLE) ;
  std::cout << turtle << std::endl ;

  rdf::Graph graph ;
  graph.parse_string(turtle, rdf::Graph::Format::TURTLE) ;
  example::Object from_rdf(object_uri, graph) ;

  std::cout << "String:       " << from_rdf.string()  << std::endl
            << "Integer:      " << from_rdf.integer() << std::endl
            << "Decimal:      " << from_rdf.decimal() << std::endl
            << "Starttime:    " << from_rdf.starttime().to_string() << std::endl
            << "Duration:     " << from_rdf.duration().to_string() << std::endl
            << "Investigator: " << from_rdf.investigator().to_string() << std::endl
            << std::endl ;

  std::cout << from_rdf.serialise_metadata(rdf::Graph::Format::TURTLE) << std::endl ;
  }
