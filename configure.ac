dnl written by: Oliver Cordes 2015-01-01
dnl changed by: Oliver Cordes 2016-03-09

AC_INIT([arctic], [1.2.1])
AC_CONFIG_SRCDIR([src/arctic.cc])
AM_INIT_AUTOMAKE


DATE=`date +%Y-%m-%d`
AC_DEFINE_UNQUOTED( BUILD, "$DATE", build date of package )
AC_CONFIG_MACRO_DIR([m4])
AM_CONFIG_HEADER(src/config.h)
AC_CONFIG_FILES(python_acs/python_wrapper)
AC_PROG_LIBTOOL
AC_PROG_CXX
AM_PROG_CC_C_O
AC_LANG_CPLUSPLUS 
AX_CXX_COMPILE_STDCXX_11
dnl AC_SUBST([AM_CPPFLAGS], [-Wall -g -O3])
dnl AC_PROG_RANLIB
LT_INIT
AC_HEADER_STDC
AC_CHECK_HEADERS(utime.h)
AC_CHECK_HEADERS(sys/time.h)
AC_CHECK_HEADERS(sys/resource.h)
dnl AC_EGREP_HEADER(utimbuf, utime.h, AC_DEFINE(HAVE_STRUCT_UTIMBUF))
AC_FUNC_UTIME_NULL
AC_CHECK_LIB(m,cos)
AC_CHECK_LIB([cfitsio], main,,AC_MSG_ERROR(cfitsio library not found))
AC_CHECK_HEADERS([fitsio.h],,AC_MSG_ERROR(cfitsio headers not found))
AC_CHECK_LIB(CCfits,main,,AC_MSG_ERROR(CCfits library not found))
AC_CHECK_HEADERS([CCfits/CCfits],,AC_MSG_ERROR(CCfits headers not found))

  
AC_OUTPUT( [ Makefile
             python_acs/Makefile ] )
