#include "qrapidshareuser.h"
//
QRapidshareUser::QRapidshareUser( const QString &userName, const QString &pass ) 
 : m_apUserName( new QString(userName) ), m_apUserPass( new QString ( pass ) )
{
	
}
QRapidshareUser::~QRapidshareUser()
{
	m_apUserName.release();
	m_apUserPass.release();
}
QString QRapidshareUser::ComposeCookie()
{
	QString cookie="Cookie: user=";
	cookie += getUserName();
	cookie +="-";
	QString aa;
	aa = m_apUserPass->toAscii();
	foreach(QChar a,*m_apUserPass)
	{
		cookie += "%" + aa.sprintf("%x", a.toLatin1()) ;	
	}
	return cookie;
}
//
