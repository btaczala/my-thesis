
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
    
    DownloadEngine * pEngine = dwnlMgr.engineManager()->findEngine("http") ;

    OptionsContainer options ;//= new OptionsContainer();
    options.addOption("username", std::string("4625386"));
    options.addOption("password", std::string("maggot666_rs"));
    pEngine->setOptionsForEngine( options ); 
    //delete options;
    /*
    http://rapidshare.com/files/166197404/Flashpoint.S02E01.PREAiR.PDTV.XviD-DiNA.part1.rar
    http://rapidshare.com/files/166196575/Flashpoint.S02E01.PREAiR.PDTV.XviD-DiNA.part2.rar
    http://rapidshare.com/files/166201581/Flashpoint.S02E01.PREAiR.PDTV.XviD-DiNA.part3.rar
    http://rapidshare.com/files/166212106/Flashpoint.S02E01.PREAiR.PDTV.XviD-DiNA.part4.rar
    */

   // dwnlMgr.addDownload("http://rapidshare.com/files/166197404/Flashpoint.S02E01.PREAiR.PDTV.XviD-DiNA.part1.rar",QDir::homePath().toStdString() );
    //dwnlMgr.addDownload("http://rapidshare.com/files/166196575/Flashpoint.S02E01.PREAiR.PDTV.XviD-DiNA.part2.rar",QDir::homePath().toStdString() );
    //dwnlMgr.addDownload("http://rapidshare.com/files/166201581/Flashpoint.S02E01.PREAiR.PDTV.XviD-DiNA.part3.rar",QDir::homePath().toStdString() );
    dwnlMgr.addDownload("http://sui-side.com/pinio/dach.jpg",QDir::homePath().toStdString() );
    QTestApp t ; 
    t.setConnector( &dwnlMgr ) ;
    t.show();
	return app.exec();
}
