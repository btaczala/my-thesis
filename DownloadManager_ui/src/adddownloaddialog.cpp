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
#include "mainwindow.h"
#include <downloadmanager.h>
#include <proxy.h>
#include <settings.h>

const QColor AddDownloadDialog::DialogSettings::invalidItemBackgroundColor = QColor(0xef, 0x29, 0x29);

AddDownloadDialog::AddDownloadDialog(QWidget* parent)
    : QDialog(parent)
{
    initialize();
}

AddDownloadDialog::~AddDownloadDialog()
{
    QStringList folders;

    if (m_folderCombo->count() <= 0)
        return;

    for(int i = 0; i < m_folderCombo->count(); ++i)
    {
        folders << m_folderCombo->itemText(i);
    }

    Proxy::settings()->setValue(SettingsValNames::scRecentDownloadFolders, folders, Settings::LIBRARY);
}

void AddDownloadDialog::accept()
{
    QString str = m_urlEdit->toPlainText();
    QString folder = m_folderCombo->currentText();

    if (str.isEmpty())
    {
        markWidgetContentAsInvalid(m_urlEdit);
        return;
    }

    if (folder.isEmpty())
    {
        markWidgetContentAsInvalid(m_folderCombo);
        return;
    }
    QStringList urls = str.split(QRegExp("\\s+"));

    Q_FOREACH(QString url, urls)
    {
        qobject_cast<MainWindow*>(parent())->addDownload(url, folder);
    }

    QDialog::accept();
}

void AddDownloadDialog::resetItemBackground()
{
    if (m_urlEdit->hasFocus())
        resetItemBackground(m_urlEdit);
    else if (m_folderCombo->hasFocus())
        resetItemBackground(m_folderCombo);
}

void AddDownloadDialog::initialize()
{
    QGridLayout* layout = new QGridLayout;

    QVBoxLayout* urlLabelLayout = new QVBoxLayout;
    QLabel* urlLabel = new QLabel(tr("Download URL(s)"));
    urlLabelLayout->addWidget(urlLabel);
    urlLabelLayout->addStretch(1);    
    layout->addLayout(urlLabelLayout, 0, 0);

    m_urlEdit = new QTextEdit;
    m_urlEdit->setAcceptRichText(false);
    m_urlEdit->setAutoFormatting(QTextEdit::AutoNone);
    m_urlEdit->setLineWrapMode(QTextEdit::NoWrap);
    m_urlEdit->setMinimumWidth(DialogSettings::MinimumUrlTextEditWidth);
    layout->addWidget(m_urlEdit, 0, 1);

    QHBoxLayout* pasteLayout = new QHBoxLayout;
    QPushButton* pasteButton = new QPushButton(tr("Paste from clipboard"));
    pasteLayout->addStretch(1);
    pasteLayout->addWidget(pasteButton);
    layout->addLayout(pasteLayout, 1, 1);

    QHBoxLayout* folderLayout = new QHBoxLayout;
    QLabel* folderLabel = new QLabel(tr("Download Folder:"));
    layout->addWidget(folderLabel, 2, 0);
    m_folderCombo = new QComboBox;
    m_folderCombo->setEditable(true);
    QCompleter* completer = new QCompleter;
    completer->setModel(new QDirModel(completer));
    m_folderCombo->setCompleter(completer);
    m_folderCombo->setInsertPolicy(QComboBox::InsertAtCurrent);
    QPushButton* folderButton = new QPushButton(tr("..."));
    folderButton->setMaximumWidth(DialogSettings::FolderButtonMaxmimumWidth);
    folderLayout->addWidget(m_folderCombo);
    folderLayout->addWidget(folderButton);
    layout->addLayout(folderLayout, 2, 1);

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    QPushButton* okButton = new QPushButton(tr("OK"));
    okButton->setDefault(true);
    QPushButton* cancelButton = new QPushButton(tr("Cancel"));
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout, 3,1);

    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    connect(pasteButton, SIGNAL(clicked()), m_urlEdit, SLOT(paste()));
    connect(folderButton, SIGNAL(clicked()), this, SLOT(browseForDownloadFolder()));

    connect(m_urlEdit, SIGNAL(textChanged()), this, SLOT(resetItemBackground()));
    connect(m_folderCombo, SIGNAL(editTextChanged(const QString&)), this, SLOT(resetItemBackground()));

    setLayout(layout);

    pasteButton->click();

    QStringList folders = Proxy::settings()->value(SettingsValNames::scRecentDownloadFolders, Settings::LIBRARY).toStringList();

    QString defaultDir = Proxy::settings()->value(SettingsValNames::scDefaultDownloadDirectory).toString();
    if (!defaultDir.isEmpty() && !folders.contains(defaultDir))
        m_folderCombo->addItem(defaultDir);

    Q_FOREACH(QString one, folders)
    {
        m_folderCombo->addItem(one);
    }

    QTextCursor cursor = m_urlEdit->textCursor();
    cursor.setPosition(QTextCursor::Start);
    m_urlEdit->setTextCursor(cursor);
}


void AddDownloadDialog::markWidgetContentAsInvalid(QWidget* widget)
{
    QPalette p = widget->palette();
    p.setColor(QPalette::Base, DialogSettings::invalidItemBackgroundColor);
    widget->setPalette(p);
}

void AddDownloadDialog::resetItemBackground(QWidget *widget)
{
    QPalette p = widget->palette();
    p.setColor(QPalette::Base, QApplication::palette().color(QPalette::Base));
    widget->setPalette(p);
}

void AddDownloadDialog::browseForDownloadFolder()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Find download folder..."));

    if (dir.isEmpty())
        return;

    int item = m_folderCombo->findText(dir);

    if (item != -1)
    {
        m_folderCombo->setCurrentIndex(item);
        return;
    }

    m_folderCombo->addItem(dir);
    m_folderCombo->setCurrentIndex(m_folderCombo->count()-1);
}
