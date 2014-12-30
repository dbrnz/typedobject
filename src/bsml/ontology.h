/**
Provide access to the BioSignalML ontology.

Generated from file:///Users/dave/biosignalml/workspace/ontologies/bsml/2011-04-biosignalml.ttl at 12:12:54 Wed 31 Dec 2014

Full documentation of the ontology is at http://www.biosignalml.org/ontologies/2011/04/biosignalml.html
**/

#ifndef BSML_ONTOLOGY_H

#include "rdf/rdf.h"
#include "rdf/defs.h"

namespace bsml {

  class BSML {
   public:
    static const rdf::Namespace NS ;

// owl:Class resources:
    static const rdf::URI Annotation ;
    static const rdf::URI BP_Filter ;
    static const rdf::URI Device ;
    static const rdf::URI Electrode ;
    static const rdf::URI ErrorTag ;
    static const rdf::URI Event ;
    static const rdf::URI EventType ;
    static const rdf::URI Filter ;
    static const rdf::URI HP_Filter ;
    static const rdf::URI Instant ;
    static const rdf::URI Interval ;
    static const rdf::URI LP_Filter ;
    static const rdf::URI Notch_Filter ;
    static const rdf::URI Recording ;
    static const rdf::URI RecordingGraph ;
    static const rdf::URI SampleClock ;
    static const rdf::URI Segment ;
    static const rdf::URI SemanticTag ;
    static const rdf::URI Sensor ;
    static const rdf::URI Signal ;
    static const rdf::URI SignalType ;
    static const rdf::URI Simulation ;
    static const rdf::URI Source ;
    static const rdf::URI TemporalEntity ;
    static const rdf::URI Transducer ;
    static const rdf::URI UniformSignal ;
    static const rdf::URI UnitOfMeasure ;

// owl:DatatypeProperty resources:
    static const rdf::URI dataBits ;
    static const rdf::URI filterFrequency ;
    static const rdf::URI index ;
    static const rdf::URI maxFrequency ;
    static const rdf::URI maxValue ;
    static const rdf::URI minFrequency ;
    static const rdf::URI minValue ;
    static const rdf::URI offset ;
    static const rdf::URI period ;
    static const rdf::URI rate ;
    static const rdf::URI resolution ;

// owl:NamedIndividual resources:
    static const rdf::URI BP ;
    static const rdf::URI ECG ;
    static const rdf::URI EEG ;
    static const rdf::URI ErrorTAG ;

// owl:ObjectProperty resources:
    static const rdf::URI clock ;
    static const rdf::URI dataset ;
    static const rdf::URI eventType ;
    static const rdf::URI preFilter ;
    static const rdf::URI recording ;
    static const rdf::URI sensor ;
    static const rdf::URI signalType ;
    static const rdf::URI tag ;
    static const rdf::URI time ;
    static const rdf::URI uncertainty ;
    static const rdf::URI units ;
    } ;

  } ;

#endif