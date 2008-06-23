#include "rapidsharedownloadmanager.h"

RapidShareDownloadManager::RapidShareDownloadManager()
{	
	m_iMaxDownload = 3;
	m_iCurrentDownload = 0;
	m_qpDownloadTimer = new QTimer( this );
	connect( m_qpDownloadTimer, SIGNAL( timeout()), this, SLOT( Slot_Timer() ) );
	m_qpDownloadTimer->start( 10 );
}
RapidShareDownloadManager::~RapidShareDownloadManager()
{
	foreach(QRapidshareDownload *rsd, m_RapidshareDownloads)
	{
		if(rsd)
		{
			rsd->stop();
			delete rsd;	
		}		
	}
	m_apRapidshareUser.release();
	delete m_qpDownloadTimer;
};
void RapidShareDownloadManager::AddDownload(const QString & toDownload,const QString & where )
{
	QRapidshareDownload *rsd = new QRapidshareDownload( toDownload, where );
	rsd->SetUser( *m_apRapidshareUser );
	QObject::connect( rsd,SIGNAL( WhatAmIDoing(  const RapidShareStateMachine )), this, SLOT( Slot_ChangedState( const RapidShareStateMachine ) ) );
	QObject::connect( rsd,SIGNAL( DownloadStatus( int )), this, SLOT( Slot_ChangeProgressValue( int ) ) );
	QObject::connect( rsd,SIGNAL( Done() ), this, SLOT( Slot_DoneDownloadingOne() ) );
	m_RapidshareDownloads.push_back(rsd);
};
void RapidShareDownloadManager::SetUser(const QRapidshareUser & user)
{
	m_apRapidshareUser.reset(new QRapidshareUser( user ) );
};
void RapidShareDownloadManager::SetMaxDownloads(const unsigned int & _MaxDownloads)
{
	m_iMaxDownload = _MaxDownloads;
};
void RapidShareDownloadManager::Pause(const QList<int> & listToPause ) 
{
	if(listToPause.isEmpty())
	{
		foreach( QRapidshareDownload *rsd, m_RapidshareDownloads)
		{
			rsd->stop();
		}
	}
	else
	{
		foreach(int iter, listToPause) 
		{
			QRapidshareDownload *rsd = m_RapidshareDownloads.at( iter );
			rsd->stop();	
		}
	}
}
void RapidShareDownloadManager::DownloadAsManyAsCan(const unsigned int & startPoint )
{
	if(m_iCurrentDownload == m_iMaxDownload)
		return;
	
	unsigned int iDownloadCounter = 0;
	for(int k = startPoint ; k != m_RapidshareDownloads.size() ; ++k)
	{
		if( iDownloadCounter > m_iMaxDownload )
		{
			break;
		}
		QRapidshareDownload *rsd = m_RapidshareDownloads.at(k);
		if(rsd)
		{
			if(rsd->GetState() == STOPPED )
				rsd->Download() ;
				++m_iCurrentDownload;
		}
	}
}
/*
 *		SLOTS
*/
void RapidShareDownloadManager::Slot_DoneDownloadingOne()
{
	QRapidshareDownload *rsd = qobject_cast<QRapidshareDownload*>(sender());
	int iPos = m_RapidshareDownloads.indexOf(rsd);
	-- m_iCurrentDownload;
	emit DoneDownloading(iPos);
	DownloadAsManyAsCan(iPos);
};
void RapidShareDownloadManager::Slot_ChangedState( const RapidShareStateMachine & state)
{
	QRapidshareDownload *rsd = qobject_cast<QRapidshareDownload*>(sender());
	int iPos = m_RapidshareDownloads.indexOf(rsd);
	emit ChangedName( iPos, StateToString(state) );
};
void RapidShareDownloadManager::Slot_ChangeProgressValue( int progress )
{
	QRapidshareDownload *rsd = qobject_cast<QRapidshareDownload*>(sender());
	int iPos = m_RapidshareDownloads.indexOf(rsd);
	emit ChangedProgress( iPos, progress );
};
void RapidShareDownloadManager::Slot_Timer()
{
	DownloadAsManyAsCan(0);
};	
