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
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) 
    :QDialog(parent)
{
    Initialize();
}


void SettingsDialog::Initialize()
{
    m_contentsWidget = new QListWidget;
    m_contentsWidget->setViewMode(QListView::IconMode);
    m_contentsWidget->setIconSize(QSize(96, 84));
    m_contentsWidget->setMovement(QListView::Static);
    m_contentsWidget->setMaximumWidth(128);
    m_contentsWidget->setSpacing(12);

    m_pagesWidget = new QStackedWidget;

    QPushButton* closeButton = new QPushButton(tr("Close"));

    m_contentsWidget->setCurrentRow(0);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(m_contentsWidget);
    horizontalLayout->addWidget(m_pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Config Dialog"));

    connect(m_contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));

}


void SettingsDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    m_pagesWidget->setCurrentIndex(m_contentsWidget->row(current));
}

void SettingsDialog::AddPage(ISettingsPage* page, bool activate)
{
    QListWidgetItem* item = new QListWidgetItem(m_contentsWidget);
    item->setIcon(page->getIcon());
    item->setText(page->getTitle());
    item->setTextAlignment(Qt::AlignHCenter);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    m_pagesWidget->addWidget(page);
    
    if (activate)
    {
        m_pagesWidget->setCurrentIndex(m_contentsWidget->row(item));
    }
}
