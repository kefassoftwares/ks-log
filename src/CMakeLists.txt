set(Log_SRCS Log.cpp RealtimeTokenParser.cpp)
if (WIN32)
   set(Log_SRCS ${Log_SRCS} Log.h Token.h RealtimeTokenParser.h Utils.h)
endif (WIN32)

add_library(Log STATIC ${Log_SRCS} )
target_link_libraries(Log ${Boost_LIBRARIES})
