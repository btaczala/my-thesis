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

#include <QtGui>
#include <QFile>
#include "generalsettingspage.h"

#include <proxy.h>
#include <settings.h>

GeneralSettingsPage::GeneralSettingsPage(QWidget *parent) 
    :ISettingsPage(parent)
{
    initialize();
}

QIcon GeneralSettingsPage::getIcon() const
{
    return QIcon(":/settings.png");
}

QString GeneralSettingsPage::getTitle() const
{
    return tr("General");
}

void GeneralSettingsPage::initialize()
{    
    m_tabWidget->addTab(new general_settings_tabs::DownloadTab, tr("Download"));
    m_tabWidget->addTab(new general_settings_tabs::ApplicationTab, tr("Application"));
#ifdef _DEBUG
    m_tabWidget->addTab(new general_settings_tabs::InfoTab, tr("Debug information"));
#endif

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_tabWidget);
    mainLayout->setMargin(0);

    setLayout(mainLayout);
}


namespace general_settings_tabs
{
    DownloadTab::DownloadTab(QWidget *parent)
        :QWidget(parent)
    {
        QLabel* defaultFolderLabel = new QLabel(tr("Default download folder:"));
        m_defaultFolderEdit = new QLineEdit();
        QString defaultDir = Proxy::settings()->value(SettingsValNames::scDefaultDownloadDirectory).toString();
        if( defaultDir.isEmpty() )
        {
            defaultDir = QDir::homePath();
            Proxy::settings()->setValue(SettingsValNames::scDefaultDownloadDirectory, defaultDir);
        }
        m_defaultFolderEdit->setText(defaultDir);

        QPushButton* defaultFolderButton = new QPushButton(tr("Browse..."));

        QHBoxLayout* folderLayout = new QHBoxLayout;
        folderLayout->addWidget(defaultFolderLabel);
        folderLayout->addWidget(m_defaultFolderEdit);
        folderLayout->addWidget(defaultFolderButton);

        QCheckBox* autoDownloadCheck = new QCheckBox;
        autoDownloadCheck->setText(tr("Start download after add new link"));

        QCheckBox* autoDownloadStartCheck = new QCheckBox;
        autoDownloadStartCheck->setText(tr("Begin download after application start"));

        QCheckBox* removeDownloadedCheck = new QCheckBox;
        removeDownloadedCheck->setText(tr("Remove downloaded item form the list"));

        QCheckBox* delaydownloadCheck = new QCheckBox;
        delaydownloadCheck->setText(tr("Wait time beetween file downloads"));
        m_downloadDelay = new QSpinBox;
        m_downloadDelay->setRange(0, 9999);
        m_downloadDelay->setMaximumWidth(60);
        m_downloadDelay->setValue(0);
        m_downloadDelay->setEnabled(false);
        QLabel* secondsLabel = new QLabel(tr("seconds"));
        QHBoxLayout* delayLayout = new QHBoxLayout;
        delayLayout->addWidget(delaydownloadCheck);
        delayLayout->addWidget(m_downloadDelay);
        delayLayout->addWidget(secondsLabel);
        delayLayout->addStretch(1);

        QVBoxLayout* layout = new QVBoxLayout;
        layout->addSpacing(25);
        layout->addLayout(folderLayout);
        layout->addSpacing(settings_ui::SpaceBeetwenWidgets);
        layout->addWidget(autoDownloadCheck);
        layout->addSpacing(settings_ui::SpaceBeetwenWidgets);
        layout->addWidget(autoDownloadStartCheck);
        layout->addSpacing(settings_ui::SpaceBeetwenWidgets);
        layout->addWidget(removeDownloadedCheck);
        layout->addSpacing(settings_ui::SpaceBeetwenWidgets);
        layout->addLayout(delayLayout);
        layout->addStretch(1);

        connect(delaydownloadCheck, SIGNAL(stateChanged(int)), this, SLOT(delayStateChanged(int)));
        connect(defaultFolderButton, SIGNAL(clicked()), this, SLOT(browseForDefaultFolder()));

        setLayout(layout);
    }

    DownloadTab::~DownloadTab()
    {
        QString dir = m_defaultFolderEdit->text();
        if( QFile::exists( dir ))
        {
            Proxy::settings()->setValue( SettingsValNames::scDefaultDownloadDirectory, dir);
        }
    }
    void DownloadTab::browseForDefaultFolder()
    {
        QString dir = QFileDialog::getExistingDirectory(this, tr("Find default download folder..."));

        if (dir.isEmpty())
            return;

        m_defaultFolderEdit->setText(dir);
    }

    void DownloadTab::delayStateChanged(int state)
    {
        m_downloadDelay->setEnabled(state == Qt::Checked);
    }


    ApplicationTab::ApplicationTab(QWidget *parent)
        :QWidget(parent)
    {
        bool autostart = Proxy::settings()->value(SettingsValNames::scStartAppWithSystemStartup).toBool();
        QCheckBox* autostartCheck = new QCheckBox;
        autostartCheck->setText(tr("Start application on system startup"));
        autostartCheck->setChecked(autostart);
        connect(autostartCheck, SIGNAL(stateChanged(int)), this, SLOT(onAutostartCheck(int)));

        bool minimize2tray = Proxy::settings()->value(SettingsValNames::scMinimize2Tray).toBool();
        QCheckBox* minimize2TrayCheck = new QCheckBox;
        minimize2TrayCheck->setText(tr("Move to tray icon when minimized"));
        minimize2TrayCheck->setChecked(minimize2tray);
        connect(minimize2TrayCheck, SIGNAL(stateChanged(int)), this, SLOT(onMinimize2TrayCheck(int)));

        bool close2tray = Proxy::settings()->value(SettingsValNames::scClose2Tray).toBool();
        QCheckBox* close2TrayCheck = new QCheckBox;
        close2TrayCheck->setText(tr("Move to tray icon when closed - Tip: Press Shift key to force exit"));
        close2TrayCheck->setChecked(close2tray);
        connect(close2TrayCheck, SIGNAL(stateChanged(int)), this, SLOT(onClose2TrayCheck(int)));

        bool startInTray = Proxy::settings()->value(SettingsValNames::scStartInTrayArea).toBool();
        QCheckBox* startInTrayCheck = new QCheckBox;
        startInTrayCheck->setText(tr("Start in system tray area"));
        startInTrayCheck->setChecked(startInTray);
        connect(startInTrayCheck, SIGNAL(stateChanged(int)), this, SLOT(onStartInTray(int)));

        bool oneInstance = Proxy::settings()->value(SettingsValNames::scOnlyOneInstanceAllowed).toBool();
        QCheckBox* oneInstanceCheck = new QCheckBox;
        oneInstanceCheck->setText(tr("Allow only 1 copy of application at a time"));
        oneInstanceCheck->setChecked(oneInstance);
        connect(oneInstanceCheck, SIGNAL(stateChanged(int)), this, SLOT(onOneInstanceCheck(int)));

        bool confirmAppExit = Proxy::settings()->value(SettingsValNames::scConfirmAppExit).toBool();
        QCheckBox* confirmAppExitCheck = new QCheckBox;
        confirmAppExitCheck->setText(tr("Confirm application exit"));
        confirmAppExitCheck->setChecked(confirmAppExit);
        connect(confirmAppExitCheck, SIGNAL(stateChanged(int)), this, SLOT(onConfirmAppExit(int)));

        bool confirmItemDelete = Proxy::settings()->value(SettingsValNames::scConfirmItemDelete).toBool();
        QCheckBox* confirmDeleteCheck = new QCheckBox;
        confirmDeleteCheck->setText(tr("Confirm item delete"));
        confirmDeleteCheck->setChecked(confirmItemDelete);
        connect(confirmDeleteCheck, SIGNAL(stateChanged(int)), this, SLOT(onConfirmItemDelete(int)));

        QVBoxLayout* layout = new QVBoxLayout;
        layout->addSpacing(settings_ui::SpaceBeforeFirstWidget);
        layout->addWidget(autostartCheck);
        layout->addSpacing(settings_ui::SpaceBeetwenWidgets);
        layout->addWidget(minimize2TrayCheck);
        layout->addSpacing(settings_ui::SpaceBeetwenWidgets);
        layout->addWidget(close2TrayCheck);
        layout->addSpacing(settings_ui::SpaceBeetwenWidgets);
        layout->addWidget(startInTrayCheck);
        layout->addSpacing(settings_ui::SpaceBeetwenWidgets);
        layout->addWidget(oneInstanceCheck);
        layout->addSpacing(settings_ui::SpaceBeetwenWidgets);
        layout->addWidget(confirmAppExitCheck);
        layout->addSpacing(settings_ui::SpaceBeetwenWidgets);
        layout->addWidget(confirmDeleteCheck);
        layout->addStretch(1);

        setLayout(layout);
    }

    void ApplicationTab::onClose2TrayCheck(int state)
    {
        Proxy::settings()->setValue(SettingsValNames::scClose2Tray, static_cast<int>(state == Qt::Checked));
    }

    void ApplicationTab::onMinimize2TrayCheck(int state)
    {
        Proxy::settings()->setValue(SettingsValNames::scMinimize2Tray, static_cast<int>(state == Qt::Checked));
    }

    void ApplicationTab::onConfirmAppExit(int state)
    {
        Proxy::settings()->setValue(SettingsValNames::scConfirmAppExit, static_cast<int>(state == Qt::Checked));
    }

    void ApplicationTab::onStartInTray(int state)
    {
        Proxy::settings()->setValue(SettingsValNames::scStartInTrayArea, static_cast<int>(state == Qt::Checked));
    }

    void ApplicationTab::onOneInstanceCheck(int state)
    {
        Proxy::settings()->setValue(SettingsValNames::scOnlyOneInstanceAllowed, static_cast<int>(state == Qt::Checked));
    }

    void ApplicationTab::onConfirmItemDelete(int state)
    {
        Proxy::settings()->setValue(SettingsValNames::scConfirmItemDelete, static_cast<int>(state == Qt::Checked));
    }

    void ApplicationTab::onAutostartCheck(int state)
    {
        Proxy::settings()->setValue(SettingsValNames::scStartAppWithSystemStartup, static_cast<int>(state == Qt::Checked));
    }
    InfoTab::InfoTab(QWidget *parent ) : QWidget(parent)
    {
        QVBoxLayout* layout = new QVBoxLayout(this);
        QLabel *label = new QLabel(this);
        label->setText( Proxy::settings()->settingsDirectory() ) ; 
        layout->addSpacing(settings_ui::SpaceBeforeFirstWidget);
        layout->addWidget(label);
        layout->addSpacing(settings_ui::SpaceBeetwenWidgets);
        setLayout(layout);
    }
}
