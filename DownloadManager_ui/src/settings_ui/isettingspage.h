/***************************************************************************
 *   Copyright (C) 2008 by Tomasz Czyzewski  						*
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
#ifndef ISETTINGSPAGE_H
#define ISETTINGSPAGE_H

#include <QWidget>
#include <QTabWidget>
#include <QBoxLayout>

namespace settings_ui
{
    static const int SpaceBeforeFirstWidget = 25;
    static const int SpaceBeetwenWidgets = 12;
}

class ISettingsPage : public QWidget
{
    Q_OBJECT
public:
    ISettingsPage(QWidget* parent = 0) : QWidget(parent) 
    {
        m_tabWidget = new QTabWidget(this);
    }

    virtual QIcon getIcon() const = 0;
    virtual QString getTitle() const = 0;

protected:
    QTabWidget* m_tabWidget;

};

#endif
