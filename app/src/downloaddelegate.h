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
	DownloadViewDelegate(QObject * parent);
protected:
	virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index );
};
#endif /*DOWNLOADDELEGATE_H_*/
