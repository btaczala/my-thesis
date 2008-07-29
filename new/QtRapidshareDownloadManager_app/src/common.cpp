#include "common.h"

QString TimeToString()
{
	QDateTime now ( QDate::currentDate(), QTime::currentTime());
	return now.toString(QString(" dd.MM.yyyy - hh:mm:ss.zzz ")).remove('"');
};
