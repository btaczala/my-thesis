#include "downloaddelegate.h"
DownloadViewDelegate::DownloadViewDelegate(QObject *parent) :  QItemDelegate(parent)
{
	;
}
void DownloadViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index )
{
	qDebug() << __FUNCTION_NAME__ ;
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
	// Set the progress and text values of the style option.
	//int progress = qobject_cast<MainWindow *>( parent())->GetProgress();
	//progressBarOption.progress = progress < 0 ? 0 : progress;
	//progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);
	//Draw the progress bar onto the view.
	//QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
};
