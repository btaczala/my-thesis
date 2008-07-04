#ifndef DOWNLOADDELEGATE_H_
#define DOWNLOADDELEGATE_H_

#include <QItemDelegate>
#include <QtGui>
#include <QtDebug>
#include "common.h"

class DownloadViewDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	DownloadViewDelegate(QObject * parent=0);
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const ;
};
#endif /*DOWNLOADDELEGATE_H_*/
