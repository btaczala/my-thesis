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
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "isettingspage.h"

class QListWidget;
class QStackedWidget;
class QListWidgetItem;

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    SettingsDialog(QWidget* parent = 0);

    void addPage(ISettingsPage* page, bool activate = false);

public slots:
    void changePage(QListWidgetItem* current, QListWidgetItem* previous);

protected: 
    void initialize();

    struct DialogSettings
    {
        static const int ContentsWidgetIconWidth = 48;
        static const int ContentsWidgetIconHeigth = 48;
        static const int ContentsWidgetMinimumWidth = 94;
        static const int ContentsWidgetMaximumWidth = 94;
        static const int ContentsWidgetSpacing = 12;
        static const int MainLayoutColumnMinimumWidth = 360;
        static const int MainLayoutRowMinimumHeigth = 360;
    };

private:
    QListWidget* m_contentsWidget;
    QStackedWidget* m_pagesWidget;
};


#endif
