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
#include "generalsettingspage.h"

GeneralSettingsPage::GeneralSettingsPage(QWidget *parent) 
    :ISettingsPage(parent)
{
    Initialize();
}

QIcon GeneralSettingsPage::getIcon() const
{
    return QIcon(":/settings.png");
}

QString GeneralSettingsPage::getTitle() const
{
    return tr("General");
}

void GeneralSettingsPage::Initialize()
{    


    m_tabWidget->addTab(new general_settings_tabs::DownloadTab, tr("Download"));
    m_tabWidget->addTab(new general_settings_tabs::ApplicationTab, tr("Application"));

    
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
        QPushButton* defaultFolderButton = new QPushButton(tr("Browse..."));

        connect(defaultFolderButton, SIGNAL(clicked()), this, SLOT(browseForDefaultFolder()));

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

        connect(delaydownloadCheck, SIGNAL(stateChanged(int)), this, SLOT(delayStateChanged(int)));

        QVBoxLayout* layout = new QVBoxLayout;
        layout->addSpacing(25);
        layout->addLayout(folderLayout);
        layout->addSpacing(12);
        layout->addWidget(autoDownloadCheck);
        layout->addSpacing(12);
        layout->addWidget(autoDownloadStartCheck);
        layout->addSpacing(12);
        layout->addWidget(removeDownloadedCheck);
        layout->addSpacing(12);
        layout->addLayout(delayLayout);
        layout->addStretch(1);

        setLayout(layout);
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
        QCheckBox* autostartCheck = new QCheckBox;
        autostartCheck->setText(tr("Start application on system startup"));

        QCheckBox* minimizeCheck = new QCheckBox;
        minimizeCheck->setText(tr("Move to tray icon when closed"));

        QCheckBox* systemtrayCheck = new QCheckBox;
        systemtrayCheck->setText(tr("Start in system tray area"));

        QCheckBox* oneAppCheck = new QCheckBox;
        oneAppCheck->setText(tr("Allow only 1 copy of application at a time"));

        QCheckBox* confirmExitCheck = new QCheckBox;
        confirmExitCheck->setText(tr("Confirm exit"));

        QCheckBox* confirmDeleteCheck = new QCheckBox;
        confirmDeleteCheck->setText(tr("Confirm item delete"));

        QVBoxLayout* layout = new QVBoxLayout;
        layout->addSpacing(25);
        layout->addWidget(autostartCheck);
        layout->addSpacing(12);
        layout->addWidget(minimizeCheck);
        layout->addSpacing(12);
        layout->addWidget(systemtrayCheck);
        layout->addSpacing(12);
        layout->addWidget(oneAppCheck);
        layout->addSpacing(12);
        layout->addWidget(confirmExitCheck);
        layout->addSpacing(12);
        layout->addWidget(confirmDeleteCheck);
        layout->addStretch(1);

        setLayout(layout);
    }
}
