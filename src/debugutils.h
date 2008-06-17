#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H
//

#include <QString>
#include <QDebug>

#define QT_DEBUG_FUNCTION qDebug() << __PRETTY_FUNCTION__ << endl;

class DebugUtils  
{
public:
	static void q_Warn( const QString & toWarn, const char * functionName = NULL );
	static void q_Log ( const QString & toLog, const char * functionName = NULL );
	static void q_Error ( const QString & toLog, const char * functionName = NULL );
};
#endif
