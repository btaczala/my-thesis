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

#ifndef STYLESHEETS_H
#define STYLESHEETS_H

#include <QString>
                          
namespace UI
{
    class StyleSheets
    {
        public:
            static const QString styleForFunctionLineEdit () ;
            static const QString styleForMainWindow () ;
            static const QString styleForMenuBar () ; 
        private:
            static const QString m_styleForFunctionLineEdit ;
            static const QString m_styleForMainWindow ;
            static const QString m_styleForMenuBar ; 
    };
}

#endif // STYLESHEETS_H
