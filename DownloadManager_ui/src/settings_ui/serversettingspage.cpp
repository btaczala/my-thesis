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
#include "serversettingspage.h"

#include <proxy.h>
#include <settings.h>
#include <optionscontainer.h>

ServerSettingsPage::ServerSettingsPage(QWidget *parent) 
    :ISettingsPage(parent)
{
    Initialize();
}

QIcon ServerSettingsPage::getIcon() const
{
    return QIcon(":/server_settings.png");
}

QString ServerSettingsPage::getTitle() const
{
    return tr("Servers\nsettings");
}

void ServerSettingsPage::Initialize()
{   
    m_tabWidget->addTab(new server_settings_page::ServerTab, tr("Download servers"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_tabWidget);
    mainLayout->setMargin(0);

    setLayout(mainLayout);
}

namespace server_settings_page
{
    ServerTab::ServerTab(QWidget *parent)
        : QWidget(parent)
    {
        
        const short int labelWidth = 80;

        QComboBox* enginesCombo = new QComboBox;
        enginesCombo->setEditable(false);
        enginesCombo->setInsertPolicy(QComboBox::InsertAlphabetically);
        enginesCombo->addItem("Rapidshare");
        
        QLabel* downloadsLabel = new QLabel(tr("Simultanious downloads:"));
        downloadsLabel->setMinimumWidth(labelWidth);
        QSpinBox* downloadsSpin = new QSpinBox;
        downloadsSpin->setRange(1,999);
        downloadsSpin->setValue(2);
        downloadsSpin->setMaximumWidth(45);
        QHBoxLayout* downloadsLayout = new QHBoxLayout;
        downloadsLayout->addWidget(downloadsLabel);
        downloadsLayout->addWidget(downloadsSpin);
        downloadsLayout->addStretch(1);

        QCheckBox* credentialsCheck = new QCheckBox(tr("Use credentials"));
        
        connect(credentialsCheck,SIGNAL( stateChanged ( int ) ), this, SLOT( useCredentialChecked( int ) ) );

        QLabel* userLabel = new QLabel(tr("User:"));
        userLabel->setMinimumWidth(labelWidth);
        m_UserEdit = new QLineEdit;
        QHBoxLayout* userLayout = new QHBoxLayout;
        userLayout->addWidget(userLabel);
        userLayout->addWidget(m_UserEdit);
        userLayout->addStretch(1);

        QLabel* passwordLabel = new QLabel(tr("Password:"));
        passwordLabel->setMinimumWidth(labelWidth);
        m_PasswordEdit = new QLineEdit;
        m_PasswordEdit->setDisabled(true);
        m_PasswordEdit->setEchoMode(QLineEdit::Password);
        QHBoxLayout* passwordLayout = new QHBoxLayout;
        passwordLayout->addWidget(passwordLabel);
        passwordLayout->addWidget(m_PasswordEdit);
        passwordLayout->addStretch(1);

        QVBoxLayout* layout = new QVBoxLayout;
        layout->addSpacing(25);
        layout->addWidget(enginesCombo);
        layout->addSpacing(20);
        layout->addLayout(downloadsLayout);
        layout->addSpacing(12);
        layout->addWidget(credentialsCheck);
        layout->addSpacing(6);
        layout->addLayout(userLayout);
        layout->addSpacing(6);
        layout->addLayout(passwordLayout);
        layout->addStretch(1);
        setLayout(layout);

         std::string userName = boost::any_cast<std::string>( Proxy::settings()->optionsForEngine("rapidshare")->option("username") );
        std::string userPass = boost::any_cast<std::string>( Proxy::settings()->optionsForEngine("rapidshare")->option("password") ) ; 
        if ( userName.empty() || userPass.empty() ) 
            credentialsCheck->setDisabled(true);
        else
        {
            credentialsCheck->setDisabled(false);
            m_UserEdit->setText( userName.c_str() ) ; 
            m_PasswordEdit->setText( userPass.c_str() ) ; 
        }
        

    }
    void ServerTab::useCredentialChecked(int state ) 
    {
        if ( state == Qt::Unchecked ) 
        {
            m_UserEdit->setDisabled(true);
            m_PasswordEdit->setDisabled(true);
        }
        if ( state == Qt::Checked ) 
        {
            m_UserEdit->setDisabled(false);
            m_PasswordEdit->setDisabled(false);
        }   
    }
}