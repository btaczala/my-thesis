#include "dialogimpl.h"

DialogImpl::DialogImpl( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f), m_RapidShareDownload( "" )
{
	QT_DEBUG_FUNCTION
	setupUi(this);
	QRapidshareUser aa("4625386", "WM2FTZgx5Y");	
	qDebug() << aa.ComposeCookie();
//	QObject::connect(DownloadButton, SIGNAL(  pressed() ), this, SLOT( downloadPressed() ) );
	
//	AddressLine->insert("http://rapidshare.com/files/122322166/Me__su____eyrum_vi__spilum_endalaust.part1.rar");
}
void DialogImpl::downloadPressed()
{
	QT_DEBUG_FUNCTION
	QString address = AddressLine->displayText();
	if( !address.isEmpty() )
	{
		m_RapidShareDownload.SetUrlFileAddress(address);
		m_RapidShareDownload.Download();
	}
}
