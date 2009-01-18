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
#include "columnsconfigdialog.h"
#include <QListWidget>

ColumnsConfigDialog::ColumnsConfigDialog(QDownloadWidget::ColumnCollection& columns, QWidget* parent)
: QDialog(parent), _columns(columns)
{
    _ui.setupUi(this);

    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    
    for (QDownloadWidget::ColumnCollection::iterator i = _columns.begin(); i != _columns.end(); ++i)
    {
        QListWidgetItem* item = new QListWidgetItem(i->getName());
        item->setCheckState(i->isVisible()?Qt::Checked:Qt::Unchecked);
        item->setData(Qt::UserRole, qVariantFromValue(reinterpret_cast<void*>(&*i)));
        _ui.columnsList->addItem(item);
    }

    bool b = connect(_ui.columnsList, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(itemChanged(QListWidgetItem*)));

}

void ColumnsConfigDialog::itemChanged(QListWidgetItem* item)
{
    QDownloadWidget::QDownloadWidgetColumnInfo* column = reinterpret_cast<QDownloadWidget::QDownloadWidgetColumnInfo*>(item->data(Qt::UserRole).value<void*>());

    column->setVisible(item->checkState()==Qt::Checked);

    emit columnChanged(column);
}