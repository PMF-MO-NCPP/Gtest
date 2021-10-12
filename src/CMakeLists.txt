add_library(mylib mylib.cc mylib.h)
target_link_libraries(mylib fmt::fmt-header-only)

set_target_properties(mylib
    PROPERTIES
    CMAKE_CXX_STANDARD 17
    )
