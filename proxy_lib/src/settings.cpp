#include "settings.h"
#include "proxy.h"


#include <optionscontainer.h>
#include <downloadengine.h>
#include <enginemanager.h>
#include <QStringList>
#include <rslogger.h>

Settings::Settings()
{
    // read all plugins options ;)
};
Settings::~ Settings()
{
    m_Settings.sync();
    RSDM_LOG_FUNC;
}
OptionsContainer * Settings::optionsForEngine(const std::string & name )
{
    return m_PluginsOptions[name].get();
};
void Settings::setValue(const QString & valName, const QVariant & value, Context context, const QString & subGroup )
{
    switch ( context ) 
    {
        case Settings::LIBRARY :
            m_Settings.beginGroup("Library");
            m_Settings.setValue(valName,value);
            m_Settings.endGroup();
            break ;
        case Settings::UI :
            m_Settings.beginGroup("UI");
            m_Settings.setValue(valName,value);
            m_Settings.endGroup();
            break ;
        case Settings::PLUGINS :
            m_Settings.beginGroup("Plugins");
            if ( !subGroup.isEmpty() )
                m_Settings.beginGroup(subGroup);
            m_Settings.setValue(valName,value);
            if ( !subGroup.isEmpty() )
                m_Settings.endGroup();
            m_Settings.endGroup();
            break ; 
        default:
            m_Settings.setValue(valName,value);
            break; 
    }
    m_Settings.sync();
}
QVariant Settings::value(const QString & valName, Context context, const QString & subGroup )
{
    QVariant ret ; 
    switch ( context ) 
    {
        case Settings::LIBRARY :
            m_Settings.beginGroup("Library");
            ret = m_Settings.value( valName );
            m_Settings.endGroup();
            break ;
        case Settings::UI :
            m_Settings.beginGroup("UI");
            ret = m_Settings.value( valName );
            m_Settings.endGroup();
            break ;
        case Settings::PLUGINS :
            m_Settings.beginGroup("Plugins");
            if ( !subGroup.isEmpty() )
                m_Settings.beginGroup(subGroup);
            ret = m_Settings.value( valName );
            if ( !subGroup.isEmpty() )
                m_Settings.endGroup();
            m_Settings.endGroup();
            break ; 
        default:
            ret = m_Settings.value( valName );
            break ; 
    }
    return ret ; 
    
}
void apply_options_to_engine ( const std::pair<std::string,boost::shared_ptr<DownloadEngine> >  & pair ) 
{
    pair.second->setOptionsForEngine( *( Proxy::settings()->optionsForEngine(pair.first) ) );
}
void Settings::loadSettings()
{
    /* plugins */
    EngineManager::EngineMap map = Proxy::engineManager()->engines() ; 
    loadPluginsOptions();
    std::for_each(map.begin(),map.end(),boost::bind( apply_options_to_engine, _1 ) ); 
}
void Settings::loadPluginsOptions()
{
    m_Settings.beginGroup("Plugins");
    EngineManager::EngineMap map = Proxy::engineManager()->engines() ; 
    EngineManager::EngineMap::iterator itEnd = map.end() ;
    
    for ( EngineManager::EngineMap::iterator it = map.begin() ; it != itEnd ; ++it ) 
    {
        boost::shared_ptr<OptionsContainer> shOptions ( new OptionsContainer() ) ;
        m_Settings.beginGroup( it->first.c_str() );
        QStringList allKeys = m_Settings.childKeys();
        Q_FOREACH(QString key, allKeys ) 
        {
            shOptions->addOption( key.toStdString() ,m_Settings.value(key).value<QString>().toStdString() );
        }
        m_Settings.endGroup();
        m_PluginsOptions[ it->first ] = shOptions ; 
    }
    m_Settings.endGroup();
};

void Settings::loadUIOptions()
{   
}
QString Settings::settingsDirectory() const
{
    return m_Settings.fileName() ; 
}