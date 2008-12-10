/***************************************************************************
 *   Copyright (C) 2008 by Tomasz Czyzewski                                *
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
#ifndef ADDDOWNLOADDIALOG_H
#define ADDDOWNLOADDIALOG_H

#include <QDialog>
#include <QColor>

class QTextEdit;
class QComboBox;

class AddDownloadDialog : public QDialog
{
    Q_OBJECT
public:
    AddDownloadDialog(QWidget* parent = NULL);
    virtual ~AddDownloadDialog();

public slots:
    virtual void accept();
    void resetItemBackground();
    void browseForDownloadFolder();

protected:
    void initialize();
    void markWidgetContentAsInvalid(QWidget* widget);
    void resetItemBackground(QWidget* widget);

    struct DialogSettings
    {
        static const int MinimumUrlTextEditWidth = 430;
        static const int FolderButtonMaxmimumWidth = 30;
        static const int MainLayoutWidgetSpacing = 12;
        static const QColor invalidItemBackgroundColor;
    };

protected:
    QTextEdit* m_urlEdit;
    QComboBox* m_folderCombo;

};

#endif