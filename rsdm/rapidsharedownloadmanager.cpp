#include "rapidsharedownloadmanager.h"

RapidShareDownloadManager::RapidShareDownloadManager() : m_Logger("RapidshareDownloadManager.log"), m_iMaxDownload(3), m_iCurrentDownload(0)
{	
	RSDM_LOG_FUNC ;
}
RapidShareDownloadManager::~RapidShareDownloadManager()
{
	RSDM_LOG_FUNC ;
	foreach(QRapidshareDownload *rsd, m_RapidshareDownloads)
	{
		if(rsd)
		{
			rsd->stop();
			delete rsd;
		}
	}
	m_apRapidshareUser.release();
};
void RapidShareDownloadManager::AddDownload(const QString & toDownload,const QString & where )
{
	RSDM_LOG_FUNC ;
	QMutex mutex;
	QString where2 = where +".part"; 
	QFile whereFile (where2);
	m_Logger.Write("Mutex::Lock");
	mutex.lock();
	m_Logger.Write("Adding download with toDownload: " + toDownload + " and where : " + where  );
	if( whereFile.exists() )
	{
		if(whereFile.remove())
			m_Logger << "File " << whereFile.fileName() << "removed";
		else
			m_Logger << "File " << whereFile.fileName() << "not removed";
	}
	QRapidshareDownload *rsd = new QRapidshareDownload( toDownload, where );
	rsd->SetUser( *m_apRapidshareUser );
	QObject::connect( rsd,SIGNAL( WhatAmIDoing(  const RapidShareStateMachine )), this, SLOT( Slot_ChangedState( const RapidShareStateMachine ) ) );
	QObject::connect( rsd,SIGNAL( DownloadStatus( int )), this, SLOT( Slot_ChangeProgressValue( int ) ) );
	QObject::connect( rsd,SIGNAL( Done() ), this, SLOT( Slot_DoneDownloadingOne() ) );
	QObject::connect( rsd,SIGNAL( downloadRate(const QString ) ), this, SLOT( Slot_DownloadRateChanged(const QString) ) );
	
	m_RapidshareDownloads.push_back(rsd);
	DownloadAsManyAsCan();
	mutex.unlock();
	m_Logger.Write("Mutex::UnLock");
};
void RapidShareDownloadManager::SetUser(const QRapidshareUser & user)
{
	RSDM_LOG_FUNC ;
	m_Logger.Write("Setting user: name" + user.getUserName() + " pass:" + user.getUserPass());
	m_apRapidshareUser.reset(new QRapidshareUser( user ) );
};
void RapidShareDownloadManager::SetMaxDownloads(const unsigned int & _MaxDownloads)
{
	RSDM_LOG_FUNC ;
	QString log = QString("Setting max downloads to %1").arg(_MaxDownloads);
	m_Logger.Write(log);
	m_iMaxDownload = _MaxDownloads;
};

const unsigned int RapidShareDownloadManager::GetMaxDownloads()
{
	RSDM_LOG_FUNC ;
	return m_iMaxDownload;
}
void RapidShareDownloadManager::Pause(const QList<int> & listToPause ) 
{
	RSDM_LOG_FUNC ;
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
};
void RapidShareDownloadManager::stopAt(const unsigned int & at )
{
	RSDM_LOG_FUNC ;
	unsigned int iSize =m_RapidshareDownloads.size();
	if( at > (iSize - 1) || iSize == 0 )
		return ;
	QRapidshareDownload * pDownload = m_RapidshareDownloads.at(at);
	if( pDownload )
		pDownload->stop();

}
void RapidShareDownloadManager::stopAll()
{
	foreach(QRapidshareDownload *pDownload, m_RapidshareDownloads )
	{
		pDownload->stop();
	}
}
const QRapidshareDownload * RapidShareDownloadManager::GetAt(const unsigned int & at ) 
{
	unsigned int iSize =m_RapidshareDownloads.size();
	if( at > (iSize -1) || iSize == 0 )
		return NULL;
	const QRapidshareDownload *ret = m_RapidshareDownloads.at(at); 
	return ret;
};
void RapidShareDownloadManager::swap(const unsigned int & one, const unsigned int & two )
{
	if( one > ( unsigned int ) (m_RapidshareDownloads.size() -1)  || two > (unsigned int ) (m_RapidshareDownloads.size() - 1 )  || m_RapidshareDownloads.size() == 0 )
		return;
	QRapidshareDownload * qrsdOne = m_RapidshareDownloads.at( one );
	QRapidshareDownload * qrsdTwo = m_RapidshareDownloads.at( two );
	QRapidshareDownload * temp = qrsdOne ;
	qrsdOne = qrsdTwo;
	qrsdTwo = temp;
}
QRapidshareUser RapidShareDownloadManager::GetUser()
{
	RSDM_LOG_FUNC ;
	if( m_apRapidshareUser.get() )
		return *m_apRapidshareUser;
	else
		return QRapidshareUser("","");
}
void RapidShareDownloadManager::DownloadAsManyAsCan(const unsigned int & startPoint )
{
	RSDM_LOG_FUNC ;
	m_Logger.Write( QString("With startpoint=%1,m_iCurrentDownload=%2, m_iMaxDownload=%3 ").arg(startPoint).arg(m_iCurrentDownload).arg(m_iMaxDownload) );
	if(m_iCurrentDownload == m_iMaxDownload)
		return;
	
	QRapidshareDownload *rsd;
	for(int k = startPoint ; k != m_RapidshareDownloads.size() ; ++k)
	{
		if( m_iCurrentDownload == m_iMaxDownload)
			break;
		rsd = m_RapidshareDownloads.at(k);
		if( rsd )
		{
			if( rsd->GetState() == STOPPED )
			{
				rsd->Download() ;
				++m_iCurrentDownload ;
			}
		}
	}
}
/*
 *		SLOTS
*/
void RapidShareDownloadManager::Slot_DoneDownloadingOne()
{
	RSDM_LOG_FUNC ;
	QMutex mutex;
	mutex.lock();
	QRapidshareDownload *rsd = qobject_cast<QRapidshareDownload*>(sender());
	int iPos = m_RapidshareDownloads.indexOf(rsd);
	-- m_iCurrentDownload;
	mutex.unlock();
	emit DoneDownloading(iPos);
	DownloadAsManyAsCan(0);
};
void RapidShareDownloadManager::Slot_ChangedState( const RapidShareStateMachine & state)
{
	RSDM_LOG_FUNC ;
	QRapidshareDownload *rsd = qobject_cast<QRapidshareDownload*>(sender());
	int iPos = m_RapidshareDownloads.indexOf(rsd);
	emit ChangedName( (unsigned int)iPos, StateToString(state) );
};
void RapidShareDownloadManager::Slot_ChangeProgressValue( int progress )
{
	//RSDM_LOG_FUNC ;
	QRapidshareDownload *rsd = qobject_cast<QRapidshareDownload*>(sender());
	int iPos = m_RapidshareDownloads.indexOf(rsd);
	emit ChangedProgress( iPos, progress );
};
void RapidShareDownloadManager::Slot_DownloadRateChanged(const QString & rate) 
{
	RSDM_LOG_FUNC ;
	QRapidshareDownload *rsd = qobject_cast<QRapidshareDownload*>(sender());
	if(rsd)
	{
		int iPos = m_RapidshareDownloads.indexOf(rsd);
		emit DownloadRateChanged( (unsigned int)iPos, rate);
	}
}

void RapidShareDownloadManager::RemoveAt(const unsigned int & iPos)
{
	RSDM_LOG_FUNC ;
	unsigned int iSize = m_RapidshareDownloads.size();
	if( iPos > iSize -1 || iSize == 0)
	{
		m_Logger << "Item not found ";
		return;
	}
	QRapidshareDownload* pRSDownload = m_RapidshareDownloads.at(iPos);
	RapidShareStateMachine rssmState = pRSDownload->GetState() ;
	if( rssmState!=STOPPED && rssmState != DONE && rssmState != FAILED )
	{
		pRSDownload->abort();
		--m_iCurrentDownload;
		
	}
	m_Logger << "Removing item with iPos = " << iPos;
	m_RapidshareDownloads.removeAt(iPos);
	delete pRSDownload;
	DownloadAsManyAsCan( 0 );
}

