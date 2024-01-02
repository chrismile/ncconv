cmake_minimum_required (VERSION 3.7)
cmake_policy(VERSION 3.7...3.20)

project(ncconv)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.c src/*.hpp src/*.h)
include_directories(src)

find_package(Boost COMPONENTS system filesystem REQUIRED)
if(VCPKG_TOOLCHAIN)
    find_package(netCDF CONFIG REQUIRED)
else()
    find_package(NetCDF REQUIRED)
endif()

add_executable(ncconv ${SOURCES})

target_link_libraries(ncconv PRIVATE ${Boost_LIBRARIES})
target_include_directories(ncconv PRIVATE ${Boost_INCLUDE_DIR})
if(VCPKG_TOOLCHAIN)
    target_link_libraries(ncconv PRIVATE netCDF::netcdf)
else()
    target_link_libraries(ncconv PRIVATE ${NETCDF_LIBRARIES})
    target_include_directories(ncconv PRIVATE ${NETCDF_INCLUDE_DIR})
endif()
