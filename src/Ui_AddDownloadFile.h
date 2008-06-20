#ifndef UI_ADDDOWNLOADFILE_H_
#define UI_ADDDOWNLOADFILE_H_

#include <QDialog>
#include <QClipboard>
#include <QPointer>
#include <QFileDialog>
#include <QDebug>
#include <QUrl>
#include "ui_adddownload.h"
class Ui_AddDownloadFile : public QDialog
{
	Q_OBJECT
public:
	Ui_AddDownloadFile( QWidget *parent = 0 , const QString & pathToFile = QString("") );
	virtual ~Ui_AddDownloadFile();
	QUrl GetUrl() const ;
	QString GetUrlPath() const ;
	QString	GetDestinationPath() const ;
private:
	Ui::Ui_AddDownloadFile ui;
	QPointer<QClipboard>	m_qpClipboard;
	QString					m_UrlPathToFile;
	QString					m_DestinationPath;
public slots:
	void browseForDestination() ;
	void ok();
};

#endif /*UI_ADDDOWNLOADFILE_H_*/
