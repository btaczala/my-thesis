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

#ifndef FUNCTIONEDIT_H
#define FUNCTIONEDIT_H

///qt 
#include <QLineEdit>
#include <QStringList>
class QKeyEvent ; 
namespace UI
{
    class FunctionEdit : public QLineEdit 
    {
        Q_OBJECT
        public:
            FunctionEdit( QWidget *pParent, const QString & _prefix = "" )  ; 
            void addVariable ( const QChar & _var ) ; 
            
            inline void setPrefix( const QString & _prefix ) { m_Prefix = _prefix ;} 
            inline QString prefix() const { return m_Prefix ; }
            void setText( const QString & _text ="") ; 
            virtual QString text() const  ;
        protected:
            virtual void keyPressEvent ( QKeyEvent * pEvent ) ; 
        private:
            ///methods
            void init() ; 
            bool isVariable ( int key ) ; 
            /// fields
            QList<QChar> m_Variables ; 
            QString m_Prefix ; 
            QString m_FunctionEquation ; 
        private slots:
            void textChange( const QString & _newText ) ; 
        signals:
            void functionChanged ( const QString & _equation ) ;
    };
}

#endif // FUNCTIONEDIT_H
