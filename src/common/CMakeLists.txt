find_package(Threads REQUIRED)
#find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui PrintSupport)
include_directories(${CMAKE_SOURCE_DIR}/src/common ${CMAKE_SOURCE_DIR}/3rdParty/spdlog/include)

set(SRC
    componentmodel.h
    propertyfields.h
    candbhandler.cpp
    candbhandler.h
    candbpainter.cpp
    candbpainter.h
    sortmodel.cpp
    bcastmsgs.h
    componentinterface.h
    confighelpers.h
    context.h
    enumiterator.h
    log.h
    modelvisitor.h
    nodepainter.h
    plugin_type.h
    pluginloader.h
    propertyfields.h
    sortenums.h
    sortmodel.h
    visitablewith.h
    visitor.h
)

add_library(cds-common STATIC ${SRC})
target_include_directories(cds-common INTERFACE ${CMAKE_SOURCE_DIR}/src/common ${CMAKE_SOURCE_DIR}/3rdParty/spdlog/include)
target_link_libraries(cds-common ${CMAKE_THREAD_LIBS_INIT} nodes Qt5::Svg)

# if(WITH_TESTS)
#     add_executable(common_test tests/common_test.cpp)
#     target_link_libraries(common_test Qt5::Test cds-common fakeit projectconfig)
#     add_test(NAME common_test COMMAND common_test)
# endif()

