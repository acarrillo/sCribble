AC_INIT(myconfig, version-0.1)
AC_PROG_CC
AC_CHECK_HEADER(SDL/SDL.h)
if test "$ac_cv_header_SDL_SDL" == no
then
    AC_MSG_NOTICE([Using local libsdl.])
    AC_SUBST(include,-I/home/students/2012/acarrillo/include/SDL)
fi
AC_OUTPUT(Makefile)
