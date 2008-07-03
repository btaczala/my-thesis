#include "qrapidshareuser.h"
//
QRapidshareUser::QRapidshareUser( const QString &userName, const QString &pass ) 
 : m_apUserName( QString(userName) ), m_apUserPass(QString ( pass ) ), m_Logger("qrapidshareuser")
{
	RSDM_LOG_FUNC ;
};
 
QRapidshareUser::QRapidshareUser(const QRapidshareUser & _cpy)
{
	RSDM_LOG_FUNC ;
 	m_apUserName = _cpy.getUserName();
 	m_apUserPass = _cpy.getUserPass();

}
QString QRapidshareUser::ComposeCookie()
{
	RSDM_LOG_FUNC ;
	QString cookie="user=";
	cookie += getUserName();
	cookie +="-";
	QString aa;
	aa = m_apUserPass.toAscii();
	foreach(QChar a,m_apUserPass)
	{
		cookie += QString("%" + aa.sprintf("%x", a.toLatin1()) ).toUpper() ;	
	}
	return cookie;
}
//
