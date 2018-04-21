noinst_PROGRAMS=test_smartptr test_string
test_smartptr_SOURCES=TestSmartPtr.cpp
test_smartptr_LDADD=$(top_srcdir)/src/lib/common/libcommon.a \
                    $(top_srcdir)/src/lib/concurrent/libconcurrent.a 

test_string_SOURCES=TestString.cpp
test_string_LDADD=$(top_srcdir)/src/lib/common/libcommon.a \
                    $(top_srcdir)/src/lib/concurrent/libconcurrent.a 

INCLUDES=   -I$(top_srcdir)/src/include/common \
            -I$(top_srcdir)/src/include/concurrent
#export INCLUDES
