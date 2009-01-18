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
        m_pDownloadsSpin = new QSpinBox; 
        m_pDownloadsSpin->setRange(1,999);
        int v = Proxy::settings()->value( SettingsValNames::scMaxDownloads,Settings::LIBRARY).value<int>() ;
        m_pDownloadsSpin->setValue ( v==0 ? 2 : v );
        m_pDownloadsSpin->setMaximumWidth(45);
        QHBoxLayout* downloadsLayout = new QHBoxLayout;
        downloadsLayout->addWidget(downloadsLabel);
        downloadsLayout->addWidget( m_pDownloadsSpin );
        downloadsLayout->addStretch(1);

        QVBoxLayout* layout = new QVBoxLayout;
        layout->addSpacing(settings_ui::SpaceBeforeFirstWidget);
        layout->addLayout(bandwidthLayout);
        layout->addSpacing(settings_ui::SpaceBeetwenWidgets);
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
        bool useAppProxy = Proxy::settings()->value(SettingsValNames::scUseAppProxy).toBool();
        QString proxyServer = Proxy::settings()->value(SettingsValNames::scProxyServer).toString();
        QString proxyPort = Proxy::settings()->value(SettingsValNames::scProxyPort).toString();
        QString proxyUser = Proxy::settings()->value(SettingsValNames::scProxyUser).toString();
        QString proxyPass = Proxy::settings()->value(SettingsValNames::scProxyPass).toString();

        if (!proxyPass.isEmpty())
            proxyPass = Proxy::decrypt(proxyPass);

        m_useProxyCheck = new QCheckBox;
        m_useProxyCheck->setText(tr("Use proxy server"));
        m_useProxyCheck->setChecked(useAppProxy);
        connect(m_useProxyCheck, SIGNAL(stateChanged(int)), this, SLOT(useProxyStateChanged(int)));

        QLabel* serverLabel = new QLabel(tr("Proxy server:"));
        serverLabel->setMinimumWidth(DialogSettings::ServerLabelMinimumWidth);
        m_serverEdit = new QLineEdit;
        m_serverEdit->setText(proxyServer);
        m_serverEdit->setEnabled(useAppProxy);
        QLabel* portLabel = new QLabel(tr("Port:"));
        m_portEdit = new QLineEdit;
        m_portEdit->setText(proxyPort);
        m_portEdit->setEnabled(useAppProxy);
        m_portEdit->setMaximumWidth(DialogSettings::PortEditMaximumWidth);
        QHBoxLayout* serverLayout = new QHBoxLayout;
        serverLayout->addWidget(serverLabel);
        serverLayout->addWidget(m_serverEdit);
        serverLayout->addWidget(portLabel);
        serverLayout->addWidget(m_portEdit);

        QLabel* userLabel = new QLabel(tr("Proxy user:"));
        userLabel->setMinimumWidth(DialogSettings::UserLabelMinimumWidth);
        m_userEdit = new QLineEdit;
        m_userEdit->setText(proxyUser);
        m_userEdit->setEnabled(useAppProxy);
        QHBoxLayout* userLayout = new QHBoxLayout;
        userLayout->addWidget(userLabel);
        userLayout->addWidget(m_userEdit);
        userLayout->addStretch(1);

        QLabel* passwordLabel = new QLabel(tr("Proxy password:"));
        passwordLabel->setMinimumWidth(DialogSettings::PasswordLabelMinimumWidth);
        m_passwordEdit = new QLineEdit;
        m_passwordEdit->setText(proxyPass);
        m_passwordEdit->setEnabled(useAppProxy);
        m_passwordEdit->setEchoMode(QLineEdit::Password);
        QHBoxLayout* passwordLayout = new QHBoxLayout;
        passwordLayout->addWidget(passwordLabel);
        passwordLayout->addWidget(m_passwordEdit);
        passwordLayout->addStretch(1);

        QVBoxLayout* layout = new QVBoxLayout;
        layout->addSpacing(settings_ui::SpaceBeforeFirstWidget);
        layout->addWidget(m_useProxyCheck);
        layout->addSpacing(settings_ui::SpaceBeetwenWidgets);
        layout->addLayout(serverLayout);
        layout->addSpacing(settings_ui::SpaceBeetwenWidgets);
        layout->addLayout(userLayout);
        layout->addSpacing(settings_ui::SpaceBeetwenWidgets);
        layout->addLayout(passwordLayout);
        layout->addStretch(1);
        setLayout(layout);
    }

    ProxyTab::~ProxyTab()
    {
        Proxy::settings()->setValue(SettingsValNames::scProxyServer, m_serverEdit->text());
        Proxy::settings()->setValue(SettingsValNames::scProxyPort, m_portEdit->text());
        Proxy::settings()->setValue(SettingsValNames::scProxyUser, m_userEdit->text());
        Proxy::settings()->setValue(SettingsValNames::scProxyPass, Proxy::encrypt(m_passwordEdit->text()));
    }

    void ProxyTab::useProxyStateChanged(int state)
    {
        bool enabled = state == Qt::Checked;

        Proxy::settings()->setValue(SettingsValNames::scUseAppProxy, enabled);

        m_serverEdit->setEnabled(enabled);
        m_portEdit->setEnabled(enabled);
        m_userEdit->setEnabled(enabled);
        m_passwordEdit->setEnabled(enabled);
    }
}

