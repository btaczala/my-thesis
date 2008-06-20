#include "qrapidshareuser.h"
//
QRapidshareUser::QRapidshareUser( const QString &userName, const QString &pass ) 
 : m_apUserName( new QString(userName) ), m_apUserPass( new QString ( pass ) )
{
	QT_DEBUG_FUNCTION
}
QRapidshareUser::QRapidshareUser(const &QRapidshareUser _cpy)
{
	m_apUserName.reset(new 
}
QRapidshareUser::~QRapidshareUser()
{
	QT_DEBUG_FUNCTION
	m_apUserName.release();
	m_apUserPass.release();
}
QString QRapidshareUser::ComposeCookie()
{
	QT_DEBUG_FUNCTION
	QString cookie="user=";
	cookie += getUserName();
	cookie +="-";
	QString aa;
	aa = m_apUserPass->toAscii();
	foreach(QChar a,*m_apUserPass)
	{
		cookie += QString("%" + aa.sprintf("%x", a.toLatin1()) ).toUpper() ;	
	}
	return cookie;
}
//
