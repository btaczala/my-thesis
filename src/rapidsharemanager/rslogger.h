#ifndef RSLOGGER_H
#define RSLOGGER_H
//
#include <QString>
#include <QFile>
#include <QDir>
#include <QMutex>
#include <QDateTime>
#include "debugutils.h"
#include "../common.h"

#define RSDM_LOG_FUNC 	m_Logger.Write(__PRETTY_FUNCTION__ )
#define MAX_NONFLUSHED_LOGS 10
class RSLogger  
{
public:
	RSLogger(const QString & fileName = QString("") );
	~RSLogger();
	void		Write( const QString & toLog ) ;
	void		SetFile(const QString & fileName ) ;
	void		Error (const QString & toLog );
	RSLogger &operator<<(const QString & toLog);
	RSLogger &operator<<(const int & toLog);  
private:
	bool					OpenFile();
	std::auto_ptr<QMutex>	m_fileMutex;
	std::auto_ptr<QString>	m_fileName;
	std::auto_ptr<QFile>	m_fileLogger;
	
	int						m_NumberOfLogs;
};

#endif
