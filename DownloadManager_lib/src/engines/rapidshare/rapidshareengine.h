/***************************************************************************
 *   Copyright (C) 2008 by Bartek Taczała                                  *
 *   b@kontrasty.szczecin.pl                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef RAPIDSHAREENGINE_H
#define RAPIDSHAREENGINE_H


#include <map>  
#include <algorithm>
#include <boost/any.hpp>

#include "downloadengine.h"

#define  RS_USER_NAME std::string("RS_username")
#define  RS_USER_PASS "RS_userpassword"

static const std::string scRS_USER_NAME = std::string("RS_username");
static const std::string scRS_USER_PASS = std::string("RS_userpassword");


class RapidshareEngine : public DownloadEngine
{
    public:
        RapidshareEngine();
        virtual void            setOptionsForEngine( const std::map<std::string, boost::any> & options ) ; 
        virtual IDownload *     spawn() const; 
        const std::string &     username() const; 
        const std::string &     userpass() const; 
    private:
        std::string             m_UserName ;
        std::string             m_UserPass ;
};
#endif // RAPIDSHAREENGINE_H
