noinst_PROGRAMS=test_boost_client test_boost_server

test_boost_client_SOURCES=TestBoostClient.cpp
test_boost_client_CXXFLAGS=-std=c++11
test_boost_client_LDADD=  $(top_srcdir)/src/lib/asionet/libasionet.a \
                    $(top_srcdir)/src/lib/system/libsystem.a \
                    $(top_srcdir)/src/lib/common/libcommon.a \
                    $(top_srcdir)/src/lib/concurrent/libconcurrent.a \
                    $(top_srcdir)/src/lib/json/libjson.a \
                    -lboost_system -lpthread

test_boost_server_SOURCES=TestBoostServer.cpp
test_boost_server_CXXFLAGS=-std=c++11
test_boost_server_LDADD=  $(top_srcdir)/src/lib/asionet/libasionet.a \
                    $(top_srcdir)/src/lib/system/libsystem.a \
                    $(top_srcdir)/src/lib/common/libcommon.a \
                    $(top_srcdir)/src/lib/concurrent/libconcurrent.a \
                    $(top_srcdir)/src/lib/json/libjson.a \
                    -lboost_system

INCLUDES=   -I$(top_srcdir)/src/include/common \
            -I$(top_srcdir)/src/include/concurrent \
            -I$(top_srcdir)/src/include/system \
            -I$(top_srcdir)/src/include/json \
            -I$(top_srcdir)/src/include/asionet 
