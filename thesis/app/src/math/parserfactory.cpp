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
#include <boost/math/special_functions/asinh.hpp>
#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/distributions/normal.hpp>


//#define ADD_FUN(func_name) #func_name ##, Math::SpecialFunctions::func_name, false 
namespace Math
{
    namespace SpecialFunctions
    {
        static double square ( double _x ) 
        { return _x * _x ; }
        
        static double gamma( double _x ) 
        { return boost::math::tgamma(_x) ; }
        static double normal( double _x ) 
        { return boost::math::pdf( boost::math::normal(),_x); }
        static double normal3( double _x,double _y, double _z) 
        { return boost::math::pdf( boost::math::normal(_y,_z),_x); }
    };
}

mu::Parser* Math::ParserFactory::factory() {
    mu::Parser *pParser  = new mu::Parser() ; 
    pParser->DefineFun("square", Math::SpecialFunctions::square, false );
    pParser->DefineFun("gamma", Math::SpecialFunctions::gamma, false );
    pParser->DefineFun("normal", Math::SpecialFunctions::normal, false );
    pParser->DefineFun("normal3", Math::SpecialFunctions::normal3, false );
    
    return pParser ; 
}
