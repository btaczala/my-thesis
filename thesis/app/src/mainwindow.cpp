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

#include "mainwindow.h"
#include "UI/menus.h"
#include "UI/actions.h"
#include "UI/functionwidget.h"
#include "UI/stylesheets.h"
///qt 
#include <QTimer>
#include <QAction>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QKeyEvent>



MainWindow::MainWindow(QWidget* pParent) 
: QMainWindow ( pParent ), m_pMenus( NULL ) 
{
    m_pMdiWidget = new QMdiArea(this);
    QTimer::singleShot(100,this,SLOT(initUI()));
}
MainWindow::~MainWindow() {
}
void MainWindow::initUI() {
    m_pMenus = new UI::Menus( this ) ; 
    m_pMenus->attachTo( menuBar() );
    connectToActions();
    
    m_pMdiWidget->setViewMode(QMdiArea::TabbedView);
    m_pMdiWidget->setTabShape(QTabWidget::Triangular);
    setCentralWidget(m_pMdiWidget);
    setWindowState( Qt::WindowMaximized );
//     setStyleSheet( UI::StyleSheets::styleForMainWindow() );
//     menuBar()->setStyleSheet( UI::StyleSheets::styleForMenuBar() ) ; 
}
void MainWindow::connectToActions(){
    connect ( UI::Actions::action("quit") , SIGNAL(triggered()) , this , SLOT( close() ) ) ;
    connect ( UI::Actions::action("new_fun") , SIGNAL(triggered()) , this , SLOT( addSubWindow() ) ) ;
}
void MainWindow::addSubWindow() {
    QWidget *pWidget = new QWidget( this ) ; 
    m_pLayout = new QHBoxLayout( pWidget );
    UI::FunctionWidget *pFunctionWidget = new UI::FunctionWidget( pWidget );
    m_pLayout->addWidget(pFunctionWidget);
    connect ( pFunctionWidget, SIGNAL(pickerMouseSelected(double,double)),this,SLOT(mouseChangedInFunctionPlot(double,double)));
//     m_pLayout->addWidget(new UI::FunctionWidget( pWidget ) );
    pWidget->setLayout(m_pLayout);
    pWidget->setAttribute( Qt::WA_DeleteOnClose, true );
    m_pMdiWidget->addSubWindow(pWidget);
    pWidget->show();
}
void MainWindow::keyPressEvent(QKeyEvent* pEvent) {
//     if ( pEvent->key() == Qt::Key_F 
    QWidget::keyPressEvent(pEvent);
}
void MainWindow::mouseChangedInFunctionPlot(double _x, double _y ) 
{
    QString txt = "(" + QString::number(_x) + "," + QString::number(_y) + ")";
    statusBar()->showMessage(txt);
}