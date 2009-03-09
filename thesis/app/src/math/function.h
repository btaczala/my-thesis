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

#ifndef MATH_FUNCTION_H
#define MATH_FUNCTION_H
#include <string>
#include <memory>

#include "Eigen/Core"
//#include "muparser/muParser.h"
#include <boost/scoped_array.hpp>

USING_PART_OF_NAMESPACE_EIGEN
namespace mu
{
    class Parser ; 
}
namespace Math {
    /*!
     * \brief Enumeration over types of function avaible. 
     *          Function can exist in only three states
     */
    enum FunctionDef
    {
        eUnknown=0, /*!< Unknown. When function does not have equation, and no data has been assign */
        eDiscrete,/*!< Discrete. When function is in fact in Matrix form. */
        eContinuous /*!< Continuous. When function is given by equation, so matrix is not used*/
    };
    /*!
     * \author Bartosz.Taczala
     * \brief class defining 2d function 
     */
    class Function2D 
    {
        public:
            /*!
             * \brief ctor
             * \param _equation [in] equation of function 
             * \see setEquation()
             */
            Function2D( const std::string & _equation ="") ; 
            ~Function2D() ; 
            /*!
             * \brief set eqution for given func
             * \param _equation - equation of function 
             */
            void setEquation ( const std::string & _equation ) ;
            /*!
             * \brief return  eqution for given func
             * \see Function2D()
             * \return  equation of function 
             */
            inline const std::string & equation( ) const { return m_Equation ; }
            /*!
             * \brief return  type of function. This cannot be modified by user explicitly. 
             * \see Function2D()
             * \see setEquation()
             * \return  type of function
             */
            inline FunctionDef typeOf() const ; 
            /*!
             * \brief Calculates f(x)
             * \param _x 
             * \return  value of function in point x
             */
            double value( double _x ) ; 
            
            void getData ( boost::scoped_array<double> & _x, boost::scoped_array<double> & _y , int _size, double _xMin, double _xMax, double _step ) ; 
        private:
            /// fields
            std::string m_Equation ; /*! < equation of function */
            std::auto_ptr<MatrixXd> m_apMatrixOfValues ; /*! < When function is in discrete form, this cannot be NULL. Otherwise NULL*/
            FunctionDef m_Type ; /*! < type of function*/
            /// methods 
            /*! 
             * \brief wrapper around mu::Parser
             */
            class Parser
            {
                public:
                    Parser( const std::string & _equation="") ;
                    inline void setEquation( const std::string & _equation );
                    ~Parser();
                    double evaluate( double _x ) ; 
                private:
                    std::auto_ptr<mu::Parser> m_pParser ; 
                    double m_fVal ; 
            } m_Parser ; 
            boost::scoped_array<double> m_Xs ; 
            boost::scoped_array<double> m_Ys ; 
    };
    class Function3D
    {
        ;
    };
}
#endif // MATH_FUNCTION_H
