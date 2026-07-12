add_executable(
    archlab-tests
    test_main.cpp
    test_address_space.cpp
    test_event_queue.cpp
    test_stats_and_core.cpp
    test_sim_time.cpp
)

target_link_libraries(archlab-tests PRIVATE archlab::sim)
target_include_directories(archlab-tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(archlab-tests PRIVATE cxx_std_20)

archlab_set_project_warnings(archlab-tests)
archlab_enable_sanitizers(archlab-tests)

add_test(NAME archlab-tests COMMAND archlab-tests)
