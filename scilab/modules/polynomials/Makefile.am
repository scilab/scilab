##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006
##########

POLYNOMIALS_FORTRAN_SOURCES = src/fortran/residu.f \
src/fortran/imptra.f \
src/fortran/dmpadj.f \
src/fortran/impins.f \
src/fortran/dpmul1.f \
src/fortran/wprxc.f \
src/fortran/dmpcnc.f \
src/fortran/wmpmu.f \
src/fortran/mpinsp.f \
src/fortran/impext.f \
src/fortran/bezout.f \
src/fortran/recbez.f \
src/fortran/sfact1.f \
src/fortran/dmptld.f \
src/fortran/dpodiv.f \
src/fortran/dmptra.f \
src/fortran/dmpins.f \
src/fortran/dpsimp.f \
src/fortran/dmp2pm.f \
src/fortran/dmpext.f \
src/fortran/wpmul.f \
src/fortran/bezstp.f \
src/fortran/wmpad.f \
src/fortran/dprxc.f \
src/fortran/quadsd.f \
src/fortran/wmpadj.f \
src/fortran/dmpmu.f \
src/fortran/wpmul1.f \
src/fortran/wmpcnc.f \
src/fortran/mptri.f \
src/fortran/horner.f \
src/fortran/idegre.f \
src/fortran/fxshfr.f \
src/fortran/mpdiag.f \
src/fortran/matra.f \
src/fortran/dmpcle.f \
src/fortran/rpoly.f \
src/fortran/wpodiv.f \
src/fortran/wdmpmu.f \
src/fortran/wmptra.f \
src/fortran/wmpins.f \
src/fortran/mpdegr.f \
src/fortran/ddmpev.f \
src/fortran/realit.f \
src/fortran/dpmul.f \
src/fortran/sfact2.f \
src/fortran/dmpad.f \
src/fortran/nextk.f \
src/fortran/blktit.f \
src/fortran/dimin.f \
src/fortran/newest.f \
src/fortran/dwmpmu.f \
src/fortran/impcnc.f \
src/fortran/wdmpad.f \
src/fortran/wmpcle.f \
src/fortran/quadit.f \
src/fortran/quad.f \
src/fortran/calcsc.f \
src/fortran/writebufsfact.f


GATEWAY_C_SOURCES = sci_gateway/c/gw_polynomials.c \
sci_gateway/c/sci_sfact.c \
sci_gateway/c/sci_cleanp.c \
sci_gateway/c/sci_sum.c \
sci_gateway/c/sci_prod.c \
sci_gateway/c/sci_diag.c \
sci_gateway/c/sci_simp_mode.c \
sci_gateway/c/sci_roots.c \
sci_gateway/c/sci_varn.c \
sci_gateway/c/sci_simp.c \
sci_gateway/c/sci_bezout.c \
sci_gateway/c/sci_tril.c \
sci_gateway/c/sci_div.c \
sci_gateway/c/sci_coeff.c \
sci_gateway/c/sci_poly.c \
sci_gateway/c/sci_triu.c \
sci_gateway/c/sci_degree.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_f_simp.f \
sci_gateway/fortran/sci_f_poly.f \
sci_gateway/fortran/sci_f_varn.f \
sci_gateway/fortran/sci_f_sum.f \
sci_gateway/fortran/sci_f_div.f \
sci_gateway/fortran/sci_f_diag.f \
sci_gateway/fortran/sci_f_prod.f \
sci_gateway/fortran/polops.f \
sci_gateway/fortran/sci_f_bezout.f \
sci_gateway/fortran/sci_f_triu_tril.f \
sci_gateway/fortran/sci_f_roots.f \
sci_gateway/fortran/sci_f_psimp.f \
sci_gateway/fortran/sci_f_rsimp.f \
sci_gateway/fortran/sci_f_coeff.f \
sci_gateway/fortran/sci_f_degree.f \
sci_gateway/fortran/sci_f_pclean.f \
sci_gateway/fortran/sci_f_sfact.f \
sci_gateway/fortran/sci_f_simpmd.f



libscipolynomials_la_CFLAGS= 	-I$(srcdir)/includes/ \
				-I$(top_srcdir)/modules/api_scilab/includes \
				-I$(top_srcdir)/modules/output_stream/includes

pkglib_LTLIBRARIES = libscipolynomials.la

libscipolynomials_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION)

libscipolynomials_la_SOURCES = $(POLYNOMIALS_FORTRAN_SOURCES) $(GATEWAY_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES)

# For the code check (splint)
CHECK_SRC= ) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscipolynomials_la_CFLAGS)

libscipolynomials_la_LIBADD =  $(top_builddir)/modules/cacsd/libscicacsd.la $(top_builddir)/modules/elementary_functions/libscielementary_functions.la $(top_builddir)/modules/arnoldi/libsciarnoldi.la $(top_builddir)/modules/linear_algebra/libscilinear_algebra.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/modules/output_stream/libscioutput_stream.la 

#### Target ######
modulename=polynomials


#### polynomials : Conf files ####
libscipolynomials_la_rootdir = $(mydatadir)
libscipolynomials_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### polynomials : init scripts ####
libscipolynomials_la_etcdir = $(mydatadir)/etc
libscipolynomials_la_etc_DATA = etc/polynomials.quit etc/polynomials.start


#### polynomials : gateway declaration ####
libscipolynomials_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscipolynomials_la_sci_gateway_DATA = sci_gateway/polynomials_gateway.xml


include $(top_srcdir)/Makefile.incl.am
