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

#include "functionplot.h"
#include "../math/function.h"
#include <QWidget>
#include <QMouseEvent>

#include <qwt_plot_curve.h>

#include <cmath>
UI::FunctionPlot::FunctionPlot(QWidget* pParent) {
    m_XMin = -5 ; 
    m_XMax = 5 ;
    m_YMin = -2 ; 
    m_YMax = 2 ; 
    m_Step = 0.01; 
    m_NumberOfValues = (int) ( abs(m_XMin) + abs(m_XMax) ) / m_Step ; 
    setAxisScale(0,m_YMin,m_YMax);
    setAxisScale(2,m_XMin,m_XMax);
}
void UI::FunctionPlot::addFunction( Math::Function2D *  _pFunction) {
    boost::scoped_array<double> pXs ; 
    boost::scoped_array<double> pYs ; 
    QwtPlotCurve * pCurve = new QwtPlotCurve(_pFunction->equation().c_str() );
    _pFunction->getData ( pXs , pYs , m_NumberOfValues , m_XMin , m_XMax , m_Step ) ; 
    pCurve->setData( pXs.get(),pYs.get(), m_NumberOfValues ) ;
    pCurve->attach( this );
    
    m_MapOfFunctions.insert(tMapFunctionKey( _pFunction ) , tMapFunctionValue( pCurve ) );
    replot();
}
void UI::FunctionPlot::replot() {
    QwtPlot::replot();
}
void UI::FunctionPlot::mousePressEvent(QMouseEvent* pEvent) {
    if ( pEvent->button() == Qt::LeftButton ) 
    {
        m_PointWhereLMBWasPressed = pEvent->pos();
        m_isLMBPressed = true ; 
    }
    QwtPlot::mousePressEvent(pEvent);
}
void UI::FunctionPlot::mouseReleaseEvent(QMouseEvent* pEvent) {
    if ( m_isLMBPressed ) 
    {
        m_PointWhereLMBWasReleased = pEvent->pos();
        int xShift = m_PointWhereLMBWasReleased.x() - m_PointWhereLMBWasPressed.x();
        int yShift = m_PointWhereLMBWasReleased.y() - m_PointWhereLMBWasPressed.y();
    }
    QwtPlot::mouseReleaseEvent(pEvent);
}
void UI::FunctionPlot::mouseMoveEvent(QMouseEvent* pEvent) {
    QwtPlot::mouseMoveEvent(pEvent);
}
void UI::FunctionPlot::keyPressEvent(QKeyEvent* pEvent) {
    QwtPlot::keyPressEvent(pEvent);
}
