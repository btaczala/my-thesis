#include "qrapidshareuser.h"
//
QRapidshareUser::QRapidshareUser( const QString &userName, const QString &pass ) 
 : m_apUserName( QString(userName) ), m_apUserPass(QString ( pass ) )
{
	QT_DEBUG_FUNCTION
};
QRapidshareUser::QRapidshareUser(const QRapidshareUser & _cpy)
{
	
 	m_apUserName = _cpy.getUserName();
 	m_apUserPass = _cpy.getUserPass();

}
QString QRapidshareUser::ComposeCookie()
{
	QT_DEBUG_FUNCTION
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
