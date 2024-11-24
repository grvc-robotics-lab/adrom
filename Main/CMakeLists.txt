cmake_minimum_required(VERSION 2.8...3.5)

include_directories( ${ADROPMAN_SOURCE_DIR}/ModuleInterface )
include_directories( ${ADROPMAN_SOURCE_DIR}/OperationManager )

add_executable( ADROM Main.cpp )

# Link the executable to the libraries. The order of the libraries is important.
target_link_libraries( ADROM OperationManager -pthread )

