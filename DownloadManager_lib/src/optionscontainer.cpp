#include <QtGlobal>
#include <QString>
#include <rslogger.h>

#include "optionscontainer.h"

OptionsContainer::OptionsContainer()
{

}

OptionsContainer::OptionsContainer( const OptionsContainer& options )
{
    options.clone( m_Options );
}

void OptionsContainer::clone( Options& options ) const
{
    Options::const_iterator it = m_Options.begin();
    while( it != m_Options.end() )
    {
        options[it->first] = it->second;
        ++it;
    }
}

void OptionsContainer::addOption( const std::string& key, const boost::any& value )
{
    m_Options[ key ] = value;
}
 
boost::any OptionsContainer::option( const std::string& key )
{
    Options::const_iterator it = m_Options.find(key);

    if ( it != m_Options.end() ) 
    {
        return it->second;
    }
	return boost::any(std::string(""));
}
