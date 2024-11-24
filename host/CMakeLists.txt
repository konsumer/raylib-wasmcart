find_package(raylib)
find_package(physfs)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/main.c")
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/")

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
target_link_libraries(${PROJECT_NAME} PRIVATE raylib physfs-static)

# build for web
if (EMSCRIPTEN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -sUSE_GLFW=3  -sASYNCIFY -Oz -sSINGLE_FILE -sMALLOC=emmalloc  -sALLOW_MEMORY_GROWTH=1 -sEXPORTED_FUNCTIONS=_malloc,_free,_main")
    set(CMAKE_EXECUTABLE_SUFFIX ".mjs")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
       "$<TARGET_FILE:${PROJECT_NAME}>"
       "${PROJECT_SOURCE_DIR}/docs/$<TARGET_FILE_NAME:${PROJECT_NAME}>")
endif ()
