include(${PROJECT_SOURCE_DIR}/cmake/DownloadProject/DownloadProject.cmake)

download_project(
    PROJ googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG master
    UPDATE_DISCONNECTED 1
)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
add_executable(accel_core_test core/test.cpp)
target_compile_features(accel_core_test PRIVATE cxx_std_20)
target_link_libraries(accel_core_test PRIVATE accel gtest_main)


include(GoogleTest)
gtest_add_tests(TARGET accel_core_test)