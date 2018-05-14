noinst_PROGRAMS=test_monitor
test_monitor_SOURCES=TestMonitor.cpp
test_monitor_LDADD= $(top_srcdir)/src/lib/monitor/libmonitor.a \
                    $(top_srcdir)/src/lib/system/libsystem.a \
                    $(top_srcdir)/src/lib/concurrent/libconcurrent.a \
                    $(top_srcdir)/src/lib/common/libcommon.a \
                    -lpthread
INCLUDES=   -I$(top_srcdir)/src/include/common \
            -I$(top_srcdir)/src/include/system \
            -I$(top_srcdir)/src/include/monitor
#export INCLUDES
