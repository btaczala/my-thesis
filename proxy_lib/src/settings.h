#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <map>
#include <boost/shared_ptr.hpp>

class OptionsContainer;
class Settings
{
    public:
        /**
         * @brief Will return Download Manager 
         */
		typedef std::map<std::string, boost::shared_ptr<OptionsContainer> > PluginsOptions ; 
		Settings();
        OptionsContainer *  optionsForEngine(const std::string & name ) ; 
	private:
		void loadSettings() ; 
		void saveSettings() ;
		
		PluginsOptions m_PluginsOptions ; 
};
#endif //__SETTINGS_H__