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
#ifndef RSLOGGER_H
#define RSLOGGER_H
//
#include <QString>
#include <QFile>
#include <QDir>
#include <QMutex>
#include <QDateTime>
#include <memory>
#include <QHttpRequestHeader>
#include "rscommon.h"


#define RSDM_LOG_FUNC 	m_Logger.Write( __FUNCTION_NAME__ )


/*! \class RSLogger 
	\brief Class to log to files
	Class that log messages to corresponding files. 
*/
class RSLogger
{
public:
	/**
		Constructor && default constructor
		\param fileName as QString - sets the filename if  fileName != ""
	*/
	RSLogger(const QString & fileName = QString("") );
	/**
		DCtor
	*/
	~RSLogger();
	/**
		Writes message to buffer. It's flushed to file after MAX_NONFLUSHED_LOGS.
		\param toLog as QString - message to be logged.  
	*/
	void		Write( const QString & toLog ) const;
	/**
		Sets m_fileName to fileName, check if path exist, if not creates it. Allocates memory for QFile. 
		\param fileName as QString - name of log file. 
	*/
	void		SetFile(const QString & fileName ) ;
	/**
		Writes error message to buffer. It's flushed to file after MAX_NONFLUSHED_LOGS.
		\param toLog as QString - error message to be logged.  
	*/
	void		Error (const QString & toLog );
	/**
		Writes message to buffer. 
		\param toLog as QString - message to be logged. If toLog.contains("err:"), Error is called
			if not Write is called
	*/
	RSLogger &operator<<(const QString & toLog);
	/**
		Converts toLog to QString and calling Write()
		\pararm toLog as const int - convert to QString ( via QString::number())
	*/
	RSLogger &operator<<(const int & toLog); 

	static const QString rsEndLine ; 
private:
	/**
		Open file given by m_fileLogger. If open succed, marker is put on cursor position in the file. 
		\return True if success, false is m_fileLogger == NULL || m_fileName==""
	*/
	bool					OpenFile();
	/**
		Function returns Current time in string formated by sm_TimeFormat
		\return 
	*/
	QString					GetCurrentTime() const ;
	
	QString					createLogEntry(const QString &toLog) const ;
	/**
		Writes toWrite to file given by m_fileLogger. 
		\param toWrite - writes message to file. 
	*/
	void					writeToFile(const QString & toWrite) const ;
	std::auto_ptr<QMutex>	m_fileMutex;
	std::auto_ptr<QString>	m_fileName;
	std::auto_ptr<QFile>	m_fileLogger;
	mutable unsigned int    m_NumberOfLogs;
	bool					m_bOpenedFile ;
	bool					m_bIsOneLine ; 
	QString					m_tmpOneLine ; 

	static const unsigned int sm_MaxNonFlushedLogs = 10;
	static const  char* const  sm_TimeFormat;
};
/**
	\class ILogable - interface to enable class logging
*/
class ILogable 
{
	public:
		/**
			Only one ctor
		*/
		ILogable(const QString & fileName ) ;
		/**
			virtual dctor
		*/
		virtual ~ILogable(){};
	protected:
		RSLogger		m_Logger;
	private:
		ILogable(const ILogable & ); /// hidden
		ILogable(); 				  /// hidden
};
class DebugUtils
{
public:
	static void q_Warn( const QString & toWarn, const char * functionName = NULL );
	static void q_Log ( const QString & toLog, const char * functionName = NULL );
	static void q_Error ( const QString & toLog, const char * functionName = NULL );
	static QString httpReqToString(const QHttpRequestHeader & req);
	static void DumpReponseToFile(const QByteArray & byteArray, const QString & fileName);
};
#endif

