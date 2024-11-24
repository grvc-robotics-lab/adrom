cmake_minimum_required (VERSION 2.8...3.5)

add_library( OperationManager OperationManager.h OperationManager.cpp )

target_link_libraries (OperationManager ModuleInterface)

