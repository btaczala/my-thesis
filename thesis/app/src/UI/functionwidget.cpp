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

#include "functionwidget.h"
#include "functionedit.h"
#include "functionplot.h"

#include "../math/function.h"

///qt
#include <QToolButton>
#include <QVBoxLayout> 
#include <QGraphicsView>
#include <QMouseEvent>
#include <QListWidget>
#include <QMenu>
#include <QDebug>

/// qwt 
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

UI::FunctionWidget::FunctionWidget(QWidget* pParent) 
: QWidget( pParent ) 
{
    m_pLayout = new QVBoxLayout( this ) ; 
    m_pFunctionEditLine = new UI::FunctionEdit( this, "f(x) = ") ; 
    m_pLayout->addWidget(m_pFunctionEditLine);
    
    /// upper toolBox
    m_pToolButtonsWidget = new QWidget ( this ) ; 
    m_pToolButtonsWidgetLayout = new QHBoxLayout( m_pToolButtonsWidget );
    
    m_pRecentToolButton = new QToolButton( this ) ; 
    m_pRecentToolButton->setText("recent");
    m_pAddFunctionButton = new QToolButton( this ) ; 
    m_pAddFunctionButton->setText(tr("Add Function"));
    QMenu * pMenu = new QMenu ( m_pAddFunctionButton ) ; 
    pMenu->addAction("someaction");
    m_pAddFunctionButton->setMenu(pMenu);
    
    m_pToolButtonsWidgetLayout->addWidget(m_pRecentToolButton);
    m_pToolButtonsWidgetLayout->addWidget(m_pAddFunctionButton);
    m_pToolButtonsWidget->setLayout(m_pToolButtonsWidgetLayout);
    
    m_pLayout->addWidget( m_pToolButtonsWidget ) ; 
    
    m_pPlot = new UI::FunctionPlot(this);
//     m_pPlot->setAxisScale(0,ymin,ymax);
//     m_pPlot->setAxisScale(2,xmin,xmax);
    m_pLayout->addWidget( m_pPlot );
    
    // toolBox
    m_pToolboxWidget = new QWidget( this ) ; 
    QHBoxLayout * pLay = new QHBoxLayout(m_pToolboxWidget);
    pLay->addWidget(new QToolButton(m_pToolboxWidget));
    pLay->addWidget(new QToolButton(m_pToolboxWidget));
    m_pToolboxWidget->setLayout(pLay);
//     m_pToolboxWidget->hide();
//     hidden = true ; 
    m_pLayout->addWidget(m_pToolboxWidget);
    
    setLayout(m_pLayout);
    
    
//     m_apFunction.reset(new Math::Function2D());
//     m_apFunction->setEquation("sin(x)");
    connects() ; 
//     m_pFunctionEditLine->setFocus();
}
UI::FunctionWidget::~FunctionWidget() {
    disconnects(); 
}

void UI::FunctionWidget::connects() {
    connect ( m_pRecentToolButton, SIGNAL ( pressed()), this, SLOT(recentFunctions()) );
    connect ( m_pFunctionEditLine, SIGNAL( functionChanged(QString)), this, SLOT( functionChanged(QString)) ) ; 
}
void UI::FunctionWidget::disconnects() {

}
void UI::FunctionWidget::mouseMoveEvent(QMouseEvent* pEvent) {
    QWidget::mouseMoveEvent(pEvent);
}
void UI::FunctionWidget::mousePressEvent(QMouseEvent* pEvent) {    
    QWidget::mousePressEvent(pEvent);
}
void UI::FunctionWidget::mouseReleaseEvent(QMouseEvent* pEvent) {
    QWidget::mouseReleaseEvent(pEvent);
}

void UI::FunctionWidget::keyPressEvent(QKeyEvent* pEvent) {
    if ( pEvent->key() == Qt::Key_Enter ) 
    {
        
    }
    else
        QWidget::keyPressEvent(pEvent);
}

///slots
void UI::FunctionWidget::recentFunctions() {
    if ( m_pFunctionListWidget ) 
    {
        m_pFunctionListWidget = new QListWidget ( this ) ; 
    }
}
void UI::FunctionWidget::functionChanged(const QString& _equation) {
    m_pPlot->addFunction(new Math::Function2D(_equation.toStdString()) );
}
