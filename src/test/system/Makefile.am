noinst_PROGRAMS=test_system
test_system_SOURCES=TestSystem.cpp
test_system_LDADD=  $(top_srcdir)/src/lib/monitor/libmonitor.a \
                    $(top_srcdir)/src/lib/system/libsystem.a  \
                    $(top_srcdir)/src/lib/concurrent/libconcurrent.a  \
                    $(top_srcdir)/src/lib/common/libcommon.a -lpthread
INCLUDES=   -I$(top_srcdir)/src/include/common \
            -I$(top_srcdir)/src/include/system \
            -I$(top_srcdir)/src/include/monitor \
            -I$(top_srcdir)/src/include/concurrent
#export INCLUDES
