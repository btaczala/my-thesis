# find_package(Qt4 REQUIRED )
# find_package(Boost REQUIRED )
# if(WIN32)
# ADD_DEFINITIONS(  )
# else(WIN32)
# ADD_DEFINITIONS( -Wall -O2 )
# endif(WIN32)
# set(QT_USE_QTNETWORK 1)
# include_directories ( "./" ) 
# 
# 
# SET( SRCS 
#     downloadmanager.cpp
#     downloadengine.cpp
#     idownload.cpp
#     rapidsharedownloadmanager.cpp 
#     rslogger.cpp 
#     rscommon.cpp 
#     enginemanager.cpp
#     engines/rapidshare/qrapidsharedownload.cpp
#     engines/rapidshare/qrapidshareuser.cpp
#     engines/rapidshare/rapidshareengine.cpp
#     ) 
# SET( HDRS 
#     downloadmanager.h
#     downloadengine.h
#     idownload.h
#     rapidsharedownloadmanager.h 
#     rslogger.h 
#     rscommon.h
#     enginemanager.h
#     engines/rapidshare/qrapidsharedownload.h
#     engines/rapidshare/qrapidshareuser.h
#     engines/rapidshare/rapidshareengine.h
#     )
# 
# QT4_WRAP_CPP(QT_MOC_SRC engines/rapidshare/qrapidsharedownload.h rapidsharedownloadmanager.h downloadmanager.h )
# INCLUDE(${QT_USE_FILE})
# add_library( downloadmanager_lib STATIC ${QT_MOC_SRC} ${SRCS} ${HDRS} )
# target_link_libraries( downloadmanager_lib ${QT_LIBRARIES} )
# 
# 
