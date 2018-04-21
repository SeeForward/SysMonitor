#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([monitor], [1.0], [v-lidengke@foxmail.com])
AC_CONFIG_SRCDIR([src/app/sys_monitor.cpp])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE(monitor, 1.0)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h inttypes.h mntent.h stdlib.h string.h sys/ioctl.h sys/param.h sys/socket.h sys/vfs.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_GETMNTENT
AC_CHECK_FUNCS([getmntent inet_ntoa memset socket strchr strstr uname])

AC_CONFIG_FILES([Makefile
                 src/app/Makefile
                 src/lib/common/Makefile
                 src/lib/concurrent/Makefile
                 src/lib/monitor/Makefile
                 src/lib/net/Makefile
                 src/lib/system/Makefile
                 src/test/common/Makefile
                 src/test/concurrent/Makefile
                 src/test/monitor/Makefile
                 src/test/net/Makefile
                 src/test/system/Makefile
                 ])
AC_OUTPUT
