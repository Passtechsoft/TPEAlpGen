#Déclaration du projet

project(PWorldGen CXX)
set(debug "yes")

set(PWorldGen_VERSION_MAJOR 0)
set(PWorldGen_VERSION_MINOR 1)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/source")

#Déclaration de l'exécutable

add_executable(
    PWGenExe
    
    ${SRC_DIR}/IO.h
    ${SRC_DIR}/IO.cpp
    
    ${SRC_DIR}/World.h
    ${SRC_DIR}/World.cpp
    
    ${SRC_DIR}/Zone.h
    ${SRC_DIR}/Zone.cpp
    
    ${SRC_DIR}/Cell.h
    ${SRC_DIR}/Cell.cpp

    ${SRC_DIR}/main.cpp
)

if ( DEBUG == "yes" )
	#if( CMAKE_COMPILER_IS_GNUCXX )
	set( CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wunused-parameter -Wno-long-long -pedantic -g" )
else		
	set( CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wunused-parameter -Wno-long-long -pedantic -O2" )

