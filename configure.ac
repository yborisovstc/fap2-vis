
AC_INIT([fap2-vis], [0.1], [yuri.borisov.v@gmail.com])

AM_INIT_AUTOMAKE([-Wall])

AC_CONFIG_FILES([
		 Makefile
		 src/Makefile
		 test/Makefile
		 ])


AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL

# Configure options: --enable-debug[=no].
AC_ARG_ENABLE([debug],
	      [AS_HELP_STRING([--enable-debug],
			      [enable debug code (default is no)])],
			      [debug="$withval"], [debug=no])


PKG_CHECK_MODULES(GDK, [gdk-2.0 >= 2.24.0], , [
    AC_MSG_ERROR([You must have 'gdk-2.0' dev headers (libgtk2.0-dev) installed
    ]) ])

AC_SUBST(GDK_CFLAGS)
AC_SUBST(GDK_LIBS)


PKG_CHECK_MODULES(FAPWS2, [libgrayb >= 0.1], , [
    AC_MSG_ERROR([You must have 'libgrayb' dev headers (libgrayb-dev) installed
    ]) ])

AC_SUBST(FAPWS2_CFLAGS)
AC_SUBST(FAPWS2_LIBS)


if test x$debug = xyes; then
    CXXFLAGS="-g -O0"
else
#    CXXFLAGS="-g -O2"
    CXXFLAGS="-g -O0"
fi


CXXFLAGS="$CXXFLAGS $FAPWS2_CFLAGS $GDK_CFLAGS" 
LDFLAGS="$LDFLAGS $FAP2WS_LIBS $GDK_LIBS"

AC_CONFIG_FILES([fap2vis.pc])

AC_OUTPUT

echo CXXFLAGS $CXXFLAGS
echo "
($PACKAGE_NAME) version $PACKAGE_VERSION
Prefix.........: $prefix
Debug Build....: $debug
C++ Compiler...: $CXX $CXXFLAGS $CPPFLAGS
Linker.........: $LD $LDFLAGS $LIBS
Doxygen........: ${DOXYGEN:-NONE}
"

