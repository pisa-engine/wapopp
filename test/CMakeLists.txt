add_executable(test_detail test_detail.cpp)
target_include_directories(test_detail PRIVATE "${PROJECT_SOURCE_DIR}/include/")
target_link_libraries(test_detail
    nlohmann_json
    Catch2
)
add_test(test_detail test_detail)

add_executable(test_wapopp test_wapopp.cpp)
target_link_libraries(test_wapopp
    wapopp
    nlohmann_json
    Catch2
)
add_test(test_wapopp test_wapopp)
