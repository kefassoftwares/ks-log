set(LogE_SRCS Main.cpp)
include_directories("../src")

add_executable(Example ${LogE_SRCS} )
target_link_libraries(Example Log)
