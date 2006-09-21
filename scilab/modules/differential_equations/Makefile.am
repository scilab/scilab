DIFFERENTIAL_EQUATIONS_C_SOURCES = src/c/dassl.c

DIFFERENTIAL_EQUATIONS_FORTRAN_SOURCES = src/fortran/rscar1.f \
src/fortran/prja.f \
src/fortran/vnorm.f \
src/fortran/lsoda.f \
src/fortran/ainvg.f \
src/fortran/lsode.f \
src/fortran/svcom1.f \
src/fortran/quarul.f \
src/fortran/solsy.f \
src/fortran/lsodi.f \
src/fortran/ddassl.f \
src/fortran/order.f \
src/fortran/epsalg.f \
src/fortran/cfode.f \
src/fortran/rscma1.f \
src/fortran/colnew.f \
src/fortran/dcutet.f \
src/fortran/xsetf.f \
src/fortran/dgbsl.f \
src/fortran/rkf45.f \
src/fortran/rchek.f \
src/fortran/dqag0.f \
src/fortran/xerrwv.f \
src/fortran/twodq.f \
src/fortran/dqags.f \
src/fortran/svcar1.f \
src/fortran/daux.f \
src/fortran/dgbfa.f \
src/fortran/rscom1.f \
src/fortran/rksimp.f \
src/fortran/roots.f \
src/fortran/rgk4.f \
src/fortran/stoda.f \
src/fortran/bnorm.f \
src/fortran/rchek2.f \
src/fortran/stode.f \
src/fortran/vmnorm.f \
src/fortran/prepj.f \
src/fortran/lsdisc.f \
src/fortran/fnorm.f \
src/fortran/ddasrt.f \
src/fortran/stodi.f \
src/fortran/xsetun.f \
src/fortran/intdy.f \
src/fortran/ewset.f \
src/fortran/roots2.f \
src/fortran/prepji.f \
src/fortran/svcma1.f 

GATEWAY_C_SOURCES = \
sci_gateway/c/sci_int2d.c \
sci_gateway/c/sci_int3d.c \
sci_gateway/c/sci_intg.c \
sci_gateway/c/sci_ode.c \
sci_gateway/c/sci_feval.c \
sci_gateway/c/gw_differential_equations1.c \
sci_gateway/c/gw_differential_equations2.c \
sci_gateway/c/gw_differential_equations3.c \
sci_gateway/c/gw_differential_equations4.c \
sci_gateway/c/sci_bvode.c \
sci_gateway/c/gw_differential_equations5.c \
sci_gateway/c/gw_differential_equations6.c

GATEWAY_FORTRAN_SOURCES = \
sci_gateway/fortran/Ex-ode-more.f \
sci_gateway/fortran/sci_dasrt.f \
sci_gateway/fortran/sci_feval.f \
sci_gateway/fortran/sci_odedc.f \
sci_gateway/fortran/sci_bvode.f \
sci_gateway/fortran/sci_impl.f \
sci_gateway/fortran/sci_dassl.f \
sci_gateway/fortran/sci_ode.f \
sci_gateway/fortran/bsurf.f \
sci_gateway/fortran/bsurfd.f \
sci_gateway/fortran/bydot.f \
sci_gateway/fortran/bydot2.f \
sci_gateway/fortran/bresd.f \
sci_gateway/fortran/bresid.f \
sci_gateway/fortran/bjac.f \
sci_gateway/fortran/bjacd.f \
sci_gateway/fortran/badd.f \
sci_gateway/fortran/bj2.f \
sci_gateway/fortran/bfeval.f \
sci_gateway/fortran/int2d.f \
sci_gateway/fortran/bint2d.f \
sci_gateway/fortran/bint3d.f \
sci_gateway/fortran/bintg.f \
sci_gateway/fortran/int3d.f \
sci_gateway/fortran/intg.f \
sci_gateway/fortran/Ex-odedc.f \
sci_gateway/fortran/Ex-ode.f \
sci_gateway/fortran/Ex-impl.f \
sci_gateway/fortran/Ex-int2d.f \
sci_gateway/fortran/Ex-int3d.f \
sci_gateway/fortran/Ex-intg.f \
sci_gateway/fortran/Ex-dasrt.f \
sci_gateway/fortran/Ex-dassl.f \
sci_gateway/fortran/Ex-bvode.f \
sci_gateway/fortran/bbvode.f \
sci_gateway/fortran/Ex-feval.f

libdifferential_equations_la_includedir=$(includedir)/differential_equations

libdifferential_equations_la_include_HEADERS = 
#includes/gw_differential_equations.h

libdifferential_equations_la_CFLAGS= 
# -Iincludes/ 
#-Isrc/c/
lib_LTLIBRARIES = libdifferential_equations.la

libdifferential_equations_la_SOURCES = $(DIFFERENTIAL_EQUATIONS_C_SOURCES) $(DIFFERENTIAL_EQUATIONS_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libdifferential_equations_la_DEPENDENCIES = \
	$(top_builddir)/libs/blas/libblas.la \
	$(top_builddir)/modules/elementaries_functions/libelementaries_functions.la 



libdifferential_equations_la_LIBADD = $(libdifferential_equations_la_DEPENDENCIES)

libdifferential_equations_la_PKGCONFIG = differential_equations.pc
