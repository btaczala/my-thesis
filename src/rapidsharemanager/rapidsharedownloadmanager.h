#ifndef RAPIDSHAREDOWNLOADMANAGER_H
#define RAPIDSHAREDOWNLOADMANAGER_H

/**
	@author Bartek Taczała <b@kontrasty.szczecin.pl>
*/

#include <QString>
class RapidShareDownloadManager{
public:
	RapidShareDownloadManager();
	~RapidShareDownloadManager();
	void AddDownload(const QString & toDownload );	
};

#endif
