set(COMPONENT_NAME imagesource)

set(SRC
    gui/canrawfilter.ui
    gui/imagesourceguiimpl.h
    gui/imagesourceguiint.h
    imagesource.cpp
    imagesource.h
    imagesource_p.cpp
    imagesource_p.h
    imagesourcemodel.cpp
    imagesourcemodel.h
    imagesourceplugin.h
)

add_library(${COMPONENT_NAME} ${SRC})
target_link_libraries(${COMPONENT_NAME} cds-common rwqueue)
target_include_directories(${COMPONENT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})