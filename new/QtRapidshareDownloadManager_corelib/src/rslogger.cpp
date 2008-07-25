/***************************************************************************
 *   Copyright (C) 2008 by Bartek Taczała 								   *
 *   b@kontrasty.szczecin.pl   											   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "rslogger.h"
//
const char* const RSLogger::sm_TimeFormat= "dd.MM.yyyy - hh:mm:ss.zzz ";
const QString RSLogger::rsEndLine = QString ( "__endline__");
RSLogger::RSLogger( const QString & fileName ) : m_fileMutex( new QMutex() ), m_fileName(NULL), m_fileLogger(NULL), m_bOpenedFile(false), m_NumberOfLogs(0), m_bIsOneLine( true )
{
	QString fn = fileName;
	if( ! fileName.isEmpty() )
	{
		if(!fn.endsWith(".log"))
			fn += ".log";
		SetFile( fn ) ;
	}
	OpenFile();
	
}
RSLogger::~RSLogger()
{
	m_fileLogger->flush();
	if(m_fileLogger.get() != NULL )
	{
		if( m_fileLogger->isOpen() )
		{
			m_fileLogger->close();
		}
	}
	m_fileLogger.release();
	m_fileMutex.release();
	m_fileName.release();
}
bool RSLogger::OpenFile()
{
	if( m_fileName.get() == NULL)
		return false;
	if( m_fileName->isEmpty() ) 
		return false;
	if(m_fileLogger.get() == NULL)
		m_fileLogger.reset(new QFile( *m_fileName ) ) ;
	if(! m_fileLogger->isOpen() )
	{
		if( ! m_fileLogger->open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append ))
		{
			DebugUtils::q_Error("Unable to open log file " + *m_fileName );
			return false ;
		}
		m_fileLogger->write("-----NEW LOG ----- \n");		
	}
	m_bOpenedFile = true ; 
	return true;
};
QString RSLogger::GetCurrentTime() const 
{
	QString toRet ;
	QDateTime now ( QDate::currentDate(), QTime::currentTime());
	toRet = now.toString(sm_TimeFormat);
	return toRet;
}
QString RSLogger::createLogEntry(const QString & toLog) const 
{
	QString logEntry = GetCurrentTime();
	logEntry +=toLog;
	return logEntry;
}
void RSLogger::writeToFile( const QString & toLog ) const 
{
	QByteArray log ;
	log.append(toLog);
	m_fileLogger->write( log );
	m_fileLogger->write("\n");
	++m_NumberOfLogs;
	if( m_NumberOfLogs >= sm_MaxNonFlushedLogs )
	{
			m_NumberOfLogs = 0;
			m_fileLogger->flush();
	}
}
void RSLogger::Write( const QString & toLog ) const 
{
	m_fileMutex->lock();
	if(!m_bOpenedFile)
	{
		m_fileMutex->unlock();
		return;
	}
	QString lToLog = createLogEntry(toLog);
	writeToFile(lToLog);
	m_fileMutex->unlock();
};
void RSLogger::Error( const QString & toLog )
{
	m_fileMutex->lock();
	if(!OpenFile())
		return;
	QString logString = GetCurrentTime() + "FATAL ERROR! " +  toLog; 
	writeToFile(logString);
	m_fileMutex->unlock();
};

void RSLogger::SetFile(const QString & fileName ) 
{
	QDir dir( s_RSDMLogPath );
	if( !dir.exists() )
		dir.mkpath(dir.path());
	QString filePath = dir.path() + "/" + fileName;
	m_fileName.reset(new QString( filePath ) ) ;
	m_fileLogger.reset( new QFile( filePath ) );
	
}
RSLogger& RSLogger::operator<<(const QString & toLog)
{
	/*
	if( toLog.contains(rsEndLine) )
	{
		writeToFile(m_tmpOneLine) ; 
		m_tmpOneLine="";
	}
	else
	{
		if ( m_tmpOneLine.isEmpty())
		{
			m_tmpOneLine += GetCurrentTime() ; 
		}
		m_tmpOneLine += toLog ; 
	}*/
	
	QString log = toLog ; 
	if(log.contains("err:"))
		Error(log.remove("err:"));
	else
		Write(toLog);
	return *this;
}
RSLogger& RSLogger::operator<<(const int & toLog)
{
	operator<<(QString::number(toLog));
	//Write(QString::number(toLog));
	return *this;
};
ILogable::ILogable(const QString & fileName) : m_Logger(fileName)
{
	;
}
void DebugUtils::q_Warn( const QString & toWarn, const char * functionName )
{
	qDebug() << "Warning: " ;
	if( functionName != NULL ) 
		qDebug() << functionName ;
	qDebug() << toWarn;
		
	 
}
void DebugUtils::q_Log( const QString & toLog, const char * functionName )
{
	qDebug() << "Log: " ;
	if( functionName != NULL ) 
		qDebug() << functionName ;
	qDebug() << toLog;	 
}
void DebugUtils::q_Error( const QString & toLog, const char * functionName )
{
	qDebug() << "Error!: " ;
	if( functionName != NULL ) 
		qDebug() << functionName ;
	qDebug() << toLog;	 
}
QString DebugUtils::httpReqToString(const QHttpRequestHeader & req)
{
	QString ret;
	ret = "Request Header Debug :\n ";
	foreach(QString tmp, req.keys() )
	{
		ret += tmp;
		ret += ": ";
		ret += req.value(tmp) ;
		ret += " \n ";
	}
	return ret; 
}

void DebugUtils::DumpReponseToFile(const QByteArray & byteArray, const QString & fileName)
{
	QFile file;
	file.setFileName( fileName );
	if( ! file.open( QIODevice::WriteOnly ) )
	{
		DebugUtils::q_Error("Unable to open file",__FUNCTION_NAME__);
	}
	if(byteArray.isEmpty())
		DebugUtils::q_Error("Response Error" + fileName,__FUNCTION_NAME__);
	file.write( byteArray );
	file.close();
	
}

