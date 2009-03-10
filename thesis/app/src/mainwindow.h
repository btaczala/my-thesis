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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//Qt includes
#include <QMainWindow>
#include <QPointer>
//boost includes 

/// fwd 
class QMenu ;
class QHBoxLayout  ; 
class QMdiArea ; 
class QKeyEvent; 
namespace UI
{ class Menus ; }
class MainWindow : public QMainWindow {
    Q_OBJECT
    public:
        MainWindow (QWidget *pParent = NULL ) ; 
        virtual ~MainWindow() ; 
    private:
        /// ui methods 
        void createUI() ;
        void connectToActions();
        
        /// fields
        UI::Menus * m_pMenus ;
        QHBoxLayout *m_pLayout ;
        QPointer<QMdiArea> m_pMdiWidget ; 
        
    protected:
        virtual void keyPressEvent ( QKeyEvent *pEvent ) ; 
    private slots:
        void initUI() ; 
        void addSubWindow();
        void mouseChangedInFunctionPlot( double _x, double _y ) ; 
};

#endif // MAINWINDOW_H
