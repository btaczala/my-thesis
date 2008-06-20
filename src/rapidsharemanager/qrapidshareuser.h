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
	QRapidshareUser(const &QRapidshareUser _cpy); 
	~QRapidshareUser();
	QString	getUserName()
	{
		QT_DEBUG_FUNCTION
		return *m_apUserName;
	}
	QString	getUserPass()
	{
		QT_DEBUG_FUNCTION
		return *m_apUserPass;
	}
	QString							ComposeCookie();
private:
	std::auto_ptr<QString>					m_apUserName;
	std::auto_ptr<QString>					m_apUserPass;
};
#endif
