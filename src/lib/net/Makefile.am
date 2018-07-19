noinst_LIBRARIES=libnet.a
libnet_a_SOURCES=IPAddress.cpp SocketAddress.cpp Socket.cpp ClientSocket.cpp ServerSocket.cpp
libnet_a_CXXFLAGS=-D__LINUX__ -D__STDC_FORMAT_MACROS 
INCLUDES=   -I$(top_srcdir)/src/include/common \
            -I$(top_srcdir)/src/include/concurrent \
            -I$(top_srcdir)/src/include/json \
            -I$(top_srcdir)/src/include/net
