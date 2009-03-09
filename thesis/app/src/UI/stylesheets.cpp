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

#include "stylesheets.h"

const QString UI::StyleSheets::m_styleForFunctionLineEdit = " \
    QLineEdit { \
        border: 2px solid gray; \
        border-radius: 10px; \
        padding: 0 8px; \
        background: yellow; \
        selection-background-color: darkgray; \
    }";
 
const QString UI::StyleSheets::m_styleForMainWindow =" \
    QMainWindow{\
        background-color: rgb(255,0,110)\
    }\
    QMainWindow::separator {\
        background: yellow;\
        width: 10px; /* when vertical */\
        height: 10px; /* when horizontal */\
    }\
    QMainWindow::separator:hover {\
        background: red;\
    }";
const QString UI::StyleSheets::m_styleForMenuBar = "\
    QMenuBar {\
        background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0,\
                            stop:0 lightgreen, stop:1 darkgreen);\
    }\
    QMenuBar::item {\
        spacing: 3px; \
        padding: 1px 4px;\
        background: transparent;\
        border-radius: 4px;\
     }\
     QMenuBar::item:selected { \
         background: #a8a8a8;\
     }\
    QMenuBar::item:pressed { \
        background: #888888;\
    }"; 
const QString UI::StyleSheets::styleForFunctionLineEdit() {
    return m_styleForFunctionLineEdit ; 
}
const QString UI::StyleSheets::styleForMainWindow() {
    return /*m_styleForMainWindow */"";
}
const QString UI::StyleSheets::styleForMenuBar() {
    return /*m_styleForMenuBar*/ "";
}


