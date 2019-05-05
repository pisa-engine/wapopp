EXECUTE_PROCESS(COMMAND git submodule update --init
                WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/..
                OUTPUT_QUIET
        )

# Add JSON for Modern C++
if(NOT TARGET nlohmann_json)
    set(JSON_BuildTests OFF CACHE INTERNAL "")
    add_subdirectory(json EXCLUDE_FROM_ALL)
endif()

# Add CLI11
if(NOT TARGET CLI11)
    set(CLI11_TESTING OFF CACHE INTERNAL "")
    add_subdirectory(CLI11)
endif()

# Add Catch
if(NOT TARGET Catch2)
    add_subdirectory(Catch2)
endif()
