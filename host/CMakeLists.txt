find_package(raylib)
find_package(physfs)

# copy a taget build-product to the webroot
function(COPY_TO_WEB name)
  add_custom_command(TARGET ${name} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy
       "$<TARGET_FILE:${name}>"
       "${PROJECT_SOURCE_DIR}/docs/$<TARGET_FILE_NAME:${name}>")
endfunction()


add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE src/main.c)
if (EMSCRIPTEN)
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib physfs-static)
    set_target_properties(
        ${PROJECT_NAME} PROPERTIES
        SUFFIX ".mjs"
        COMPILE_FLAGS "-Oz"
        # add this for debugging: -sASSERTIONS=2
        LINK_FLAGS "-Oz -sFORCE_FILESYSTEM -sUSE_GLFW=3 -sASYNCIFY -sSINGLE_FILE -sMALLOC=emmalloc -sALLOW_MEMORY_GROWTH=1 -sEXPORTED_FUNCTIONS=_malloc,_free,_main"
    )
    COPY_TO_WEB(${PROJECT_NAME})
else()
    find_package(wamr)
    target_link_libraries(${PROJECT_NAME} PRIVATE raylib physfs-static vmlib)
endif()
