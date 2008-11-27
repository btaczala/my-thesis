
#include <QDialog>
#include <QApplication>
#include <QMainWindow>
#include <QDir>

#include "src/downloadmanager.h"
#include "src/downloadengine.h"
#include "src/enginemanager.h"
#include "src/optionscontainer.h"

#include "qtestapp.h"


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
    QApplication app(argc, argv); 
    DownloadManager dwnlMgr ; 
    
    DownloadEngine * pEngine = dwnlMgr.engineManager()->findEngine("rapidshare") ;

    OptionsContainer* options = new OptionsContainer();
    options->addOption(scRS_USER_NAME, std::string("4625386"));
    options->addOption(scRS_USER_PASS, std::string("pass..... "));
    pEngine->setOptionsForEngine( *options );
    delete options;

    dwnlMgr.addDownload("http://rapidshare.com/files/165900690/Puszcza_bukowa_.mp3",QDir::homePath().toStdString() );
    QTestApp t ; 
    t.setConnector( &dwnlMgr ) ;
    t.show();
    
	return app.exec();
}
