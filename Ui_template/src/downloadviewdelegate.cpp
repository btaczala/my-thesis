/***************************************************************************
 *   Copyright (C) 2008 by Bartek Taczała   *
 *   b@kontrasty.szczecin.pl   *
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
#include "downloadviewdelegate.h"

DownloadViewDelegate::DownloadViewDelegate(MainWindow* parent): QItemDelegate(parent)
{
}


DownloadViewDelegate::~DownloadViewDelegate()
{
}

void DownloadViewDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index)
{
	if (index.column() != 2) 
	{
		QItemDelegate::paint(painter, option, index);
		return;
	}
         // Set up a QStyleOptionProgressBar to precisely mimic the
         // environment of a progress bar.
	QStyleOptionProgressBar progressBarOption;
	progressBarOption.state = QStyle::State_Enabled;
	progressBarOption.direction = QApplication::layoutDirection();
	progressBarOption.rect = option.rect;
	progressBarOption.fontMetrics = QApplication::fontMetrics();
	progressBarOption.minimum = 0;
	progressBarOption.maximum = 100;
	progressBarOption.textAlignment = Qt::AlignCenter;
	progressBarOption.textVisible = true;

         // Set the progress and text values of the style option.
	//int progress = qobject_cast<MainWindow *>(parent())->clientForRow(index.row())->progress();
	//progressBarOption.progress = progress < 0 ? 0 : progress;
	//progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);
         // Draw the progress bar onto the view.
	QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
}


