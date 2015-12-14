dnl ----------------------------------------------------------------------------
dnl @synopsis ACX_BLAS([ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]])
dnl
dnl This macro looks for a library that implements the BLAS
dnl linear-algebra interface (see http://www.netlib.org/blas/).
dnl On success, it sets the BLAS_LIBS output variable to
dnl hold the requisite library linkages.
dnl
dnl To link with BLAS, you should link with:
dnl
dnl 	$BLAS_LIBS $LIBS $FLIBS
dnl
dnl in that order.  FLIBS is the output variable of the
dnl AC_F77_LIBRARY_LDFLAGS macro (called if necessary by ACX_BLAS),
dnl and is sometimes necessary in order to link with F77 libraries.
dnl Users will also need to use AC_F77_DUMMY_MAIN (see the autoconf
dnl manual), for the same reason.
dnl
dnl Many libraries are searched for, from ATLAS to CXML to ESSL.
dnl The user may also use --with-blas=<lib> in order to use some
dnl specific BLAS library <lib>.  In order to link successfully,
dnl however, be aware that you will probably need to use the same
dnl Fortran compiler (which can be set via the F77 env. var.) as
dnl was used to compile the BLAS library.
dnl
dnl ACTION-IF-FOUND is a list of shell commands to run if a BLAS
dnl library is found, and ACTION-IF-NOT-FOUND is a list of commands
dnl to run it if it is not found.  If ACTION-IF-FOUND is not specified,
dnl the default action will define HAVE_BLAS.
dnl
dnl This macro requires autoconf 2.50 or later.
dnl
dnl @version acsite.m4,v 1.3 2002/08/02 09:28:12 steve Exp
dnl @author Steven G. Johnson <stevenj@alum.mit.edu>
dnl
AC_DEFUN([ACX_BLAS], [
AC_PREREQ(2.50)
AC_REQUIRE([AC_F77_LIBRARY_LDFLAGS])
acx_blas_ok=no
acx_blas_save_LIBS="$LIBS"

AC_ARG_WITH(blas-library,
            AC_HELP_STRING([--with-blas-library=DIR], [set the path to the BLAS (refblas, Atlas, MKL...) library]))
saved_ldflags="$LDFLAGS"


if test "$with_blas_library" != no -a "$with_blas_library" != ""; then
LDFLAGS="$LDFLAGS -L$with_blas_library"
fi	

# Get fortran linker names of BLAS functions to check for.
AC_F77_FUNC(sgemm)
AC_F77_FUNC(dgemm)

LIBS="$LIBS $FLIBS"

# First, check BLAS_LIBS environment variable
if test $acx_blas_ok = no; then
    if test "x$BLAS_LIBS" != x; then
        save_LIBS="$LIBS"; LIBS="$BLAS_LIBS $LIBS"
        AC_MSG_CHECKING([for $sgemm in $BLAS_LIBS])
        AC_TRY_LINK_FUNC($sgemm, [acx_blas_ok=yes; BLAS_TYPE="Using BLAS_LIBS environment variable"], [BLAS_LIBS=""])
        AC_MSG_RESULT($acx_blas_ok)
        LIBS="$save_LIBS"
    elif $WITH_DEVTOOLS; then # Scilab thirdparties
        BLAS_LIBS="-L$DEVTOOLS_LIBDIR -lblas"
        BLAS_TYPE="Generic Blas (thirdparties)"
        acx_blas_ok=yes
    fi
fi

# BLAS linked to by default?  (happens on some supercomputers)
if test $acx_blas_ok = no; then
	save_LIBS="$LIBS"; LIBS="$LIBS"
	AC_CHECK_FUNC($sgemm, [acx_blas_ok=yes; BLAS_TYPE="Linked"])
	LIBS="$save_LIBS"
fi

# BLAS in OpenBlas library (http://www.openblas.net/)
if test $acx_blas_ok = no; then
	AC_CHECK_LIB(openblas, $sgemm, [acx_blas_ok=yes; BLAS_TYPE="OpenBLAS"; BLAS_LIBS="-lopenblas"])
fi

# BLAS in ATLAS library (http://math-atlas.sourceforge.net/)
if test $acx_blas_ok = no; then
	PKG_CHECK_MODULES(BLAS, atlas, [acx_blas_ok=yes; BLAS_TYPE="Atlas"], [acx_blas_ok=no])
fi
if test $acx_blas_ok = no; then
	AC_CHECK_LIB(f77blas, $sgemm, [acx_blas_ok=yes; BLAS_TYPE="Atlas"; BLAS_LIBS="-lf77blas"], [
		AC_CHECK_LIB(f77blas, $sgemm, [acx_blas_ok=yes; BLAS_TYPE="Atlas"; BLAS_LIBS="-lf77blas -latlas"], [], [-latlas])])
fi

# BLAS in Intel MKL libraries (http://software.intel.com/en-us/articles/a-new-linking-model-single-dynamic-library-mkl_rt-since-intel-mkl-103)
if test $acx_blas_ok = no; then
	AC_CHECK_LIB(mkl_rt, $sgemm, [acx_blas_ok=yes; BLAS_TYPE="MKL"; BLAS_LIBS="-lmkl_rt"])
fi

# BLAS in PhiPACK libraries? (requires generic BLAS lib, too)
if test $acx_blas_ok = no; then
	AC_CHECK_LIB(blas, $sgemm,
		[AC_CHECK_LIB(dgemm, $dgemm,
		[AC_CHECK_LIB(sgemm, $sgemm,
			[acx_blas_ok=yes; BLAS_TYPE="PhiPACK"; BLAS_LIBS="-lsgemm -ldgemm -lblas"],
			[], [-lblas])],
			[], [-lblas])])
fi

# BLAS in Alpha CXML library?
if test $acx_blas_ok = no; then
	AC_CHECK_LIB(cxml, $sgemm, [acx_blas_ok=yes;BLAS_TYPE="Alpha CXML"; BLAS_LIBS="-lcxml"])
fi

# BLAS in Alpha DXML library? (now called CXML, see above)
if test $acx_blas_ok = no; then
	AC_CHECK_LIB(dxml, $sgemm, [acx_blas_ok=yes;BLAS_TYPE="Alpha DXML"; BLAS_LIBS="-ldxml"])
fi

# BLAS in Sun Performance library?
if test $acx_blas_ok = no; then
	if test "x$GCC" != xyes; then # only works with Sun CC
		AC_CHECK_LIB(sunmath, acosp,
			[AC_CHECK_LIB(sunperf, $sgemm,
        			[BLAS_LIBS="-xlic_lib=sunperf -lsunmath";
								BLAS_TYPE="Sun Performance library";
                                 acx_blas_ok=yes],[],[-lsunmath])])
	fi
fi

# BLAS in SCSL library?  (SGI/Cray Scientific Library)
if test $acx_blas_ok = no; then
	AC_CHECK_LIB(scs, $sgemm, [acx_blas_ok=yes; BLAS_TYPE="SCSL"; BLAS_LIBS="-lscs"])
fi

# BLAS in SGIMATH library?
if test $acx_blas_ok = no; then
	AC_CHECK_LIB(complib.sgimath, $sgemm,
		     [acx_blas_ok=yes; BLAS_TYPE="SGIMATH"; BLAS_LIBS="-lcomplib.sgimath"])
fi

# BLAS in IBM ESSL library? (requires generic BLAS lib, too)
if test $acx_blas_ok = no; then
	AC_CHECK_LIB(blas, $sgemm,
		[AC_CHECK_LIB(essl, $sgemm,
			[acx_blas_ok=yes; BLAS_TYPE="IBM ESSL"; BLAS_LIBS="-lessl -lblas"],
			[], [-lblas $FLIBS])])
fi

# Generic BLAS library?
if test $acx_blas_ok = no; then
	AC_CHECK_LIB(blas, $sgemm, [acx_blas_ok=yes; BLAS_TYPE="Generic Blas"; BLAS_LIBS="-lblas"])
fi

if test "$with_blas_library" != no -a "$with_blas_library" != ""; then
BLAS_LIBS="-L$with_blas_library $BLAS_LIBS"
fi

AC_SUBST(BLAS_LIBS)

LIBS="$acx_blas_save_LIBS"
LDFLAGS="$saved_ldflags"

# Finally, execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
if test x"$acx_blas_ok" = xyes; then
        ifelse([$1],,,[$1])
        :
else
        acx_blas_ok=no
        $2
fi
])dnl ACX_BLAS





dnl ----------------------------------------------------------------------------
dnl @synopsis ACX_LAPACK([ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]])
dnl
dnl This macro looks for a library that implements the LAPACK
dnl linear-algebra interface (see http://www.netlib.org/lapack/).
dnl On success, it sets the LAPACK_LIBS output variable to
dnl hold the requisite library linkages.
dnl
dnl To link with LAPACK, you should link with:
dnl
dnl     $LAPACK_LIBS $BLAS_LIBS $LIBS $FLIBS
dnl
dnl in that order.  BLAS_LIBS is the output variable of the ACX_BLAS
dnl macro, called automatically.  FLIBS is the output variable of the
dnl AC_F77_LIBRARY_LDFLAGS macro (called if necessary by ACX_BLAS),
dnl and is sometimes necessary in order to link with F77 libraries.
dnl Users will also need to use AC_F77_DUMMY_MAIN (see the autoconf
dnl manual), for the same reason.
dnl
dnl The user may also use --with-lapack-library=<DIR> in order to use some
dnl specific LAPACK library <lib>.  In order to link successfully,
dnl however, be aware that you will probably need to use the same
dnl Fortran compiler (which can be set via the F77 env. var.) as
dnl was used to compile the LAPACK and BLAS libraries.
dnl
dnl ACTION-IF-FOUND is a list of shell commands to run if a LAPACK
dnl library is found, and ACTION-IF-NOT-FOUND is a list of commands
dnl to run it if it is not found.  If ACTION-IF-FOUND is not specified,
dnl the default action will define HAVE_LAPACK.
dnl
dnl @version acsite.m4,v 1.3 2002/08/02 09:28:12 steve Exp
dnl @author Steven G. Johnson <stevenj@alum.mit.edu>

AC_DEFUN([ACX_LAPACK], [
AC_REQUIRE([ACX_BLAS])
acx_lapack_ok=no

AC_ARG_WITH(lapack-library,
            AC_HELP_STRING([--with-lapack-library=DIR], [set the path to the LAPACK library]))
saved_ldflags="$LDFLAGS"

if test "$with_lapack_library" != no -a "$with_lapack_library" != ""; then
LDFLAGS="$LDFLAGS -L$with_lapack_library"
fi


# Get fortran linker name of LAPACK function to check for.
AC_F77_FUNC(cheev)

# We cannot use LAPACK if BLAS is not found
if test "x$acx_blas_ok" != xyes; then
        acx_lapack_ok=noblas
fi

# First, check LAPACK_LIBS environment variable
if test "x$LAPACK_LIBS" != x; then
        save_LIBS="$LIBS"; LIBS="$LAPACK_LIBS $BLAS_LIBS $LIBS $FLIBS"
        AC_MSG_CHECKING([for $cheev in $LAPACK_LIBS])
        AC_TRY_LINK_FUNC($cheev, [acx_lapack_ok=yes; LAPACK_TYPE="LAPACK_LIBS env variable"], [LAPACK_LIBS=""])
        AC_MSG_RESULT($acx_lapack_ok)
        LIBS="$save_LIBS"
        if test acx_lapack_ok = no; then
                LAPACK_LIBS=""
        fi
elif $WITH_DEVTOOLS; then # Scilab thirdparties
     LAPACK_LIBS="-L$DEVTOOLS_LIBDIR -llapack -lblas"
     LAPACK_TYPE="Lapack (thirdparties)"
     acx_lapack_ok=yes
fi

# LAPACK linked to by default?  (is sometimes included in BLAS lib)
if test $acx_lapack_ok = no; then
        save_LIBS="$LIBS"; LIBS="$LIBS $BLAS_LIBS $FLIBS"
        AC_CHECK_FUNC($cheev, [acx_lapack_ok=yes; LAPACK_TYPE="Default link (may be provided with BLAS)"])
        LIBS="$save_LIBS"
fi

# Generic LAPACK library?
for lapack in lapack lapack_rs6k; do
        if test $acx_lapack_ok = no; then
                save_LIBS="$LIBS"; LIBS="$BLAS_LIBS $LIBS"
                AC_CHECK_LIB($lapack, $cheev,
                    [acx_lapack_ok=yes; LAPACK_TYPE="Library -l$lapack"; LAPACK_LIBS="-l$lapack"], [], [$FLIBS])
                LIBS="$save_LIBS"
        fi
done

LDFLAGS="$saved_ldflags"

if test "$with_lapack_library" != no -a "$with_lapack_library" != ""; then
LAPACK_LIBS="$LAPACK_LIBS -L$with_lapack_library"
fi

AC_SUBST(LAPACK_LIBS)

# Finally, execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
if test x"$acx_lapack_ok" = xyes; then
        ifelse([$1],,,[$1])
        :
else
        acx_lapack_ok=no
        $2
fi
])dnl ACX_LAPACK


dnl ----------------------------------------------------------------------------
dnl @synopsis ACX_ARPACK([ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]])
dnl
dnl This macro looks for a library that implements the ARPACK
dnl collection of Fortran77 subroutines designed to solve large 
dnl scale eigenvalue problems (http://forge.scilab.org/index.php/p/arpack-ng/).
dnl On success, it sets the ARPACK_LIBS output variable to
dnl hold the requisite library linkages.
dnl
dnl To link with ARPACK, you should link with:
dnl
dnl     $ARPACK_LIBS $BLAS_LIBS $LIBS $FLIBS
dnl
dnl in that order.  BLAS_LIBS is the output variable of the ACX_BLAS
dnl macro, called automatically.  FLIBS is the output variable of the
dnl AC_F77_LIBRARY_LDFLAGS macro (called if necessary by ACX_BLAS),
dnl and is sometimes necessary in order to link with F77 libraries.
dnl Users will also need to use AC_F77_DUMMY_MAIN (see the autoconf
dnl manual), for the same reason.
dnl
dnl The user may also use --with-arpack-library=<DIR> in order to use some
dnl specific ARPACK library <lib>.  In order to link successfully,
dnl however, be aware that you will probably need to use the same
dnl Fortran compiler (which can be set via the F77 env. var.) as
dnl was used to compile the ARPACK and BLAS libraries.
dnl
dnl ACTION-IF-FOUND is a list of shell commands to run if a ARPACK
dnl library is found, and ACTION-IF-NOT-FOUND is a list of commands
dnl to run it if it is not found.  If ACTION-IF-FOUND is not specified,
dnl the default action will define HAVE_ARPACK.
dnl
dnl @version acsite.m4,v 1.3 2002/08/02 09:28:12 steve Exp
dnl @author Steven G. Johnson <stevenj@alum.mit.edu>
dnl @author Sylvestre Ledru <sylvestre.ledru@scilab-enterprises.com>

AC_DEFUN([ACX_ARPACK], [
AC_REQUIRE([ACX_BLAS])
AC_REQUIRE([ACX_LAPACK])
acx_arpack_ok=no

AC_ARG_WITH(arpack-library,
            AC_HELP_STRING([--with-arpack-library=DIR], [set the path to the ARPACK library]))
saved_ldflags="$LDFLAGS"

if test "$with_arpack_library" != no -a "$with_arpack_library" != ""; then
LDFLAGS="$LDFLAGS -L$with_arpack_library"
fi

if $WITH_DEVTOOLS; then # Scilab thirdparties
    ARPACK_LIBS="-L$DEVTOOLS_LIBDIR -larpack -llapack -lblas"
    LDFLAGS="$LDFLAGS -L$DEVTOOLS_LIBDIR"
    acx_arpack_ok=yes
else
    ARPACK_LIBS="-larpack"
fi
# Get fortran linker name of ARPACK function to check for.
AC_F77_FUNC(znaupd)

# We cannot use ARPACK if BLAS is not found
if test "x$acx_blas_ok" != xyes -a "x$acx_lapack_ok" != xyes ; then
        acx_arpack_ok=noblas
fi

# First, check ARPACK_LIBS environment variable
if test "x$ARPACK_LIBS" != x; then
        save_LIBS="$LIBS"; LIBS="$ARPACK_LIBS $LAPACK_LIBS $BLAS_LIBS $LIBS $FLIBS"
        AC_MSG_CHECKING([for $znaupd in $ARPACK_LIBS])
        AC_TRY_LINK_FUNC($znaupd, [acx_arpack_ok=yes], [ARPACK_LIBS="-larpack"])
        AC_MSG_RESULT($acx_arpack_ok)
        LIBS="$save_LIBS"
        if test acx_arpack_ok = no; then
                ARPACK_LIBS=""
        fi
fi


LDFLAGS="$saved_ldflags"

if test "$with_arpack_library" != no -a "$with_arpack_library" != ""; then
ARPACK_LIBS="$ARPACK_LIBS -L$with_arpack_library"
fi

AC_SUBST(ARPACK_LIBS)

# Finally, execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
if test x"$acx_arpack_ok" = xyes; then
        ifelse([$1],,,[$1])
        :
else
        acx_arpack_ok=no
        $2
fi
])dnl ACX_ARPACK
dnl
dnl Check whether ARPACK works (does not crash)
dnl
dnl Using a pure Fortran program doesn't seem to crash when linked
dnl with the buggy ARPACK library but the C++ program does.  Maybe
dnl it is the memory allocation that exposes the bug and using statically
dnl allocated arrays in Fortran does not?
dnl
dnl Copyright (C) 1995-2012 John W. Eaton
dnl
dnl This code is released under the GPL license.
dnl
AC_DEFUN([CHECK_ARPACK_OK], [
  AC_LANG_PUSH(C++)
  save_LIBS="$LIBS";
  LIBS="$ARPACK_LIBS $LAPACK_LIBS $BLAS_LIBS $LIBS $FLIBS"
  save_LD_LIBRARY_PATH="$LD_LIBRARY_PATH"
  if $WITH_DEVTOOLS; then # Scilab thirdparties
      export LD_LIBRARY_PATH="$DEVTOOLS_LIBDIR:$LD_LIBRARY_PATH"
  fi
  AC_CACHE_CHECK([whether the arpack library works],
    [lib_cv_arpack_ok], [
      AC_RUN_IFELSE([AC_LANG_PROGRAM([[
// External functions from ARPACK library
extern "C" int
C2F(dnaupd) (int&, const char *, const int&, const char *,
                           int&, const double&, double*, const int&,
                           double*, const int&, int*, int*, double*,
                           double*, const int&, int&, long int, long int);

extern "C" int
C2F(dneupd) (const int&, const char *, int*, double*,
                           double*, double*, const int&,
                           const double&, const double&, double*,
                           const char*, const int&, const char *,
                           int&, const double&, double*, const int&,
                           double*, const int&, int*, int*, double*,
                           double*, const int&, int&, long int,
                           long int, long int);

extern "C" int
C2F(dgemv) (const char *, const int&, const int&,
                         const double&, const double*, const int&,
                         const double*, const int&, const double&,
                         double*, const int&, long int);

#include <cfloat>

void
doit (void)
{
  // Based on Octave function EigsRealNonSymmetricMatrix from liboctave/eigs-base.cc.

  // Problem matrix.  See bug #31479
  int n = 4;
  double *m = new double [n * n];
  m[0] = 1, m[4] = 0, m[8]  = 0, m[12] = -1;
  m[1] = 0, m[5] = 1, m[9]  = 0, m[13] = 0;
  m[2] = 0, m[6] = 0, m[10] = 1, m[14] = 0;
  m[3] = 0, m[7] = 0, m[11] = 2, m[15] = 1;

  double *resid = new double [4];

  resid[0] = 0.960966;
  resid[1] = 0.741195;
  resid[2] = 0.150143;
  resid[3] = 0.868067;

  int *ip = new int [11];

  ip[0] = 1;   // ishift
  ip[1] = 0;   // ip[1] not referenced
  ip[2] = 300; // mxiter, maximum number of iterations
  ip[3] = 1;   // NB blocksize in recurrence
  ip[4] = 0;   // nconv, number of Ritz values that satisfy convergence
  ip[5] = 0;   // ip[5] not referenced
  ip[6] = 1;   // mode
  ip[7] = 0;   // ip[7] to ip[10] are return values
  ip[8] = 0;
  ip[9] = 0;
  ip[10] = 0;
 
  int *ipntr = new int [14];

  int k = 1;
  int p = 3;
  int lwork = 3 * p * (p + 2);

  double *v = new double [n * (p + 1)];
  double *workl = new double [lwork + 1];
  double *workd = new double [3 * n + 1];

  int ido = 0;
  int info = 0;

  double tol = DBL_EPSILON;

  do 
    {
      C2F(dnaupd) (ido, "I", n, "LM", k, tol, resid, p,
                                 v, n, ip, ipntr, workd, workl, lwork,
                                 info, 1L, 2L);

      if (ido == -1 || ido == 1 || ido == 2)
        {
          double *x = workd + ipntr[0] - 1;
          double *y = workd + ipntr[1] - 1;

          C2F(dgemv) ("N", n, n, 1.0, m, n, x, 1, 0.0,
                                   y, 1, 1L);
        }
      else
        {
          if (info < 0)
            {
              return;  // Error
            }

          break;
        }
    } 
  while (1);

  int *sel = new int [p];

  // The dimensions of dr and di are k+1, but k+2 avoids segfault
  double *dr = new double [k + 1];
  double *di = new double [k + 1];
  double *workev = new double [3 * p];

  for (int i = 0; i < k + 1; i++)
    dr[i] = di[i] = 0.;

  int rvec = 1;

  double sigmar = 0.0;
  double sigmai = 0.0;

  // This is n*(k+1), but k+2 avoids segfault
  double *z = new double [n * (k + 1)];

  C2F(dneupd) (rvec, "A", sel, dr, di, z, n, sigmar,
                             sigmai, workev, "I", n, "LM", k, tol,
                             resid, p, v, n, ip, ipntr, workd,
                             workl, lwork, info, 1L, 1L, 2L);
}
]], [[
  for (int i = 0; i < 10; i++)
    doit ();
]])],
  [cv_lib_arpack_ok=yes],
  [cv_lib_arpack_ok=no],
  [cv_lib_arpack_ok=yes])])
  LIBS="$save_LIBS"
  export LD_LIBRARY_PATH="$save_LD_LIBRARY_PATH"
  AC_LANG_POP(C++)
  if test "$cv_lib_arpack_ok" = "yes"; then
    $1
  else
    $2
  fi
])
