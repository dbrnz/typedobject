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

#include <typedobject/xsd.h>

#include <iostream>

void test(double t)
/*---------------*/
{
  auto d = xsd::Duration{t} ;
  std::cout << "Double: " << t << " =  '" << d.to_string() << "' " << d.to_double() << std::endl ;
  }


int main(void)
/*----------*/
{
  try {
    xsd::Duration minute1(60) ;
    xsd::Duration minute2(1, "minute") ;

    std::cout << "1 minute = " << minute1.to_string()
              << "  seconds: " << (double)minute1
              << std::endl ;

    std::cout << "1 minute = " << minute2.to_string()
              << "  minutes: " << minute2.to_double("minute")
              << std::endl ;
    }
  catch (const std::exception &error) {
    std::cout << error.what() << std::endl ;
    }

  test(0.054099) ;
  test(0.0271) ;
  }
