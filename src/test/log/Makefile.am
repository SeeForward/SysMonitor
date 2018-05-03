noinst_PROGRAMS=test_log
test_log_SOURCES=TestLog.cpp
test_log_LDADD=  $(top_srcdir)/src/lib/date/libdate.a \
                $(top_srcdir)/src/lib/common/libcommon.a \
                $(top_srcdir)/src/lib/system/libsystem.a \
                $(top_srcdir)/src/lib/log/liblog.a 
INCLUDES=   -I$(top_srcdir)/src/include/common \
            -I$(top_srcdir)/src/include/system \
            -I$(top_srcdir)/src/include/date \
            -I$(top_srcdir)/src/include/log 
