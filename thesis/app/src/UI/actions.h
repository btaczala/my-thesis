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

#ifndef UI_ACTIONS_H
#define UI_ACTIONS_H

//qt
#include <QMap>
//boost
#include <boost/shared_ptr.hpp>

class QAction;

namespace UI {

    class Actions
    {
        public:
            static QAction * action( const QString & _actionName ) ; 
        private:
            Actions() ; 
            /// methods
            static Actions *instance() ; 
            void createDefault() ; 
            ///fields
            QMap<QString,boost::shared_ptr<QAction> > m_Actions ; 
    };
}

#endif // UI_ACTIONS_H
