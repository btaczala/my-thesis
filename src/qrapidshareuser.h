#ifndef QRAPIDSHAREUSER_H
#define QRAPIDSHAREUSER_H
//

#include <QString>
#include <QDebug>
class QRapidshareUser  
{
public:
	QRapidshareUser(const QString &userName, const QString &pass);
	~QRapidshareUser();
	QString									getUserName()
	{
		return *m_apUserName;
	}
	QString									getUserPass()
	{
		return *m_apUserPass;
	}
	QString									ComposeCookie();
private:
	std::auto_ptr<QString>					m_apUserName;
	std::auto_ptr<QString>					m_apUserPass;
};
#endif
