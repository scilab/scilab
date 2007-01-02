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
src/fortran/dmpdsp.f \
src/fortran/mptri.f \
src/fortran/horner.f \
src/fortran/idegre.f \
src/fortran/fxshfr.f \
src/fortran/mpdiag.f \
src/fortran/matra.f \
src/fortran/dmpcle.f \
src/fortran/rpoly.f \
src/fortran/wmptld.f \
src/fortran/wpodiv.f \
src/fortran/wdmpmu.f \
src/fortran/wmptra.f \
src/fortran/balbak.f \
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
src/fortran/fmt.f \
src/fortran/wmpcle.f \
src/fortran/quadit.f \
src/fortran/dmrdsp.f \
src/fortran/quad.f

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
sci_gateway/c/sci_eval.c \
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

#libpolynomials_la_FFLAGS= -Iincludes/ 
libpolynomials_la_CFLAGS= -Iincludes/ 
#-Isrc/c/
lib_LTLIBRARIES = libpolynomials.la

libpolynomials_la_SOURCES = $(POLYNOMIALS_FORTRAN_SOURCES) $(GATEWAY_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES)

libpolynomials_la_LIBADD =
# Cyclic dependencies : 
#	$(top_builddir)/modules/elementaries_functions/libelementaries_functions.la  
#	$(top_builddir)/modules/core/libcore.la 

libpolynomials_la_PKGCONFIG = polynomials.pc

datadir=${prefix}/modules
#### Target ######
target=$(datadir)/polynomials


#### polynomials : Conf files ####
libpolynomials_la_rootdir = $(target)
libpolynomials_la_root_DATA = changelog.txt licence.txt readme.txt VERSION


#### polynomials : Scilab macros ###
libpolynomials_la_macrodir = $(target)/macros
libpolynomials_la_macro_DATA = macros/denom.sci \
macros/pdiv.sci \
macros/systmat.sci \
macros/pol2str.sci \
macros/diophant.sci \
macros/htrianr.sci \
macros/pol2des.sci \
macros/rowcompr.sci \
macros/hrmt.sci \
macros/lcm.sci \
macros/cmndred.sci \
macros/colcompr.sci \
macros/invr.sci \
macros/determ.sci \
macros/pfactors.sci \
macros/coffg.sci \
macros/routh_t.sci \
macros/sylm.sci \
macros/numer.sci \
macros/hermit.sci \
macros/polfact.sci \
macros/horner.sci \
macros/gcd.sci \
macros/pol2tex.sci \
macros/inv_coeff.sci \
macros/detr.sci \
macros/derivat.sci \
macros/lcmdiag.sci \
macros/factors.sci macros/cmndred.bin \
macros/coffg.bin \
macros/colcompr.bin \
macros/denom.bin \
macros/derivat.bin \
macros/determ.bin \
macros/detr.bin \
macros/diophant.bin \
macros/factors.bin \
macros/gcd.bin \
macros/hermit.bin \
macros/horner.bin \
macros/hrmt.bin \
macros/htrianr.bin \
macros/inv_coeff.bin \
macros/invr.bin \
macros/lcmdiag.bin \
macros/lcm.bin \
macros/numer.bin \
macros/pdiv.bin \
macros/pfactors.bin \
macros/pol2des.bin \
macros/pol2str.bin \
macros/pol2tex.bin \
macros/polfact.bin \
macros/routh_t.bin \
macros/rowcompr.bin \
macros/sylm.bin \
macros/systmat.bin macros/names macros/lib macros/buildmacros.sce


#### polynomials : init scripts #### 
libpolynomials_la_etcdir = $(target)/etc
libpolynomials_la_etc_DATA = etc/polynomials.quit etc/polynomials.start

#### polynomials : gateway declaration #### 
libpolynomials_la_sci_gatewaydir = $(target)/sci_gateway
libpolynomials_la_sci_gateway_DATA = sci_gateway/polynomials



#### polynomials : include files ####
libpolynomials_la_includedir=$(includedir)/polynomials
libpolynomials_la_include_HEADERS = includes/gw_polynomials.h

include $(top_srcdir)/Makefile.incl.am

