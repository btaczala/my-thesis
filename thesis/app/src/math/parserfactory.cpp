/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "parserfactory.h"
#include "muparser/muParser.h"
#include <boost/math/special_functions/sinc.hpp>
mu::Parser* Math::ParserFactory::factory() {
    // add all functions from boost::math and declare most common constanst 
    // caller taking ownership
    mu::Parser *pParser  = new mu::Parser() ; 
    pParser->DefineFun(false,&SpecialFunctions::sinc_pi,"sinc_pi");
    pParser->DefineFun(false,&SpecialFunctions::NormalDistribution,"square");
    return pParser ; 
}
double Math::SpecialFunctions::NormalDistribution(double x ) {
    return x*x ; 
}
double Math::SpecialFunctions::sinc_pi(double x) {
    if ( x == 0 ) 
        return -1;
    return boost::math::sinc_pi<double>(x);
}



