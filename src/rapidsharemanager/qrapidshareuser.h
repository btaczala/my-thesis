#ifndef QRAPIDSHAREUSER_H
#define QRAPIDSHAREUSER_H
//

#include <QString>
#include <QDebug>
#include "debugutils.h"
class QRapidshareUser  
{
public:
	QRapidshareUser(const QString &userName, const QString &pass);
	QRapidshareUser(const QRapidshareUser & _cpy);
	QString	getUserName() const 
	{
		QT_DEBUG_FUNCTION
		return ( m_apUserName );
	};
	QString	getUserPass() const
	{
		QT_DEBUG_FUNCTION
		return m_apUserPass;
	};
	QString					ComposeCookie();
private:
	QString					m_apUserName;
	QString					m_apUserPass;
};
#endif
