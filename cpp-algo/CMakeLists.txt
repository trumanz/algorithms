cmake_minimum_required(VERSION 2.8)

project(cpp-algo)
set(ROOT_PATH ${CMAKE_SOURCE_DIR})

set (CMAKE_CXX_STANDARD 11)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set( CMAKE_VERBOSE_MAKEFILE on )


function(add_source_files dir regex_name dst)
    file(GLOB_RECURSE current_files ${dir}/${regex_name})
    set(${dst} ${current_files} ${${dst}} PARENT_SCOPE)

    if(MSVC)
        source_group(TREE ${dir} FILES ${current_files})
    endif()
endfunction()


add_source_files(${CMAKE_CURRENT_SOURCE_DIR}/src *.cpp  CPP_FILES)

add_executable(${PROJECT_NAME} ${CPP_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3pp/tabulate/include)

if( WIN32 )
    set(__depend_libs gtest pthread)
    generate_gen_lib("${__depend_libs}" depend_libs d)
else()
    set(depend_libs gtest pthread)
endif()


target_link_libraries(
    ${PROJECT_NAME} 
    ${depend_libs}
    )

if(WIN32)
    set_property(TARGET ${PROJECT_NAME} PROPERTY DEBUG_POSTFIX d)
endif()


