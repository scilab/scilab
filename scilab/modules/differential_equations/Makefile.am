##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########

DIFFERENTIAL_EQUATIONS_C_SOURCES = src/c/dassl.c \
src/c/feval.c \
src/c/fevaltable.c \
src/c/fydottable.c \
src/c/impltable.c \
src/c/colnewtable.c \
src/c/intgtable.c \
src/c/int2dtable.c \
src/c/int3dtable.c \
src/c/fydot2table.c \
src/c/dassltable.c \
src/c/arnol.c \
src/c/rk4.c

DIFFERENTIAL_EQUATIONS_FORTRAN_SOURCES = src/fortran/rscar1.f \
src/fortran/bcomp.f \
src/fortran/lcomp.f \
src/fortran/loren.f \
src/fortran/prja.f \
src/fortran/vnorm.f \
src/fortran/lsoda.f \
src/fortran/lsodar.f \
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
src/fortran/greatr.f \
src/fortran/hpdel.f \
src/fortran/hpins.f \
src/fortran/svcar1.f \
src/fortran/daux.f \
src/fortran/dgbfa.f \
src/fortran/rscom1.f \
src/fortran/rksimp.f \
src/fortran/roots.f \
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
src/fortran/svcma1.f \
src/fortran/lsrgk.f \
src/fortran/odeint.f \
src/fortran/rkqc.f \
src/fortran/writbufode.f

GATEWAY_C_SOURCES = \
sci_gateway/c/sci_int2d.c \
sci_gateway/c/sci_int3d.c \
sci_gateway/c/sci_intg.c \
sci_gateway/c/sci_ode.c \
sci_gateway/c/sci_odc.c \
sci_gateway/c/sci_feval.c \
sci_gateway/c/gw_differential_equations1.c \
sci_gateway/c/gw_differential_equations2.c \
sci_gateway/c/gw_differential_equations3.c \
sci_gateway/c/gw_differential_equations4.c \
sci_gateway/c/sci_bvode.c \
sci_gateway/c/gw_differential_equations6.c

GATEWAY_FORTRAN_SOURCES = \
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
sci_gateway/fortran/bbvode.f


libscidifferential_equations_la_CFLAGS=  	-I$(srcdir)/includes/ \
						-I$(top_srcdir)/modules/output_stream/includes \
						-I$(top_srcdir)/modules/dynamic_link/includes \
						-I$(top_srcdir)/modules/api_scilab/includes \
						-I$(top_srcdir)/modules/operations/includes

pkglib_LTLIBRARIES = libscidifferential_equations.la

libscidifferential_equations_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscidifferential_equations_la_SOURCES = $(DIFFERENTIAL_EQUATIONS_C_SOURCES) $(DIFFERENTIAL_EQUATIONS_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

# For the code check (splint)
CHECK_SRC= $(DIFFERENTIAL_EQUATIONS_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscidifferential_equations_la_CFLAGS)

libscidifferential_equations_la_LIBADD =  $(top_builddir)/modules/core/libscicore.la $(top_builddir)/modules/elementary_functions/libscielementary_functions.la $(top_builddir)/modules/dynamic_link/libscidynamic_link.la $(top_builddir)/modules/arnoldi/libsciarnoldi.la $(top_builddir)/modules/string/libscistring.la $(top_builddir)/modules/output_stream/libscioutput_stream.la


#### Target ######
modulename=differential_equations


#### differential_equations : Conf files ####
libscidifferential_equations_la_rootdir = $(mydatadir)
libscidifferential_equations_la_root_DATA = changelog.txt license.txt readme.txt version.xml

#### differential_equations : init scripts ####
libscidifferential_equations_la_etcdir = $(mydatadir)/etc
libscidifferential_equations_la_etc_DATA = etc/differential_equations.quit etc/differential_equations.start

#### differential_equations : gateway declaration ####
libscidifferential_equations_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscidifferential_equations_la_sci_gateway_DATA = sci_gateway/differential_equations_gateway.xml

#### differential_equations : include files ####
libscidifferential_equations_la_includedir=$(pkgincludedir)/differential_equations
libscidifferential_equations_la_include_HEADERS = includes/gw_differential_equations1.h \
includes/gw_differential_equations2.h \
includes/gw_differential_equations3.h \
includes/gw_differential_equations4.h \
includes/gw_differential_equations6.h

include $(top_srcdir)/Makefile.incl.am
