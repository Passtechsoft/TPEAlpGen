cmake_minimum_required(VERSION 2.8)
#Déclaration du projet

project(PWorldGen CXX)
set(DEBUG "yes")

set(PWorldGen_VERSION_MAJOR 0)
set(PWorldGen_VERSION_MINOR 1)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/source")



find_package(Boost COMPONENTS system REQUIRED)
find_package(SFML 2 COMPONENTS graphics window system REQUIRED)
find_package(SFGUI REQUIRED)
find_package(Thor REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SFML_INCLUDE_DIR})
include_directories(${SFGUI_INCLUDE_DIR})
include_directories(${THOR_INCLUDE_DIR})



#Déclaration de l'exécutable
target_link_libraries(core ${Boost_LIBRARIES})
target_link_libraries(core ${SFML_LIBRARIES})
target_link_libraries(core ${THOR_LIBRARY})
target_link_libraries(core ${SFGUI_LIBRARY})

add_executable(
    ${PROJECT_NAME}

    ${SRC_DIR}/main.cpp
)

if ( ${DEBUG} EQUAL "yes" )
	#if( CMAKE_COMPILER_IS_GNUCXX )
	set( CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wunused-parameter -Wno-long-long -g" )
else()
	set( CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wunused-parameter -Wno-long-long -O2" )
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
install(DIRECTORY Data DESTINATION ${CMAKE_INSTALL_PREFIX}/

