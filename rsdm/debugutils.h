#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H
//

#include <QString>
#include <QDebug>
#include <QHttpRequestHeader>
#include <QFile>
#include "rscommon.h"

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
