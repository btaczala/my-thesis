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

#ifndef FUNCTIONWIDGET_H
#define FUNCTIONWIDGET_H

///qt
#include <QWidget>
#include <QPointer>

#include <memory>


class QLineEdit ; 
class QToolButton ; 
class QVBoxLayout ; 
class QHBoxLayout;
class QGraphicsView ; 
class QMouseEvent ; 
class QListWidget ; 

class QwtPlot ; 
namespace Math
{
    class Function2D ; 
}
namespace UI
{
    class FunctionEdit ; 
    class FunctionPlot ; 
    class FunctionWidget : public QWidget
    {
        Q_OBJECT
        public:
            FunctionWidget (QWidget *pParent ) ;
            virtual ~FunctionWidget () ;
        protected:
            virtual void mouseMoveEvent( QMouseEvent *pEvent ) ; 
            virtual void mousePressEvent( QMouseEvent *pEvent ) ;
            virtual void mouseReleaseEvent( QMouseEvent *pEvent ) ; 
            
            virtual void keyPressEvent ( QKeyEvent * pEvent ) ; 
        private:
            ///methods
            void connects() ; 
            void disconnects() ; 
            ///fields
            UI::FunctionEdit * m_pFunctionEditLine ; 
            /// tool buttons 
            QPointer<QWidget> m_pToolButtonsWidget ; 
            QPointer<QHBoxLayout> m_pToolButtonsWidgetLayout ; 
            QPointer<QToolButton>  m_pRecentToolButton ;
            QPointer<QToolButton>  m_pAddFunctionButton ;
            /// 
            //QPointer<QGraphicsView> m_pFunctionPlot ; 
            QPointer<FunctionPlot> m_pPlot ; 
            QPointer<QVBoxLayout> m_pLayout ; 
            QPointer<QWidget> m_pToolboxWidget ; 
            QPointer<QListWidget> m_pFunctionListWidget ;  
            
            
            /// function!
            std::auto_ptr<Math::Function2D> m_apFunction ; 
            
        ///slots
        private slots:
            void recentFunctions() ; 
            void functionChanged( const QString & _equation ) ; 
    };
}
#endif // FUNCTIONWIDGET_H
