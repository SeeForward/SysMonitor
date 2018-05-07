noinst_PROGRAMS=test_smartptr test_string test_path test_service
test_smartptr_SOURCES=TestSmartPtr.cpp
test_smartptr_LDADD=$(top_srcdir)/src/lib/common/libcommon.a \
                    $(top_srcdir)/src/lib/concurrent/libconcurrent.a 

test_string_SOURCES=TestString.cpp
test_string_LDADD=$(top_srcdir)/src/lib/common/libcommon.a \
                    $(top_srcdir)/src/lib/concurrent/libconcurrent.a 

test_path_SOURCES=TestPath.cpp
test_path_LDADD=$(top_srcdir)/src/lib/common/libcommon.a 

INCLUDES=   -I$(top_srcdir)/src/include/common \
            -I$(top_srcdir)/src/include/concurrent

test_service_SOURCES=TestService.cpp
test_service_LDADD= $(top_srcdir)/src/lib/system/libsystem.a \
                    $(top_srcdir)/src/lib/common/libcommon.a \
                    $(top_srcdir)/src/lib/concurrent/libconcurrent.a 

INCLUDES=   -I$(top_srcdir)/src/include/common \
            -I$(top_srcdir)/src/include/concurrent \
            -I$(top_srcdir)/src/include/system
#export INCLUDES
