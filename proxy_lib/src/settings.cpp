#include "settings.h"
#include <optionscontainer.h>

Settings::Settings()
{
	boost::shared_ptr<OptionsContainer> shOptions ( new OptionsContainer() ) ;
	shOptions->addOption("username",std::string("4625386"));
	shOptions->addOption("password",std::string("pass"));
	m_PluginsOptions["rapidshare"] = shOptions ; 
};
OptionsContainer * Settings::optionsForEngine(const std::string & name )
{
    return m_PluginsOptions[name].get();
};
    