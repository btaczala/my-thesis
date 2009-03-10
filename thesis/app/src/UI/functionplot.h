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
namespace Math
{
    class Function2D ; 
}
namespace UI
{   
    class FunctionPlot : public QwtPlot
    {
        public:
            typedef boost::shared_ptr<Math::Function2D> tMapFunctionKey;
            typedef boost::shared_ptr<QwtPlotCurve> tMapFunctionValue;
            typedef QMap < tMapFunctionKey , tMapFunctionValue > tMapFunction;
            FunctionPlot( QWidget *pParent ) ;
            virtual void            replot();
            void                    addFunction ( Math::Function2D * _pFunction ) ; 
        protected:
            virtual void            mousePressEvent(QMouseEvent* );
            virtual void            mouseReleaseEvent(QMouseEvent* );
            virtual void            mouseMoveEvent(QMouseEvent* );
            virtual void            keyPressEvent(QKeyEvent* );
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
    };
    class MouseEventOnCanvas : public QObject
    {
        Q_OBJECT
        public:
            MouseEventOnCanvas( QObject *pParent ) : QObject(pParent){}
        protected:
            bool eventFilter(QObject *obj, QEvent *event) ; 
    };
}
#endif // FUNCTIONPLOT_H
