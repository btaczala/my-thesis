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
#include "adddownloaddialog.h"


AddDownloadDialog::AddDownloadDialog(QWidget* parent)
    : QDialog(parent)
{
    initialize();
}

AddDownloadDialog::~AddDownloadDialog()
{

}

void AddDownloadDialog::initialize()
{
    QGridLayout* layout = new QGridLayout;

    QVBoxLayout* urlLabelLayout = new QVBoxLayout;
    QLabel* urlLabel = new QLabel(tr("Download URL(s)"));
    urlLabel->setMaximumWidth(DialogSettings::UrlLabelMaxmimumWidth);
    urlLabelLayout->addWidget(urlLabel);
    urlLabelLayout->addStretch(1);
    
    layout->addLayout(urlLabelLayout, 0, 0);
    QTextEdit* urlEdit = new QTextEdit;
    urlEdit->setAcceptRichText(false);
    urlEdit->setAutoFormatting(QTextEdit::AutoNone);
    urlEdit->setLineWrapMode(QTextEdit::NoWrap);
    urlEdit->setMinimumWidth(DialogSettings::MinimumUrlTextEditWidth);
    layout->addWidget(urlEdit, 0, 1);

    QHBoxLayout* pasteLayout = new QHBoxLayout;
    QPushButton* pasteButton = new QPushButton(tr("Paste from clipboard"));
    pasteLayout->addStretch(1);
    pasteLayout->addWidget(pasteButton);
    layout->addLayout(pasteLayout, 1, 1);

    QHBoxLayout* folderLayout = new QHBoxLayout;
    QLabel* folderLabel = new QLabel(tr("Download Folder:"));
    folderLabel->setMaximumWidth(DialogSettings::FolderLabelMaxmimumWidth);
    layout->addWidget(folderLabel, 2, 0);
    QComboBox* folderCombo = new QComboBox;
    QPushButton* folderButton = new QPushButton(tr("..."));
    folderButton->setMaximumWidth(DialogSettings::FolderButtonMaxmimumWidth);
    folderLayout->addWidget(folderCombo);
    folderLayout->addWidget(folderButton);
    layout->addLayout(folderLayout, 2, 1);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    QPushButton* okButton = new QPushButton(tr("OK"));
    QPushButton* cancelButton = new QPushButton(tr("Cancel"));
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout, 3,1);

    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    setLayout(layout);
}