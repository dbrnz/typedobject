#ifndef AOBJECT_UTILS_H

#include "rdf.h"

#include <string>


namespace utils {
  typedef uint64_t Datetime ;
  typedef double Duration ;


  rdf::Node datetime_to_isoformat(const Datetime & dt) ;
  Datetime isoformat_to_datetime(const rdf::Node & node) ;

  rdf::Node seconds_to_isoduration(const Datetime & dt) ;
  Duration isoduration_to_seconds(const rdf::Node & node) ;
  } ;

#endif
