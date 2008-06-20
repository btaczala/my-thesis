#include "Ui_AddDownloadFile.h"

Ui_AddDownloadFile::Ui_AddDownloadFile(QWidget *parent, const QString & pathToFile) :  QDialog( parent )
{
	qDebug() << __PRETTY_FUNCTION__;
	ui.setupUi(this) ;
	if(pathToFile.isEmpty())
	{
		m_qpClipboard = QApplication::clipboard ();
		QString text = m_qpClipboard->text(QClipboard::Clipboard);
		if( !text.isEmpty() )
		{
			ui.DownloadFileUrl->setText(text);
		}		
	}
	else
		ui.DownloadFileUrl->setText(pathToFile);
		
	QObject::connect(ui.browseDestination, SIGNAL(clicked()),this, SLOT(browseForDestination()));
	QObject::connect(ui.okButton, SIGNAL(clicked()),this, SLOT(ok()));	
}
Ui_AddDownloadFile::~Ui_AddDownloadFile()
{
}
void Ui_AddDownloadFile::browseForDestination()
{
	qDebug() << __PRETTY_FUNCTION__;
	QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a destination directory"), QDir::home().path());
	if (dir.isEmpty())
		 return;
	ui.destinationFolder->setText(dir);
	ui.okButton->setEnabled(true);
};
void Ui_AddDownloadFile::ok()
{
	qDebug() << __PRETTY_FUNCTION__;
	m_UrlPathToFile = ui.DownloadFileUrl->text();
	m_DestinationPath = ui.destinationFolder->text();
	QDialog::accept();
	
}
QString Ui_AddDownloadFile::GetDestinationPath() const 
{
	return m_DestinationPath.isEmpty() ?QDir::home().path() :  m_DestinationPath;	
};
QString Ui_AddDownloadFile::GetUrlPath() const
{
	return m_UrlPathToFile;
}
QUrl Ui_AddDownloadFile::GetUrl() const 
{
	return QUrl(m_UrlPathToFile);
};
