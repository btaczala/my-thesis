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

#include "settings.h"

#include <QString>
#include <QSettings> 
#include <QDir>
Common::Settings* Common::Settings::instance() {
    static std::auto_ptr<Settings> apRet ( new Settings( ) ) ; 
    return apRet.get() ; 
}
Common::Settings::Settings() {
    QString fileName = QDir::currentPath() ; 
    fileName +="/thesis.ini";
    m_pSettings = new QSettings (fileName, QSettings::IniFormat, NULL ) ; 
}
Common::Settings::~Settings() {
    m_pSettings->sync();
    delete m_pSettings ; 
}
void Common::Settings::setValue(const QString& _key, const QVariant& _value, Common::Settings::Context _context) {
    Settings *pSettings = Settings::instance() ; 
    if ( _context != Common::Settings::eGlobal ) {
        pSettings->m_pSettings->beginGroup(Settings::context(_context));
    }
    pSettings->m_pSettings->setValue(_key,_value);
    if ( _context != Common::Settings::eGlobal ) {
        pSettings->m_pSettings->endGroup();
    }
}
QString Common::Settings::context(Common::Settings::Context _context) {
    if ( _context == Common::Settings::eGlobal ) 
        return ""; 
    else if (_context == Common::Settings::eUi ) 
        return "Ui";
    else if ( _context == Common::Settings::eFunctions ) 
        return "Functions";
    else if ( _context == Common::Settings::eFunctionPlot ) 
        return "FunctionPlot";
}
