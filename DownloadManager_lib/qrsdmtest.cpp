
#include <QDialog>
#include <QApplication>
#include "downloadmanager.h"
int main(int argc, char** argv)
{
	/*
	QApplication app(argc, argv); 
	QDialog tmp ; 
	RapidShareDownloadManager foo;
	foo.SetUser(QRapidshareUser ("4625386","WM2FTZgx5Y") );
	foo.AddDownload(
		QString("http://rapidshare.com/files/88058939/Moonspell_-_1994_-_Anno_Satanae.rar"),
		QString(QDir::homePath() + "/ziom")
		);
	foo.Start() ;
	tmp.show() ;
	app.exec() ;
	*/
    DownloadManager dwnlMgr ; 
    DownloadEngine * pEngine = dwnlMgr->findEngine("rapidshare");
    std::map<std::string,void *> options ; 
    options["RS_username"] = (void*)(&(std::string("4625386")));
    options["RS_userpass"] = (void*)(&(std::string("WM2FTZgx5Y")));
    pEngine->setOptionsForEngine(options);
    dwnlMgr.addDownload("http://rapidshare.com/files/88058939/Moonspell_-_1994_-_Anno_Satanae.rar",QDir::homePath());
	return 0;
}
