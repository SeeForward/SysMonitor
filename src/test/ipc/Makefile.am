noinst_PROGRAMS=test_ipc_msg test_ipc_send test_ipc_recv

test_ipc_msg_SOURCES=TestIpcMsg.cpp
test_ipc_msg_LDADD=  $(top_srcdir)/src/lib/ipc/libipc.a \
                $(top_srcdir)/src/lib/date/libdate.a \
                $(top_srcdir)/src/lib/common/libcommon.a \
                $(top_srcdir)/src/lib/concurrent/libconcurrent.a \
                $(top_srcdir)/src/lib/system/libsystem.a \
                $(top_srcdir)/src/lib/json/libjson.a 

test_ipc_send_SOURCES=TestIpcSend.cpp
test_ipc_send_LDADD= $(top_srcdir)/src/lib/ipc/libipc.a \
                $(top_srcdir)/src/lib/date/libdate.a \
                $(top_srcdir)/src/lib/common/libcommon.a \
                $(top_srcdir)/src/lib/concurrent/libconcurrent.a \
                $(top_srcdir)/src/lib/system/libsystem.a \
                $(top_srcdir)/src/lib/json/libjson.a \
                -lboost_system

test_ipc_recv_SOURCES=TestIpcRecv.cpp
test_ipc_recv_LDADD=  $(top_srcdir)/src/lib/ipc/libipc.a \
                $(top_srcdir)/src/lib/date/libdate.a \
                $(top_srcdir)/src/lib/common/libcommon.a \
                $(top_srcdir)/src/lib/concurrent/libconcurrent.a \
                $(top_srcdir)/src/lib/system/libsystem.a \
                $(top_srcdir)/src/lib/json/libjson.a \
                -lboost_system

INCLUDES=   -I$(top_srcdir)/src/include/ipc \
            -I$(top_srcdir)/src/include/common \
            -I$(top_srcdir)/src/include/concurrent \
            -I$(top_srcdir)/src/include/date \
            -I$(top_srcdir)/src/include/json 

