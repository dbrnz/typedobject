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

  object->set_string("Some string") ;
  object->set_integer(3) ;
  object->set_decimal(3.1415926) ;
  object->set_starttime(xsd::Datetime("2015-05-11 14:47:30")) ;  // T
  object->set_duration(xsd::Duration("PT3H4M5.67S")) ;
  object->set_investigator(rdf::URI("http://example.org/investigator")) ;
  object->set_subobject(sobj2) ;

  object->add_resource<example::SubObject>(sobj2) ;

  object->set_resource(uri3) ;

  std::string turtle = object->serialise_metadata(rdf::Graph::Format::TURTLE) ;
  std::cout << turtle << std::endl ;

  auto graph = rdf::Graph::create_from_string(rdf::URI(), turtle, rdf::Graph::Format::TURTLE) ;

  // Restricted to "integer == 3"
  auto from_rdf = example::RestrictedObject::create_from_graph(uri1, graph) ;
  // So how do we fail??
  // Also how do we fail if "uri a Object" isn't in graph ???

  if (from_rdf->is_valid()) {
    std::cout << "URI:          " << from_rdf->uri()          << std::endl
              << "String:       " << from_rdf->string()       << std::endl
              << "Integer:      " << from_rdf->integer()      << std::endl
              << "Decimal:      " << from_rdf->decimal()      << std::endl
              << "Starttime:    " << from_rdf->starttime()    << std::endl
              << "Duration:     " << from_rdf->duration()     << std::endl
              << "SubObject:    " << from_rdf->subobject()    << std::endl
              << "Resource:     " << from_rdf->resource()     << std::endl
              << std::endl ;

    auto r2 = from_rdf->get_resource<example::SubObject>(uri2) ;
    if (r2->is_valid()) {
      std::cout << " R2: " << r2.get() << "  URI: " << r2->uri() << std::endl << std::endl ;
      }
    }

// Reference isn't created since we have no <uri3 a SubObject>
  else std::cout << "Restricted object cannot be created" << std::endl ;

  std::cout << from_rdf->serialise_metadata(rdf::Graph::Format::TURTLE) << std::endl ;


  rdf::URI time_uri1("http://example.org/time/1") ;
  rdf::URI time_uri2("http://example.org/time/2") ;

  auto tline = std::make_shared<example::RelativeTimeLine>(time_uri2) ;

  example::Interval interval(time_uri1) ;

  interval.set_timeline(tline) ;
  interval.set_start(0.0) ;
  interval.set_duration(10.0) ;
  std::cout << interval.serialise_metadata(rdf::Graph::Format::TURTLE) << std::endl ;

  }
