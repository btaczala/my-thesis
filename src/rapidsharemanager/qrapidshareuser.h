#ifndef QRAPIDSHAREUSER_H
#define QRAPIDSHAREUSER_H
//

#include <QString>
#include <QDebug>
#include "debugutils.h"
#include "rslogger.h"
#include "common.h"
class QRapidshareUser  
{
public:
	QRapidshareUser(const QString &userName, const QString &pass);
	QRapidshareUser(const QRapidshareUser & _cpy);
	QString	getUserName() const 
	{
		//RSDM_LOG_FUNC ;
		return ( m_apUserName );
	};
	QString	getUserPass() const 
	{
		//RSDM_LOG_FUNC ;
		return m_apUserPass;
	};
	QString					ComposeCookie();
private:
	QString					m_apUserName;
	QString					m_apUserPass;
	RSLogger				m_Logger;
};
#endif
