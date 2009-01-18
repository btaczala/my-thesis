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
#ifndef GENERALSETTINGSPAGE_H
#define GENERALSETTINGSPAGE_H

#include <QDialog>
#include "isettingspage.h"
/** 
 * @author Tomasz Czyzewski 
 * @author Bartek Taczala
 */

class QTabWidget;
class QLineEdit;
class QSpinBox;

namespace general_settings_tabs
{
    class DownloadTab : public QWidget
    {
        Q_OBJECT
    public:
        DownloadTab(QWidget* parent = 0);
        ~DownloadTab();
    public slots:
        void browseForDefaultFolder();
        void delayStateChanged(int state);
    private:
        QLineEdit* m_defaultFolderEdit;
        QSpinBox* m_downloadDelay;
    };

    class ApplicationTab : public QWidget
    {
        Q_OBJECT
    public:
        ApplicationTab(QWidget* parent = 0);
    public slots:
        void onClose2TrayCheck(int state);
        void onMinimize2TrayCheck(int state);
        void onConfirmAppExit(int state);
        void onStartInTray(int state);
        void onOneInstanceCheck(int state);
        void onConfirmItemDelete(int state);
        void onAutostartCheck(int state);
    };
//#ifdef _DEBUG
    class InfoTab : public QWidget
    {
        Q_OBJECT
    public:
        InfoTab ( QWidget *parent = 0 ) ; 
    };
//#endif
}

class GeneralSettingsPage : public ISettingsPage
{
    Q_OBJECT
public:
    GeneralSettingsPage(QWidget* parent = 0);

    virtual QIcon getIcon() const;
    virtual QString getTitle() const;

protected: 
    void initialize();

};


#endif
