/***************************************************************************
 *   Copyright (C) 2008 by Tomasz Czyzewski                         *
 *   tomasz.czy@gmail.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CONNECTIONSETTINGSPAGE_H
#define CONNECTIONSETTINGSPAGE_H

#include "isettingspage.h"
#include <memory>

class QSpinBox;
namespace connection_settings_page
{
    class ConnectionTab : public QWidget
    {
        Q_OBJECT
    public:
        ConnectionTab(QWidget* parent = 0);
        ~ConnectionTab();
    private:
        std::auto_ptr<QSpinBox> m_pDownloadsSpin;
    };

    class ProxyTab : public QWidget
    {
        Q_OBJECT
    public:
        ProxyTab(QWidget* parent = 0);
    };
}

class ConnectionSettingsPage : public ISettingsPage
{
    Q_OBJECT
public:
    ConnectionSettingsPage(QWidget* parent = 0);
    

    virtual QIcon getIcon() const;
    virtual QString getTitle() const;

protected: 
    void Initialize();
};


#endif
