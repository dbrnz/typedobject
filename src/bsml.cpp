#include "bsml.h"


// bsml.cpp
const rdf::Namespace bsml::BSML::NS("bsml", "http://www.biosignalml.org/ontologies/2011/04/biosignalml#") ;

const rdf::URI bsml::BSML::Recording = BSML::NS.make_URI("Recording") ;
const rdf::URI bsml::BSML::dataset = BSML::NS.make_URI("dataset") ;
