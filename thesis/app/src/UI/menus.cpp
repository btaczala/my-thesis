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

/// mine
#include "menus.h"
#include "actions.h"
/// qt
#include <QMenu>
#include <QWidget>
#include <QMenuBar>

UI::Menus::Menus(QWidget* pParent) : m_pFileMenu( NULL ), m_pAboutMenu(NULL) {
    m_pFileMenu = new QMenu (QObject::tr("&File"), pParent ) ; 
    m_pFunctionMenu = new QMenu ( QObject::tr("F&unction"), pParent ) ; 
    m_pAboutMenu = new QMenu ( QObject::tr("&About"), pParent ) ; 
    addActions();
}
UI::Menus::~Menus() {
    ;
}
void UI::Menus::attachTo(QMenuBar* _pMenuBar) {
    _pMenuBar->addMenu(m_pFileMenu);
    _pMenuBar->addMenu(m_pFunctionMenu);
    _pMenuBar->addMenu(m_pAboutMenu);
}
void UI::Menus::addActions() {
    m_pFileMenu->addAction( UI::Actions::action("new_fun") ) ; 
    m_pFileMenu->addSeparator();
    m_pFileMenu->addAction( UI::Actions::action("quit") ) ; 
    
    /// function menu 
    m_pFileMenu->addAction( UI::Actions::action("calculate integral") );
}
void UI::Menus::disableFunctionMenu() {
    m_pFunctionMenu->setEnabled(false);
}
void UI::Menus::enableFunctionMenu() {
    m_pFunctionMenu->setEnabled(true);
}


