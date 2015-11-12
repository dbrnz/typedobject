#include "example.h"

#include <iostream>


int main(void)
/*----------*/
{
  rdf::URI uri1("http://example.org/object/1") ;
  rdf::URI uri2("http://example.org/object/2") ;
  rdf::URI uri3("http://example.org/object/3") ;


  auto object = example::Object::create(uri1) ;


  auto sobj2 = example::SubObject::create(uri2) ;
  sobj2->set_parent(object) ;

  auto sobj3 = example::SubObject::create(uri3) ;


  std::string turtle = object.serialise_metadata(rdf::Graph::Format::TURTLE) ;
  std::cout << turtle << std::endl ;
  object->set_string("Some string") ;
  object->set_integer(3) ;
  object->set_decimal(3.1415926) ;
  object->set_starttime(xsd::Datetime("2015-05-11 14:47:30")) ;  // T
  object->set_duration(xsd::Duration("PT3H4M5.67S")) ;
  object->set_investigator(rdf::URI("http://example.org/investigator")) ;
  object->set_subobject(sobj2) ;

//  object->add_resource<example::SubObject>(sobj2) ;

  object->set_resource(uri3) ;

  rdf::Graph graph ;
  graph.parse_string(turtle, rdf::Graph::Format::TURTLE) ;

  example::RestrictedObject from_rdf(uri1, graph) ;  // Restricted to "integer == 3"
                                                           // So how do we fail??
  // Also how do we fail if "object_uri a Object" isn't in graph ???

  std::cout << "Valid:        " << from_rdf.is_valid()     << std::endl
            << "URI:          " << from_rdf.uri()          << std::endl
            << "String:       " << from_rdf.string()       << std::endl
            << "Integer:      " << from_rdf.integer()      << std::endl
            << "Decimal:      " << from_rdf.decimal()      << std::endl
            << "Starttime:    " << from_rdf.starttime()    << std::endl
            << "Duration:     " << from_rdf.duration()     << std::endl
            << "SubObject:    " << from_rdf.subobject()    << std::endl
            << "Reference:    " << (std::string)from_rdf.reference()    << std::endl
            << std::endl ;

  std::cout << from_rdf.serialise_metadata(rdf::Graph::Format::TURTLE) << std::endl ;

  rdf::URI time_uri1("http://example.org/time/1") ;
  rdf::URI time_uri2("http://example.org/time/2") ;

  auto tline = std::make_shared<example::RelativeTimeLine>(time_uri2) ;

  example::Interval interval(time_uri1) ;

  interval.set_timeline(tline) ;
  interval.set_start(0.0) ;
  interval.set_duration(10.0) ;
  std::cout << interval.serialise_metadata(rdf::Graph::Format::TURTLE) << std::endl ;
  }
