noinst_PROGRAMS=test_ip test_client test_server
test_ip_SOURCES=test_ip.cpp
test_ip_LDADD=  $(top_srcdir)/src/lib/net/libnet.a \
                $(top_srcdir)/src/lib/system/libsystem.a \
                $(top_srcdir)/src/lib/common/libcommon.a
test_client_SOURCES=test_client.cpp
test_client_LDADD=  $(top_srcdir)/src/lib/net/libnet.a \
                    $(top_srcdir)/src/lib/system/libsystem.a \
                    $(top_srcdir)/src/lib/common/libcommon.a
test_server_SOURCES=test_server.cpp
test_server_LDADD=  $(top_srcdir)/src/lib/net/libnet.a \
                    $(top_srcdir)/src/lib/system/libsystem.a \
                    $(top_srcdir)/src/lib/common/libcommon.a
INCLUDES=   -I$(top_srcdir)/src/include/common \
            -I$(top_srcdir)/src/include/system \
            -I$(top_srcdir)/src/include/net 
