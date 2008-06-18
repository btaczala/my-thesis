#include "dialogimpl.h"

DialogImpl::DialogImpl( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f), m_RapidShareDownload( "" )
{
	QT_DEBUG_FUNCTION
	setupUi(this);
	
	QObject::connect(DownloadButton, SIGNAL(  pressed() ), this, SLOT( downloadPressed() ) );
	QObject::connect(progressBar, SIGNAL( valueChanged( int ) ), this, SLOT( ValueChanged( int ) ) );
	QObject::connect(&m_RapidShareDownload, SIGNAL( WhatAmIDoing( QString ) ), this, SLOT( ChangeProgressName( QString ) ) );
	QObject::connect(&m_RapidShareDownload, SIGNAL( DownloadStatus( int ) ), this, SLOT( ChangeProgressValue( int ) ) );
	
	AddressLine->insert("http://rapidshare.com/files/122321322/Me__su____eyrum_vi__spilum_endalaust.part2.rar");
	progressBar->setValue(0);
	progressBar->setMinimum(0);
	progressBar->setMaximum(100);

}
void DialogImpl::downloadPressed()
{
	QT_DEBUG_FUNCTION
	QString address = AddressLine->displayText();
	if( !address.isEmpty() )
	{
		DownloadButton->setEnabled(false);
		m_RapidShareDownload.SetUrlFileAddress(address);
		m_RapidShareDownload.Download();
	}
}
void DialogImpl::ValueChanged(const int & value)
{
	QT_DEBUG_FUNCTION
	if( value == progressBar->maximum() )
	{
		DownloadButton->setEnabled(true);
	}
}
void DialogImpl::ChangeProgressName(const QString & what)
{
	QT_DEBUG_FUNCTION
	QString format = what;
	format += " %p/%m";
	progressBar->setFormat(format);
};
void DialogImpl::ChangeProgressValue(const int & iPerc)
{
	QT_DEBUG_FUNCTION
	progressBar->setValue(iPerc);
};
