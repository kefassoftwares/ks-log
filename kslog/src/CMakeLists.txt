set(Log_SRCS Log.cpp)
if (WIN32)
   set(Log_SRCS ${Log_SRCS} Log.h)
endif (WIN32)

add_library(Log STATIC ${Log_SRCS} )
