noinst_LIBRARIES=libsystem.a
libsystem_a_SOURCES=SystemInfo.cpp  SystemTime.cpp  SystemTool.cpp \
                    ProcessTime.cpp ProcessName.cpp
libsystem_a_CXXFLAGS=-D__LINUX__ -D__STDC_FORMAT_MACROS 
INCLUDES=   -I$(top_srcdir)/src/include/common \
            -I$(top_srcdir)/src/include/system \
            -I$(top_srcdir)/src/include/concurrent
