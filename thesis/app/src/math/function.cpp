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

#include "function.h"
#include <QDebug>
#include "muparser/muParser.h"
#include "parserfactory.h"

Math::Function2D::Function2D( const std::string & _equation) : 
  m_Equation(_equation) ,
  m_apMatrixOfValues( NULL ) 
{
    if ( ! _equation.empty() ) 
        setEquation( _equation ) ; // to switch function type to cintinous
}
Math::Function2D::~Function2D() {
}
double Math::Function2D::value( double _x ) {
    if ( m_Type == Math::eUnknown ) 
        return -1 ; 
    else if ( m_Type == Math::eContinuous ) 
        return m_Parser.evaluate(_x);
    return -1 ; 
}
void Math::Function2D::setEquation(const std::string& _equation) {
    ///TODO: check if _equation has 'x' 
    ///TODO: check if _equation is valid function definition
    m_Equation = _equation ; 
    m_Parser.setEquation( m_Equation );
    m_Type = Math::eContinuous;
}

/// Function2D::Parser impl
Math::Function2D::Parser::Parser(const std::string& _equation) : m_pParser ( Math::ParserFactory::factory() )  {
    m_pParser->SetExpr( _equation );
    m_pParser->DefineVar("x",&m_fVal);
}
Math::Function2D::Parser::~Parser() {
}

void Math::Function2D::Parser::setEquation(const std::string& _equation) {
    m_pParser->SetExpr( _equation );
    m_pParser->DefineVar("x",&m_fVal);
}
double Math::Function2D::Parser::evaluate(double _x) {
    m_fVal = _x ; 
    double ret ; 
    ret = m_pParser->Eval();
    return ret ; 
}
void Math::Function2D::getData(boost::scoped_array< double >& _x, boost::scoped_array< double >& _y, int _size, double _xMin, double _xMax, double _step) {
    if ( _x.get() == NULL ) 
        _x.reset(new double[_size] );
    if ( _y.get() == NULL ) 
        _y.reset(new double[_size] );
    double iter = _xMin ; 
    for ( int i = 0 ; i < _size ; ++i ) 
    {
        if ( iter > _xMax ) 
            break ; 
        try
        {   
            _y[i] = value(iter); // this will throw exception
            _x[i] = iter ; 
        }
        catch ( mu::Parser::exception_type & e ) 
        {
            qDebug() << " This will not be evaluated" << equation().c_str() << "with" << i << " exception: " << e.GetMsg().c_str() ; 
            i-- ; 
        }
        catch ( std::domain_error & err ) 
        {
            // this will not be evaluated ! 
            qDebug() << " This will not be evaluated" << equation().c_str() << "with" << i << " exception: " << err.what() ; 
            i-- ; 
        }
        iter += _step;
    }
}
