#include "rscommon.h"

QString TimeToString()
{
	QDateTime now ( QDate::currentDate(), QTime::currentTime());
	return now.toString(QString(" dd.MM.yyyy - hh:mm:ss.zzz ")).remove('"');
};
/*
//FIXME:
QString StateToString(const RapidShareStateMachine & rsMachineState)
{
	QString toRet = "";
	if(rsMachineState == STOPPED)
		toRet =  QString( scRsdm_StateMachineStateStopped );
	else if( rsMachineState == GET_FIRST )
		toRet =  QString( scRsdm_StateMachineStateGet_First );
	else if( rsMachineState == GET_SECOND )
		toRet =  QString( scRsdm_StateMachineStateGet_Second );
	else if( rsMachineState == POST_FIRST )
		toRet =  QString( "Requesting first POST" );
	else if( rsMachineState == GET_THIRD )
		toRet =  QString( "Downloading data" );
	else if( rsMachineState == DONE )
		toRet =  QString( "Downloading finished" );
	else if ( rsMachineState == FAILED )
		toRet =  QString( "Downloading failed!" );
	return toRet;
};

QString StateToString2( const RapidShareStateMachine & rsMachineState )
{
	QString toRet = "";
	if(rsMachineState == STOPPED)
		toRet =  QString( "STOPPED" );
	else if( rsMachineState == GET_FIRST )
		toRet =  QString( "GET_FIRST" );
	else if( rsMachineState == GET_SECOND )
		toRet =  QString( "GET_SECOND" );
	else if( rsMachineState == POST_FIRST )
		toRet =  QString( "POST_FIRST" );
	else if( rsMachineState == GET_THIRD )
		toRet =  QString( "GET_THIRD" );
	else if( rsMachineState == DOWNLOADING )
		toRet =  QString( "DOWNLOADING" );
	else if( rsMachineState == DONE )
		toRet =  QString( "DONE" );
	else if ( rsMachineState == FAILED )
		toRet =  QString( "FAILED" );
	else if ( rsMachineState == PAUSED )
		toRet =  QString( "PAUSED" );
	return toRet;
}
RapidShareStateMachine StringToState( const QString & string )
{
	if( string.contains("STOPPED"))
		return STOPPED ; 
	else if( string.contains("GET_FIRST"))
		return GET_FIRST ; 
	else if( string.contains("GET_SECOND"))
		return GET_SECOND ; 
	else if( string.contains("POST_FIRST"))
		return POST_FIRST ; 
	else if( string.contains("GET_THIRD"))
		return GET_THIRD ; 
	else if( string.contains("DOWNLOADING"))
		return DOWNLOADING ; 
	else if( string.contains("DONE"))
		return DONE ; 
	else if( string.contains("FAILED"))
		return FAILED ; 
	else if( string.contains("PAUSED"))
		return PAUSED ; 
	return STOPPED ; 
}
*/