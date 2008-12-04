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

#include <QtGui>
#include "connectionsettingspage.h"

#include <proxy.h>
#include <settings.h>
#include <downloadmanager.h>

ConnectionSettingsPage::ConnectionSettingsPage(QWidget *parent) 
    :ISettingsPage(parent)
{
    Initialize();
}


QIcon ConnectionSettingsPage::getIcon() const
{
    return QIcon(":/network_settings.png");
}

QString ConnectionSettingsPage::getTitle() const
{
    return tr("Connection");
}

void ConnectionSettingsPage::Initialize()
{   
    m_tabWidget->addTab(new connection_settings_page::ConnectionTab, tr("Connection"));
    m_tabWidget->addTab(new connection_settings_page::ProxyTab, tr("Proxy"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_tabWidget);
    mainLayout->setMargin(0);

    setLayout(mainLayout);
}

namespace connection_settings_page
{
    ConnectionTab::ConnectionTab(QWidget *parent)
        : QWidget(parent)
    {
        const short int labelWidth = 120;

        QLabel* bandwidthLabel = new QLabel(tr("Bandwidth limit [kB]:"));
        bandwidthLabel->setMinimumWidth(labelWidth);
        QLineEdit* bandwidthEdit = new QLineEdit("0");
        QHBoxLayout* bandwidthLayout = new QHBoxLayout;
        bandwidthLayout->addWidget(bandwidthLabel);
        bandwidthLayout->addWidget(bandwidthEdit);
        bandwidthLayout->addWidget(new QLabel("(0 - unlimited)"));
        bandwidthLayout->addStretch(1);

        QLabel* downloadsLabel = new QLabel(tr("Simultanious downloads:"));
        downloadsLabel->setMinimumWidth(labelWidth);
        m_pDownloadsSpin.reset( new QSpinBox ) ; 
        m_pDownloadsSpin->setRange(1,999);
//         downloadsSpin->setValue(2);
        int v = Proxy::settings()->value( SettingsValNames::scMaxDownloads/*"MaxDownloads"*/,Settings::LIBRARY).value<int>() ;
        m_pDownloadsSpin->setValue ( v==0 ? 2 : v );
        m_pDownloadsSpin->setMaximumWidth(45);
        QHBoxLayout* downloadsLayout = new QHBoxLayout;
        downloadsLayout->addWidget(downloadsLabel);
        downloadsLayout->addWidget( m_pDownloadsSpin.get() );
        downloadsLayout->addStretch(1);

        QVBoxLayout* layout = new QVBoxLayout;
        layout->addSpacing(25);
        layout->addLayout(bandwidthLayout);
        layout->addSpacing(12);
        layout->addLayout(downloadsLayout);
        layout->addStretch(1);

        setLayout(layout);
    }
    ConnectionTab::~ConnectionTab()
    {
        Proxy::settings()->setValue( SettingsValNames::scMaxDownloads,m_pDownloadsSpin->value(),Settings::LIBRARY);
        Proxy::downloadManager()->update();
    }


    ProxyTab::ProxyTab(QWidget *parent)
        :QWidget(parent)
    {
        QCheckBox* useProxyCheck = new QCheckBox;
        useProxyCheck->setText(tr("Use proxy server"));

        const short int labelWidth = 80;

        QLabel* serverLabel = new QLabel(tr("Proxy server:"));
        serverLabel->setMinimumWidth(labelWidth);
        QLineEdit* serverEdit = new QLineEdit;
        QLabel* portLabel = new QLabel(tr("Port:"));
        QLineEdit* portEdit = new QLineEdit;
        portEdit->setMaximumWidth(60);
        QHBoxLayout* serverLayout = new QHBoxLayout;
        serverLayout->addWidget(serverLabel);
        serverLayout->addWidget(serverEdit);
        serverLayout->addWidget(portLabel);
        serverLayout->addWidget(portEdit);

        QLabel* userLabel = new QLabel(tr("Proxy user:"));
        userLabel->setMinimumWidth(labelWidth);
        QLineEdit* userEdit = new QLineEdit;
        QHBoxLayout* userLayout = new QHBoxLayout;
        userLayout->addWidget(userLabel);
        userLayout->addWidget(userEdit);
        userLayout->addStretch(1);

        QLabel* passwordLabel = new QLabel(tr("Proxy password:"));
        passwordLabel->setMinimumWidth(labelWidth);
        QLineEdit* passwordEdit = new QLineEdit;
        passwordEdit->setEchoMode(QLineEdit::Password);
        QHBoxLayout* passwordLayout = new QHBoxLayout;
        passwordLayout->addWidget(passwordLabel);
        passwordLayout->addWidget(passwordEdit);
        passwordLayout->addStretch(1);

        QVBoxLayout* layout = new QVBoxLayout;
        layout->addSpacing(25);
        layout->addWidget(useProxyCheck);
        layout->addSpacing(12);
        layout->addLayout(serverLayout);
        layout->addSpacing(12);
        layout->addLayout(userLayout);
        layout->addSpacing(12);
        layout->addLayout(passwordLayout);
        layout->addStretch(1);
        setLayout(layout);
    }
}

