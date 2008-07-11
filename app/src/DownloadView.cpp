#include "DownloadView.h"
DownloadView::DownloadView(QWidget * parent) : QTreeWidget( parent )
{
	setAcceptDrops(true);
};
DownloadView::~DownloadView()
{
	
}
void DownloadView::dropEvent(QDropEvent *event)
{
	QString fileName = QUrl( event->mimeData()->text() ).path();
	if ( QFile::exists( fileName ) && fileName.toLower().endsWith( ".torrent" ) )
		emit fileDropped( fileName );
};
void DownloadView::dragMoveEvent(QDragMoveEvent * event)
{
	QUrl url(event->mimeData()->text());
	// 	if (url.isValid() && url.scheme().toLower() == "file"
	// 		   && url.path().toLower().endsWith(".torrent"))
	event->acceptProposedAction();
};
void DownloadView::AddToDownload(const QString & urlPath, const QString & fileDest)
{
	DownloadItem *pItem = new DownloadItem() ;
	pItem->setText( 0, urlPath ) ;
	pItem->setText( 1, fileDest ) ;
	pItem->setText( 2, tr( "0" ) ) ;
	pItem->setText( 3, tr( "0 kbps" ) ) ;
	pItem->setText( 4, tr( "Stopped" ) ) ;
	setCurrentItem( pItem );
	addTopLevelItem( pItem ) ;
};
QList<int> DownloadView::DeleteDownloads(const QList<QTreeWidgetItem*> & toDelete)
{
	QList<int> toRet; 
	int iPos = 0;
	foreach ( QTreeWidgetItem * pItem, toDelete )
	{
		iPos = indexOfTopLevelItem(pItem);
		toRet.push_back(iPos);
		delete pItem ; 
		pItem = NULL;
	}
	return toRet;
};
QList<int> DownloadView::StopDownloads(const QList<QTreeWidgetItem *> & toStop )
{
	QList<int> toRet;
	int iPos;
	DownloadItem *pDownloadItem;
	foreach(QTreeWidgetItem * pItem, toStop)
	{
		iPos = indexOfTopLevelItem(pItem);
		toRet.push_back(iPos);
	}
	return toRet;
};
void DownloadView::swap(const QTreeWidgetItem * _one, const QTreeWidgetItem * _two )
{
	QTreeWidgetItem *pItem = new QTreeWidgetItem();
	*pItem = *_one ;
	QTreeWidgetItem *pItem2 = new QTreeWidgetItem();
	*pItem2 = *_two;
	QTreeWidgetItem *aboveOne = itemAbove( _one );
	int iStartPoint = indexOfTopLevelItem( aboveOne );
	delete _one;
	delete _two;
	insertTopLevelItem( ++iStartPoint, pItem2);
	insertTopLevelItem( ++iStartPoint, pItem);
}