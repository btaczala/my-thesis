include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${Qwt5_INCLUDE_DIR} )


set(THESIS_SRCS  
main.cpp
mainwindow.cpp
UI/actions.cpp
UI/menus.cpp
UI/functionwidget.cpp
UI/functionedit.cpp
UI/functionplot.cpp
UI/stylesheets.cpp
math/function.cpp
math/parserfactory.cpp
Common/settings.cpp
)
set(THESIS_HDRS
mainwindow.h
UI/actions.h
UI/menus.h
UI/functionwidget.h
UI/functionplot.h
UI/functionedit.h
UI/stylesheets.h
math/function.h
math/parserfactory.h
Common/settings.h
)
qt4_automoc(${THESIS_SRCS})
qt4_wrap_cpp( THESIS_MOC mainwindow.h UI/functionwidget.h UI/functionedit.h  UI/functionplot.h) 
INCLUDE(${QT_USE_FILE})
add_executable(thesis ${THESIS_SRCS} ${THESIS_MOC})
target_link_libraries(thesis ${QT_LIBRARIES} ${Qwt5_Qt4_LIBRARY} muparser)
