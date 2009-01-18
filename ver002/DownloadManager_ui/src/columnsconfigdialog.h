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
#ifndef COLUMNSCONFIGDIALOG_H
#define COLUMNSCONFIGDIALOG_H

#include <QDialog>

#include "ui_columnsConfig.h"
#include "qdownloadwidget.h"

class ColumnsConfigDialog : public QDialog
{
    Q_OBJECT
public:
    ColumnsConfigDialog(QDownloadWidget::ColumnCollection& columns, QWidget* parent = 0);
private slots:
    void itemChanged(QListWidgetItem*);
signals:
    void columnChanged(QDownloadWidget::QDownloadWidgetColumnInfo* column);
    
protected:
    Ui_columnsconfig _ui;
    QDownloadWidget::ColumnCollection& _columns;
};

#endif 