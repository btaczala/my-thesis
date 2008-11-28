CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT( proxy_lib )

#find_package ( Qt4 REQUIRED )
#find_package ( Boost REQUIRED ) 

#add_definitions( -DENGINES_BUILT_IN )
SET(CMAKE_VERBOSE_MAKEFILE OFF)
#SET(QT_USE_QTNETWORK 1)
INCLUDE_DIRECTORIES ( "src/" ) 
INCLUDE_DIRECTORIES ( "../DownloadManager_lib/src" )


SET( Proxy_SRCS 
	src/proxy.cpp
	src/settings.cpp
) 

SET( Proxy_HDRS
    src/proxy.h
	src/settings.h
)

add_library( proxy_lib STATIC ${Proxy_SRCS} ${Proxy_HDRS} )
#set ( Proxy_link_libraries 
#target_link_libraries ( proxy_lib) 
