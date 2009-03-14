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

#ifndef SETTINGS_H
#define SETTINGS_H
#include <QVariant>
#include <memory>
class QString ; 
class QSettings ; 
namespace Common{
    
    class Settings{
        friend class std::auto_ptr<Settings> ;
        enum Context{
            eGlobal = 0,
            eUi=1,
            eFunctions=2,
            eFunctionPlot=3
        };
        public:
            static QVariant value ( const QString & _key, Context _context = Settings::eGlobal) ; 
            static void setValue ( const QString & _key, const QVariant & _value, Context _context = Settings::eGlobal) ;
        private:
            static Settings * instance() ; 
            static QString context( Context _context ) ;
            Settings() ;
            ~Settings() ; 
            QSettings * m_pSettings ; 
    };
}

#endif // SETTINGS_H
