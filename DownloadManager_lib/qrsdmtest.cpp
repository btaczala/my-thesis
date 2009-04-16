#include <QApplication>
#include <QMainWindow>
#include "qtestapp.h"
#include "rslogger.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv); 
    QTestApp wnd ; 
    LOG( Proxy::settings()->m_Settings.fileName() );
    Proxy::settings()->setValue(SettingsValNames::scPluginUsername,QString("4625386"),Settings::PLUGINS, "rapidshare");
    Proxy::settings()->setValue(SettingsValNames::scPluginPassword,QString(Proxy::encrypt("maggot666_rs")),Settings::PLUGINS, "rapidshare");
    LOG ( Proxy::settings()->value(SettingsValNames::scPluginUsername,Settings::PLUGINS, "rapidshare") ) ; 
    LOG ( Proxy::decrypt( Proxy::settings()->value(SettingsValNames::scPluginPassword,Settings::PLUGINS, "rapidshare").toString() ) ) ; 
    wnd.addDownload("http://rapidshare.com/files/190593491/Rguls.DVDRip.XviD-SAPHiRE.part1.rar");
    wnd.show();
    ProfileLogger::DumpToQDebug();
    app.exec() ;
}
