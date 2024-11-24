cmake_minimum_required(VERSION 3.5)
project(null0)
set(CMAKE_C_STANDARD 11)

include(FetchContent)
# set(FETCHCONTENT_QUIET OFF)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
    set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake")

# everyone wants to make an "uninstall" target
set_property(GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS 1)

if (NOT EMSCRIPTEN)
    add_subdirectory(cart/c)
endif()

if (NOT CARTS_ONLY)
    add_subdirectory(host)
endif()
