noinst_PROGRAMS=sysmonitor
sysmonitor_SOURCES=SysMonitor.cpp
sysmonitor_LDADD=  $(top_srcdir)/src/lib/monitor/libmonitor.a \
                $(top_srcdir)/src/lib/system/libsystem.a \
                $(top_srcdir)/src/lib/concurrent/libconcurrent.a \
                $(top_srcdir)/src/lib/common/libcommon.a \
                -lpthread
sysmonitor_CXXFLAGS=-D__LINUX__ 
INCLUDES=   -I$(top_srcdir)/src/include/common \
            -I$(top_srcdir)/src/include/system \
            -I$(top_srcdir)/src/include/monitor \
            -I$(top_srcdir)/src/include/concurrent
#export INCLUDES
