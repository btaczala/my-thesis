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

#include "actions.h"
///qt
#include <QAction>
///boost

///standard
#include <memory>

UI::Actions::Actions() {
    createDefault();
}
UI::Actions* UI::Actions::instance() {
    static std::auto_ptr<UI::Actions> apRet( new UI::Actions() ) ; 
    return apRet.get();
}
void UI::Actions::createDefault() {
    QAction * pAction = new QAction(QIcon(":/ui/quit.png"), QObject::tr("&Quit"), NULL ) ; 
    pAction->setShortcut(QKeySequence("Ctrl+Q"));
    m_Actions["quit"] = boost::shared_ptr<QAction> ( pAction ) ; 
    
    pAction = new QAction ( QIcon(":/ui/new_fun.png"),QObject::tr("&New function"),NULL ) ; 
    pAction->setShortcut(QKeySequence("Ctrl+N"));
    m_Actions["new_fun"] = boost::shared_ptr<QAction> ( pAction ) ; 
}
QAction* UI::Actions::action(const QString& _actionName) {
    UI::Actions *pActions = instance() ; 
    return pActions->m_Actions[_actionName].get();
}
