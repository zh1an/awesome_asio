message(STATUS ${CMAKE_CURRENT_LIST_DIR})

option(USE_VCPKG_TOOL "use vcpkg tool" ON)
if (${USE_VCPKG_TOOL})
    message(STATUS "use vcpkg tool")
    set(VcpkgRoot ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET})

    find_package(asio REQUIRED)
    option(USE_SPDLOG_TOOL "use spdlog module" ON)
    if (${USE_SPDLOG_TOOL})
        add_definitions(-DUSE_SPDLOG=true)
        add_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
        find_package(spdlog REQUIRED)
        find_package(fmt REQUIRED)
        target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt fmt::fmt-header-only)
    endif ()

    include_directories(
            ${CMAKE_CURRENT_LIST_DIR}
            ${VcpkgRoot}/include
    )
else ()
    include_directories(
            ${CMAKE_CURRENT_LIST_DIR}
            ${CMAKE_CURRENT_LIST_DIR}/../asio/asio/include
    )
endif ()