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
/// qt
#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>

///qwt
#include <qwt_plot_curve.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
///std
#include <cmath>
UI::FunctionPlot::FunctionPlot(QWidget* pParent) : QwtPlot(QwtText( "function") ,pParent) {
    m_pTimer.reset( new QTimer() ) ; 
    m_XMin = -5 ; 
    m_XMax = 5 ;
    m_YMin = -2 ; 
    m_YMax = 2 ; 
    m_Step = 0.01; 
    m_NumberOfValues = (int) ( abs(m_XMin) + abs(m_XMax) ) / m_Step ; 
    
    setAxisScale(0,m_YMin,m_YMax);
    setAxisScale(2,m_XMin,m_XMax);
    setMouseTracking( true ); 
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(2);
    canvas()->setPalette(QPalette( Qt::white ));
    QwtPlotGrid *pGrid = new QwtPlotGrid() ; 
    pGrid->attach(this);
    replot();
}
void UI::FunctionPlot::addFunction( const QString & _equation ) {
    //clear();
    if ( m_pFunction.get() == NULL ) 
        m_pFunction.reset( new Math::Function2D( _equation.toStdString() ) ) ; 
    else
        m_pFunction->setEquation( _equation.toStdString() );
    m_pFunction->recalculateData( m_NumberOfValues , m_XMin , m_XMax , m_Step ) ;
    if ( m_pFunctionCurve.get() == NULL )
    {
        m_pFunctionCurve.reset ( new QwtPlotCurve( m_pFunction->equation().c_str() ) ) ;
        m_pFunctionCurve->setRawData( m_pFunction->xs() , m_pFunction->ys() , m_NumberOfValues );
        m_pFunctionCurve->attach( this );
    }
    replot();
}
void UI::FunctionPlot::replot() {
    // disable editing !!!!!!! 
    if ( m_pFunction.get() != NULL )
        m_pFunction->recalculateData( m_NumberOfValues , m_XMin , m_XMax , m_Step ) ;
    QwtPlot::replot();
}
void UI::FunctionPlot::setXMin( double _value ) { 
    //m_pTimer->stop();
    m_XMin = _value ;
    //m_pTimer->start() ; 
    m_NumberOfValues = (int) ( abs(m_XMin) + abs(m_XMax) ) / m_Step ; 
} 
void UI::FunctionPlot::setXMax( double _value ) { 
    m_XMax = _value ; 
    m_NumberOfValues = (int) ( abs(m_XMin) + abs(m_XMax) ) / m_Step ; 
} 
void UI::FunctionPlot::setYMin( double _value ) { 
    m_YMin = _value ; 
    setAxisScale(0,m_YMin,m_YMax);
    QwtPlot::replot();
} 
void UI::FunctionPlot::setYMax( double _value ) { 
    m_YMax = _value ; 
    setAxisScale(0,m_YMin,m_YMax);
    QwtPlot::replot();
}
