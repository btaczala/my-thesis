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

#ifndef FUNCTIONPLOT_H
#define FUNCTIONPLOT_H

#include <QMap>

#include <qwt_plot.h>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_array.hpp>

class QWidget ; 
class QMouseEvent ; 
class QKeyEvent ;

class QwtPlotCurve;
class QwtPlotPicker;
namespace Math
{
    class Function2D ; 
}
namespace UI
{   
    class FunctionPlot : public QwtPlot/*, public QObject*/
    {
        friend class MouseEventOnCanvas;
        public:
            typedef boost::shared_ptr<Math::Function2D> tMapFunctionKey;
            typedef boost::shared_ptr<QwtPlotCurve> tMapFunctionValue;
            typedef QMap < tMapFunctionKey , tMapFunctionValue > tMapFunction;

            FunctionPlot( QWidget *pParent ) ;
            virtual ~FunctionPlot() {}
            virtual void            replot();
            void                    addFunction ( Math::Function2D * _pFunction ) ; 
            const QwtPlotPicker *   picker() const { return m_pPlotPicker;}
            
            inline void setXMin( double _value ) { m_XMin = _value ; } 
            inline void setXMax( double _value ) { m_XMax = _value ; } 
            inline void setYMin( double _value ) { m_YMin = _value ; } 
            inline void setYMax( double _value ) { m_YMax = _value ; } 

        protected:
        private:
            tMapFunction            m_MapOfFunctions ; 
            double                  m_XMin ; 
            double                  m_XMax ;
            double                  m_YMin ; 
            double                  m_YMax ; 
            double                  m_Step ; 
            int                     m_NumberOfValues ; 
            
            bool                    m_isLMBPressed ; 
            QPoint                  m_PointWhereLMBWasPressed ;
            QPoint                  m_PointWhereLMBWasReleased ;
            QwtPlotPicker *         m_pPlotPicker ;
    };
}
#endif // FUNCTIONPLOT_H
