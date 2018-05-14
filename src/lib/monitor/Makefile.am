noinst_LIBRARIES=libmonitor.a
libmonitor_a_SOURCES=   DiskInfo.cpp DiskIo.cpp DiskState.cpp DiskProc.cpp \
                        MemoryInfo.cpp MemoryUsage.cpp MemoryProc.cpp \
                        NetInfo.cpp TcpInode.cpp TcpConn.cpp NetListen.cpp AdapterIo.cpp \
                        CpuInfo.cpp CpuUsage.cpp CpuProc.cpp
libmonitor_a_CXXFLAGS=-D__LINUX__ -D__STDC_FORMAT_MACROS 
INCLUDES=   -I$(top_srcdir)/src/include/common \
            -I$(top_srcdir)/src/include/system \
            -I$(top_srcdir)/src/include/monitor \
            -I$(top_srcdir)/src/include/concurrent
