noinst_PROGRAMS=test_concurrent test_thread
test_concurrent_SOURCES=TestConcurrent.cpp
test_concurrent_LDADD=$(top_srcdir)/src/lib/concurrent/libconcurrent.a 
test_thread_SOURCES=TestThread.cpp
test_thread_LDADD=  $(top_srcdir)/src/lib/system/libsystem.a \
                    $(top_srcdir)/src/lib/common/libcommon.a \
                    $(top_srcdir)/src/lib/concurrent/libconcurrent.a \
                    -lpthread
INCLUDES=   -I$(top_srcdir)/src/include \
            -I$(top_srcdir)/src/include/common \
            -I$(top_srcdir)/src/include/system \
            -I$(top_srcdir)/src/include/concurrent
#export INCLUDES
