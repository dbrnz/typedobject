#ifndef BSML_RECORDING_H
#define BSML_RECORDING_H


#include "aobject.h"
#include "rdf.h"
#include "bsml.h"

#include <string>


namespace bsml {

  class Recording : public AObject::AObject
  /*-------------------------------------*/
  {
    A_OBJECT
    METACLASS(BSML::Recording)
    PROPERTY_STRING(format, rdf::DCT::format)
    PROPERTY_NODE(dataset, bsml::BSML::dataset)
    PROPERTY_NODE(source, rdf::DCT::source)
    PROPERTY_NODE(subject, rdf::DCT::subject)

   public:
    Recording(const std::string &uri) ;

    } ;

  } ;


/**
  metaclass = BSML.Recording  #: :attr:`.BSML.Recording`

  mapping = { 'format':        PropertyMap(DCT.format),
              'dataset':       PropertyMap(BSML.dataset),
              'source':        PropertyMap(DCT.source, functional=False),
              'investigation': PropertyMap(DCT.subject),
              'investigator':  PropertyMap(DCT.creator),
              'starttime':     PropertyMap(DCT.created, XSD.dateTime,
                                           utils.datetime_to_isoformat,
                                           utils.isoformat_to_datetime),
              'duration':      PropertyMap(DCT.extent, XSD.dayTimeDuration,
                                           utils.seconds_to_isoduration,
                                           utils.isoduration_to_seconds),
##            'digest':        PropertyMap(BSML.digest),
              'timeline':      PropertyMap(TL.timeline,
                                           to_rdf=PropertyMap.get_uri,
                                           from_rdf=_get_timeline, subelement=True),
              'generatedBy':   PropertyMap(PROV.wasGeneratedBy, to_rdf=PropertyMap.get_uri,
                                           subelement=True),
            }

  SignalClass = Signal       #: The class of :class:`~.signal.Signal`\s in the Recording
  EventClass  = Event        #: The class of :class:`~.event.Event`\s in the Recording

**/

#endif