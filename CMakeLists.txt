
cmake_minimum_required (VERSION 3.8)


add_executable (neural_network "ml.cpp")

set_target_properties(
    neural_network PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
   )


