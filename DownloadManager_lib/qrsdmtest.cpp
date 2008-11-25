#include <QDialog>
#include <QApplication>
#include <QMainWindow>
#include <QDir>
#include <boost/any.hpp>

#include "src/downloadmanager.h"
#include "src/downloadengine.h"
#include "src/enginemanager.h"

class QTwojStary : public QMainWindow
{
    Q_OBJECT
public:
    QTwojStary( QWidget *parent = NULL ) : QMainWindow(parent){};
};
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

    std::map<std::string,boost::any> options ; 
    options["RS_username"] = std::string("4625386");
    options["RS_userpassword"] = std::string("WM2FTZgx5Y");
    pEngine->setOptionsForEngine(options);


    dwnlMgr.addDownload("http://rapidshare.com/files/115886102/Dj_Bobo-Planet_Colors-upload_by_kreciq.rar",QDir::homePath().toStdString() );
    QTwojStary t ; 
    t.show();
	return app.exec();
}
