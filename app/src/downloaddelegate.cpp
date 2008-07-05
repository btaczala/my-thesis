#include "downloaddelegate.h"
#include "mainwindow.h"
DownloadViewDelegate::DownloadViewDelegate(QObject *parent) :  QItemDelegate(parent)
{
	;
}
void DownloadViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
	if (index.column() != 2) 
	{
		QItemDelegate::paint(painter, option, index);
		return;
	}
	QStyleOptionProgressBar progressBarOption;
	progressBarOption.state = QStyle::State_Enabled;
	progressBarOption.direction = QApplication::layoutDirection();
	progressBarOption.rect = option.rect;
	progressBarOption.fontMetrics = QApplication::fontMetrics();
	progressBarOption.minimum = 0;
	progressBarOption.maximum = 100;
	progressBarOption.textAlignment = Qt::AlignCenter;
	progressBarOption.textVisible = true;	
	int iRow = index.row();
	int progress = qobject_cast<MainWindow * >(parent())->GetRapidshareDownloadProgressAt(iRow);
	if(progress == -1 )
		progress = 0;

	// Set the progress and text values of the style option.
	progressBarOption.progress = progress < 0 ? 0 : progress;
	progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);
	//Draw the progress bar onto the view.
	QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
};
