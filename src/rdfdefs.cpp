#include "defs.h"


namespace rdf {

  const Namespace RDF::NS("rdf", "http://www.w3.org/1999/02/22-rdf-syntax-ns#") ;
  const URI RDF::type = RDF::NS.make_URI("type") ;

  const Namespace RDFS::NS("rdfs", "http://www.w3.org/2000/01/rdf-schema#") ;
  const URI RDFS::label = RDFS::NS.make_URI("label") ;
  const URI RDFS::comment = RDFS::NS.make_URI("comment") ;

  const Namespace OWL::NS("owl", "http://www.w3.org/2002/07/owl#") ;
  const URI OWL::Object = OWL::NS.make_URI("Object") ;

  const Namespace DCT::NS("dct", "http://purl.org/dc/terms/") ;
  const URI DCT::description = DCT::NS.make_URI("description") ;
  const URI DCT::creator = DCT::NS.make_URI("creator") ;
  const URI DCT::created = DCT::NS.make_URI("created") ;
  const URI DCT::format = DCT::NS.make_URI("format") ;
  const URI DCT::source = DCT::NS.make_URI("source") ;
  const URI DCT::subject = DCT::NS.make_URI("subject") ;
  const URI DCT::extent = DCT::NS.make_URI("extent") ;

  const Namespace PRV::NS("prv", "http://purl.org/net/provenance/ns#") ;
  const URI PRV::precededBy = PRV::NS.make_URI("precededBy") ;

  const Namespace PROV::NS("prov", "http://www.w3.org/ns/prov#") ;
  const URI PROV::wasGeneratedBy = PROV::NS.make_URI("generaratedBy") ;

  const Namespace TIME::NS("time", "http://www.w3.org/2006/time#") ;
  const URI TIME::TemporalEntity = TIME::NS.make_URI("TemporalEntity") ;

  const Namespace TL::NS("tl", "http://purl.org/NET/c4dm/timeline.owl#") ;
  const URI TL::RelativeTimeLine = TL::NS.make_URI("RelativeTimeLine") ;
  const URI TL::timeline = TL::NS.make_URI("timeline") ;
  const URI TL::at = TL::NS.make_URI("at") ;
  const URI TL::start = TL::NS.make_URI("start") ;
  const URI TL::duration = TL::NS.make_URI("duration") ;

/**
  'xsd':  'http://www.w3.org/2001/XMLSchema#',
  'uome': 'http://www.sbpax.org/uome/list.owl#',
**/

  } ;
