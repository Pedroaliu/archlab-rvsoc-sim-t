add_executable(archlab-sim main.cpp)

target_link_libraries(archlab-sim PRIVATE archlab::sim)
target_compile_features(archlab-sim PRIVATE cxx_std_20)

archlab_set_project_warnings(archlab-sim)
archlab_enable_sanitizers(archlab-sim)
