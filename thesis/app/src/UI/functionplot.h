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
class QTimer ; 

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
        public:
            /*!
             * \brief  ctor for FunctionPlot
             */
            FunctionPlot( QWidget *pParent ) ;
            virtual ~FunctionPlot() {}
            virtual void            replot();
            void                    addFunction ( const QString& _equation ) ; 
            const QwtPlotPicker *   picker() const { return m_pPlotPicker;}
            
            void setXMin( double _value ) ; 
            void setXMax( double _value ) ;
            void setYMin( double _value ) ;
            void setYMax( double _value ) ;
            
            inline double xMin () const {return m_XMin;}
            inline double xMax () const {return m_XMax;}
            inline double yMin () const {return m_YMin;}
            inline double yMax () const {return m_YMax;}

        protected:
        private:
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
            boost::shared_ptr<Math::Function2D> m_pFunction ; 
            boost::shared_ptr<QwtPlotCurve> m_pFunctionCurve ; 
            boost::shared_ptr<QTimer> m_pTimer ; 
    };
}
#endif // FUNCTIONPLOT_H
