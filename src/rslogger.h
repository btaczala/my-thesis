#ifndef RSLOGGER_H
#define RSLOGGER_H
//

//
#include <QString>
#include <QFile>

class RSLogger  
{
public:
	static RSLogger & Instance(const QString & logName )
	{
		static RSLogger theLog(logName);
		return theLog;
	}
	void log(const QString & toLog);
private:
	RSLogger(const QString & fileName);
	~RSLogger();
	QFile	m_logFile;
	
};
#endif
