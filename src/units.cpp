/******************************************************************************
 *                                                                            *
 *  BioSignalML Management in C++                                             *
 *                                                                            *
 *  Copyright (c) 2010-2015  David Brooks                                     *
 *                                                                            *
 *  Licensed under the Apache License, Version 2.0 (the "License");           *
 *  you may not use this file except in compliance with the License.          *
 *  You may obtain a copy of the License at                                   *
 *                                                                            *
 *      http://www.apache.org/licenses/LICENSE-2.0                            *
 *                                                                            *
 *  Unless required by applicable law or agreed to in writing, software       *
 *  distributed under the License is distributed on an "AS IS" BASIS,         *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
 *  See the License for the specific language governing permissions and       *
 *  limitations under the License.                                            *
 *                                                                            *
 ******************************************************************************/

//============================================================================

#include <typedobject/units.h>

//============================================================================

#include <string>
#include <stdexcept>
#include <unordered_map>

//============================================================================

namespace Unit {

  typedef std::unordered_map<std::string, double> Prefixes ;

  const Prefixes prefixes_1 = { { "y",  1.0e-24 },
                                { "z",  1.0e-21 },
                                { "a",  1.0e-18 },
                                { "f",  1.0e-15 },
                                { "p",  1.0e-12 },
                                { "n",  1.0e-9  },
                                { "u",  1.0e-6  },
                                { "m",  1.0e-3  },
                                { "c",  1.0e-2  },
                                { "d",  1.0e-1  },
                                { "h",  1.0e2   },
                                { "k",  1.0e3   },
                                { "M",  1.0e6   },
                                { "G",  1.0e9   },
                                { "T",  1.0e12  },
                                { "P",  1.0e15  },
                                { "E",  1.0e18  },
                                { "Z",  1.0e21  },
                                { "Y",  1.0e24  } };
  const Prefixes prefixes_2 = { { "da", 1.0e+1  } };

  const Prefixes prefixes_3 = { { "exa",   1.0e18  } };
  const Prefixes prefixes_4 = { { "atto",  1.0e-18 },
                                { "pico",  1.0e-12 },
                                { "nano",  1.0e-9  },
                                { "deci",  1.0e-1  },
                                { "deca",  1.0e+1  },
                                { "kilo",  1.0e3   },
                                { "mega",  1.0e6   },
                                { "giga",  1.0e9   },
                                { "tera",  1.0e12  },
                                { "peta",  1.0e15  } };
  const Prefixes prefixes_5 = { { "yocto", 1.0e-24 },
                                { "zepto", 1.0e-21 },
                                { "femto", 1.0e-15 },
                                { "micro", 1.0e-6  },
                                { "milli", 1.0e-3  },
                                { "centi", 1.0e-2  },
                                { "hecto", 1.0e2   },
                                { "zetta", 1.0e21  },
                                { "yotta", 1.0e24  } };


  enum class Dimension
  /*----------------*/
  {
    None = 0,
    Time,
    Length,
    Mass,
    Current,
    Temperature,
    Luminosity,
    Substance
    };


  class Definition
  /*------------*/
  {
   public:
    Definition();
    Definition(const Dimension pDimension, const Definition *pBase,
               const double pScale, const double pOffset);
    Definition(const Definition & pOther);
    Definition & operator=(const Definition & pOther) ;
    virtual ~Definition();

    static Definition get_definition(const std::string & pName);

    Dimension dimension(void) const;
    double scale(void) const;

   private:
    std::string long_prefix(const std::string &key);
    std::string short_prefix(const std::string &key);
    Dimension mDimension;
    const Definition *mBase;
    double mScale;
    double mOffset;
    };

// A lot of this should go into unitimpl.h

// aliases:
//   min/mins/minutes/minute
//   sec/secs/seconds/second   std::list<std::string>  ??

// ignore case??


  namespace Definitions
  /*-----------------*/
  {
    static const Definition second(Dimension::Time, nullptr,  1.0, 0.0);
    static const Definition minute(Dimension::Time, &second, 60.0, 0.0);
    };

  typedef std::pair<const std::string, const Definition> RegistryPair;
  /*----------------------------------------------------------------*/

  static std::unordered_map<std::string, const Definition> Registry =
  /*---------------------------------------------------------------*/
  {
    { "s", Definitions::second }, { "sec", Definitions::second }, { "second", Definitions::second },
    { "min", Definitions::minute }, { "minute", Definitions::minute }
    };


//============================================================================

  Definition::Definition()
  /*--------------------*/
  : mDimension(Dimension::None),
    mBase(nullptr),
    mScale(1.0),
    mOffset(0.0)
  {
    }

  Definition::Definition(const Dimension pDimension, const Definition *pBase,
  /*-----------------------------------------------------------------------*/
                               const double pScale, const double pOffset)
  : mDimension(pDimension),
    mBase(pBase),
    mScale(pScale),
    mOffset(pOffset)
  {
    }

  Definition::Definition(const Definition & pOther)
  /*---------------------------------------------*/
  : mDimension(pOther.mDimension),
    mBase(pOther.mBase),
    mScale(pOther.mScale),
    mOffset(pOther.mOffset)
  {
    }

  Definition & Definition::operator=(const Definition & pOther)
  /*---------------------------------------------------------*/
  {
    mDimension = pOther.mDimension;
    mBase = pOther.mBase;
    mScale = pOther.mScale;
    mOffset = pOther.mOffset;
    return *this ;
    }

  Definition::~Definition()
  /*---------------------*/
  {
    }


  Definition Definition::get_definition(const std::string & pName)
  /*------------------------------------------------------------*/
  {
    auto entry = Registry.find(pName);
    if (entry != Registry.end()) return entry->second;

    Definition result;
    std::string suffix = result.long_prefix(pName);
    if (suffix != "") {
      entry = Registry.find(suffix);
      if (entry != Registry.end()) {
        const Definition *base = &(entry->second);
        result = Definition(base->mDimension, base, result.mScale, result.mOffset);
        Registry.insert(RegistryPair(pName, result));
        return result;
        }
      }

    suffix = result.short_prefix(pName);
    if (suffix != "") {
      entry = Registry.find(suffix);
      if (entry != Registry.end()) {
        const Definition *base = &(entry->second);
        result = Definition(base->mDimension, base, result.mScale, result.mOffset);
        Registry.insert(RegistryPair(pName, result));
        return result;
        }
      }

    throw std::out_of_range("Unit not in registry: " + pName);
    }


  std::string Definition::long_prefix(const std::string &key)
  /*-------------------------------------------------------*/
  {
    auto prefix = prefixes_3.find(key.substr(0, 3));
    if (prefix != prefixes_3.end()) {
      mScale = prefix->second;
      return key.substr(3);
      }
    prefix = prefixes_4.find(key.substr(0, 4));
    if (prefix != prefixes_4.end()) {
      mScale = prefix->second;
      return key.substr(4);
      }
    prefix = prefixes_5.find(key.substr(0, 5));
    if (prefix != prefixes_5.end()) {
      mScale = prefix->second;
      return key.substr(5);
      }
    return "";
    }

  std::string Definition::short_prefix(const std::string &key)
  /*--------------------------------------------------------*/
  {
    auto prefix = prefixes_2.find(key.substr(0, 2));
    if (prefix != prefixes_2.end()) {
      mScale = prefix->second;
      return key.substr(2);
      }
    prefix = prefixes_1.find(key.substr(0, 1));
    if (prefix != prefixes_1.end()) {
      mScale = prefix->second;
      return key.substr(1);
      }
    return "";
    }

  Dimension Definition::dimension(void) const
  /*---------------------------------------*/
  {
    return mDimension;
    }

  double Definition::scale(void) const
  /*--------------------------------*/
  {
    double scale = mScale;
    auto base = mBase;
    while (base != nullptr) {
      scale *= base->mScale ;
      base = base->mBase;
      }
    return scale;
    }

//============================================================================

// NEEDS to work with full URIs as well as names...
// Converter::Converter(const rdf::URI & pFromUnits, const rdf::URI & pToUnits)

  Converter::Converter(const std::string & pFromUnits, const std::string & pToUnits)
  /*------------------------------------------------------------------------------*/
  : mScale(1.0), mOffset(0.0)
  {
    const Definition from = Definition::get_definition(pFromUnits);
    const Definition to = Definition::get_definition(pToUnits);
    if (from.dimension() != to.dimension()) {
      throw std::domain_error("Cannot convert between units with different dimensions");
      }
    mScale = from.scale()/to.scale();
    }

  double Converter::convert(const double pValue) const
  /*------------------------------------------------*/
  {
    return mScale*pValue + mOffset;
    }

//============================================================================

  };

//============================================================================

