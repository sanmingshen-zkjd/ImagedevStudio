set(COMPONENT_NAME brightnesscontrast)

set(CMAKE_AUTOMOC ON)

set(SRC
    gui/brightnesscontrast.ui
    gui/brightnesscontrastguiimpl.h
    gui/brightnesscontrastguiint.h
    gui/qcustomplot/qcustomplot.cpp
    gui/qcustomplot/qcustomplot.h
    gui/imgplayer/editableitem.cpp
    gui/imgplayer/editableitem.h
    gui/imgplayer/graphicsview.cpp
    gui/imgplayer/graphicsview.h
    gui/imgplayer/lineitem.cpp
    gui/imgplayer/lineitem.h
    gui/imgplayer/polygonitem.cpp
    gui/imgplayer/polygonitem.h
    gui/imgplayer/rectitem.cpp
    gui/imgplayer/rectitem.h
    brightnesscontrast.cpp
    brightnesscontrast.h
    brightnesscontrast_p.h
    brightnesscontrastmodel.cpp
    brightnesscontrastmodel.h
    brightnesscontrastinterface.h
    brightnesscontrastplugin.h
)

add_library(${COMPONENT_NAME} STATIC ${SRC})
target_link_libraries(${COMPONENT_NAME} cds-common rwqueue Qt5::PrintSupport)
target_include_directories(${COMPONENT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
