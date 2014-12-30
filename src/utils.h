#ifndef AOBJECT_UTILS_H

#include "rdf.h"

#include <string>


namespace utils {
  typedef uint64_t Datetime ;

  rdf::URI make_uri(const rdf::Node &uri) ;
  rdf::URI make_uri(const std::string &uri) ;
  rdf::Node datetime_to_isoformat(const Datetime & dt) ;
  Datetime isoformat_to_datetime(const rdf::Node & node) ;
  } ;

#endif
