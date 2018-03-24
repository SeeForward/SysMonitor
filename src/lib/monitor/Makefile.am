noinst_LIBRARIES=libmonitor.a
libmonitor_a_SOURCES=disk_info.cpp disk_io.cpp disk_state.cpp disk_proc.cpp memory_info.cpp memory_usage.cpp memory_proc.cpp net_info.cpp tcp_inode.cpp tcp_conn.cpp net_listen.cpp adapter_io.cpp cpu_info.cpp cpu_usage.cpp cpu_proc.cpp
libmonitor_a_CXXFLAGS=-D__LINUX__ -D__STDC_FORMAT_MACROS 
INCLUDES=-I$(top_srcdir)/src/include -I$(top_srcdir)/src/include/system -I$(top_srcdir)/src/include/monitor -I$(top_srcdir)/src/include/concurrent
