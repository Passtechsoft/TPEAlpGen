#Déclaration du projet

project(PWorldGen CXX)
set(DEBUG "yes")

set(PWorldGen_VERSION_MAJOR 0)
set(PWorldGen_VERSION_MINOR 1)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/source")

#Déclaration de l'exécutable

add_executable(
    PWGenExe
    
    ${SRC_DIR}/Interface/IO.h
    ${SRC_DIR}/Interface/IO.cpp
    
    ${SRC_DIR}/Core/Zone.h
    ${SRC_DIR}/Core/Zone.cpp
    
    ${SRC_DIR}/Core/Couche.h
    ${SRC_DIR}/Core/Couche.cpp
    
    ${SRC_DIR}/Core/World.h
    ${SRC_DIR}/Core/World.cpp

    ${SRC_DIR}/main.cpp
)

if ( ${DEBUG} EQUAL "yes" )
	#if( CMAKE_COMPILER_IS_GNUCXX )
	set( CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wunused-parameter -Wno-long-long -pedantic -g" )
else()
	set( CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wunused-parameter -Wno-long-long -pedantic -O2" )
endif()
