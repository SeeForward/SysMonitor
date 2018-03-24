noinst_PROGRAMS=test_system
test_system_SOURCES=test_system.cpp
test_system_LDADD=$(top_srcdir)/src/lib/monitor/libmonitor.a $(top_srcdir)/src/lib/system/libsystem.a  $(top_srcdir)/src/lib/concurrent/libconcurrent.a  $(top_srcdir)/src/lib/libcommon.a
INCLUDES=-I$(top_srcdir)/src/include -I$(top_srcdir)/src/include/system -I$(top_srcdir)/src/include/monitor -I$(top_srcdir)/src/include/concurrent
#export INCLUDES
