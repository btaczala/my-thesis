#include "DownloadView.h"
const unsigned int DownloadView::UrlPathColumnID = 0;
const unsigned int DownloadView::FilePathColumnID = 1;
const unsigned int DownloadView::ProgressColumnID = 2;
const unsigned int DownloadView::DownloadRateColumnID = 3;
const unsigned int DownloadView::StatusColumnID = 4;
DownloadView::DownloadView(QWidget * parent) : QTreeWidget( parent )
{
	setAcceptDrops(true);
};
void DownloadView::dropEvent(QDropEvent *event)
{
	QString fileName = QUrl( event->mimeData()->text() ).path();
	if ( QFile::exists( fileName ) && fileName.toLower().endsWith( ".torrent" ) )
		emit fileDropped( fileName );
};
void DownloadView::dragMoveEvent(QDragMoveEvent * event)
{
	QUrl url(event->mimeData()->text());
	// 	if (url.isValid() && url.scheme().toLower() == "file"s
	// 		   && url.path().toLower().endsWith(".torrent"))
	event->acceptProposedAction();
};
void DownloadView::AddToDownload(const QString & urlPath, const QString & fileDest)
{
	DownloadItem *pItem = new DownloadItem() ;
	pItem->setText( UrlPathColumnID, urlPath ) ;
	pItem->setText( FilePathColumnID, fileDest ) ;
	pItem->setText( ProgressColumnID, tr( "0" ) ) ;
	pItem->setText( DownloadRateColumnID, tr( "0 kbps" ) ) ;
	pItem->setText( StatusColumnID, tr( "Stopped" ) ) ;
	setCurrentItem( pItem ) ;
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
};

void DownloadView::UpdateProgress( const unsigned int & at, const unsigned int & percentage )
{
	ChangeColumn(at, ProgressColumnID, QString::number(percentage)) ;
}
void DownloadView::UpdateStatus( const unsigned int & at, const QString & status )
{
	ChangeColumn(at, StatusColumnID, status) ;
}
void DownloadView::ChangeColumn( const unsigned int & at, const unsigned int & columnID, const QString & toChange )
{
	QTreeWidgetItem *pItem = topLevelItem( at ) ;
	if( pItem)
		pItem->setText(columnID, toChange);
};