set(COMPONENT_NAME contourdetect)

set(SRC
    gui/contourdetect.ui
    gui/contourdetectguiimpl.h
    gui/contourdetectguiint.h
    contourdetect.cpp
    contourdetect.h
    contourdetect_p.h
    contourdetectmodel.cpp
    contourdetectmodel.h
    contourdetectinterface.h
    contourdetectplugin.h
)

add_library(${COMPONENT_NAME} STATIC ${SRC})
target_link_libraries(${COMPONENT_NAME} cds-common rwqueue)
target_include_directories(${COMPONENT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})