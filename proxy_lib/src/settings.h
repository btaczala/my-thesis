#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <map>
#include <QSettings>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

namespace SettingsValNames
{
    // LIBRARY NAMES
    static const char scDefaultDownloadDirectory[] = "DefaultDownloadDirectory";
    static const char scContentLength[] = "ContentLength";
    static const char scMaxDownloads[] = "MaxDownloads";
    static const char scUseAppProxy[] = "useAppProxy";
    static const char scProxyUser[] = "proxyUser";
    static const char scProxyPass[] = "proxyPass";
    static const char scProxyServer[] = "proxyServer";
    static const char scProxyPort[] = "proxyPort";
    static const char scConfirmItemDelete[] = "confirmItemDelete";
    
    // UI NAMES
    static const char scColumnsInfo[] = "QDownloadWidgetColumnInfo";
    static const char scMainWindowGeometry[] = "MainWindowGeometry";
    //settings
    static const char scMinimize2Tray[] = "minimize2tray";
    static const char scClose2Tray[] = "close2tray";
    static const char scConfirmAppExit[] = "confirmAppExit";
    static const char scStartInTrayArea[] = "startInTrayArea";
    static const char scOnlyOneInstanceAllowed[] = "onlyOneInstanceAllowed";
    static const char scStartAppWithSystemStartup[] = "startAppWithSystemStartup";
    static const char scRecentDownloadFolders[] = "recentDownloadFolders";
    // PLUGINS NAMES
    static const char scPluginUsername[] = "username";
    static const char scPluginPassword[] = "password";
    static const char scUseCredentials[] = "useCredentials";
};
    
class OptionsContainer;

class Settings
{
    public:
        /**
         * @var Context of settings 
         */
        enum Context
        {
            LIBRARY = 0, 
            UI,
            PLUGINS,
            NOSUBGROUP  // all others 
        };
        /**
         * @var typedef for map of plugins settings 
         */
		typedef std::map<std::string, boost::shared_ptr<OptionsContainer> > PluginsOptions ; 
        /**
         * @brief Ctor
         */
		Settings();
        ~Settings();
        /**
         * @brief Return options for plugin
         * @param name name of plugin 
         * @return settings for plugin given by name
         */
        OptionsContainer *  optionsForEngine(const std::string & name ) ; 

        /**
         * @brief Sets a value in Settings
         * @param valName Name of settings value
         * @param value Value
         * @param context What is the context of settings value ( UI, PLUGIN, LIB ) 
         * @param subgroup It's used only with context = PLUGIN. This is the subgroup ( each plugin has a subgroup ) for plugin 
         * @see Context
         */
        void                setValue (const QString & valName, const QVariant & value, Context context = Settings::NOSUBGROUP, const QString & subgroup="") ; 
        /**
         * Returns value of setting
         * @param valName setting name
         * @param context What is the context of settings value ( UI, PLUGIN, LIB ) 
         * @param subgroup It's used only with context = PLUGIN. This is the subgroup ( each plugin has a subgroup ) for plugin 
         * @return 
         * @see setValue
         */
        QVariant            value( const QString & valName, Context context = Settings::NOSUBGROUP, const QString & subgroup="") ; 

        /**
         * load application settings. Should be invoked as soon as possible after application starts
         */
        void loadSettings() ; 
        /**
         * save application settings. 
         */
        void saveSettings() ;

        QString defaultDownloadDirectory() const ; 
        void    setDefaultDownloadDirectory() ;

        QString settingsDirectory() const ;
        QSettings       m_Settings ; 
	private:
        /**
         * load plugin options. sets group to "Plugin"
         */
        void loadPluginsOptions() ; 		
        /**
         * load plugin options. sets group to "UI"
         */
        void loadUIOptions() ;
        /**
         * load plugin options. sets group to "Lib"
         */
        void loadLibOptions() ;  
        
		
		PluginsOptions m_PluginsOptions ; 
        
};
#endif //__SETTINGS_H__