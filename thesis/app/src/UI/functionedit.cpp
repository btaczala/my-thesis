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

#include "functionedit.h"
#include <QKeyEvent>
#include <QDebug>
#include "stylesheets.h"

UI::FunctionEdit::FunctionEdit(QWidget* pParent, const QString & _prefix ) 
: QLineEdit( pParent ) ,
  m_Prefix( _prefix )
{
    setText( );
    setStyleSheet( UI::StyleSheets::styleForFunctionLineEdit() );
}
void UI::FunctionEdit::init() {
    connect ( this, SIGNAL(textChanged(QString)),this,SLOT(textChange(QString)) );
}
void UI::FunctionEdit::addVariable(const QChar & _var) {
    if ( !m_Variables.contains(_var) )
        m_Variables.append(_var);
}
bool UI::FunctionEdit::isVariable(int key) {
    return ( m_Variables.contains(key) );
}
void UI::FunctionEdit::keyPressEvent(QKeyEvent* pEvent) {
//     if ( isVariable(pEvent->key() ) )
//     {
//         // light up
//     }
    if ( pEvent->key() == Qt::Key_Return )
    {
        emit ( functionChanged(text()) ) ; 
    }
    QLineEdit::keyPressEvent(pEvent);
}
void UI::FunctionEdit::textChange(const QString& _newText) {
    for ( int i = 0 ; i < _newText.size() ; ++i ) 
    {
        if ( isVariable( _newText.at(i).toLatin1() ) )
            // light up
            ;
    }
}
void UI::FunctionEdit::setText(const QString& _text) {
    m_FunctionEquation = _text ; 
    QLineEdit::setText(m_Prefix + m_FunctionEquation );
}
QString UI::FunctionEdit::text() const {
    QString ret = QLineEdit::text() ; 
    ret.remove(m_Prefix);
    return ret.trimmed();
}
