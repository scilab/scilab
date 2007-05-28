##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006 
##########

# SUBDIRS = src/ocaml/

pkglib_LTLIBRARIES = libsciscicos.la

#### Target ######
modulename=scicos

include $(top_srcdir)/Makefile.incl.am

if SCICOS

SCICOS_C_SOURCES = src/c/cstblk4.c \
src/c/readau.c \
src/c/variable_delay.c \
src/c/absolute_value.c \
src/c/integral_func.c \
src/c/ratelimiter.c \
src/c/atanh_blk.c \
src/c/minmax.c \
src/c/samphold4.c \
src/c/ramp.c \
src/c/sciblk2i.c \
src/c/prod.c \
src/c/invblk4.c \
src/c/scalar2vector.c \
src/c/tan_blk.c \
src/c/atan_blk.c \
src/c/relay.c \
src/c/plusblk.c \
src/c/cos_blk.c \
src/c/time_delay.c \
src/c/acos_blk.c \
src/c/readc.c \
src/c/switchn.c \
src/c/evtvardly.c \
src/c/sin_blk.c \
src/c/asin_blk.c \
src/c/delay4.c \
src/c/multiplex.c \
src/c/cosh_blk.c \
src/c/tcsltj4.c \
src/c/sciblk2.c \
src/c/relationalop.c \
src/c/edgetrig.c \
src/c/sciblk4.c \
src/c/writec.c \
src/c/satur.c \
src/c/dmmul1.c \
src/c/signum.c \
src/c/sum.c \
src/c/bounce_ball.c \
src/c/zcross2.c \
src/c/modulo_count.c \
src/c/gainblk.c \
src/c/backlash.c \
src/c/logicalop.c \
src/c/hystheresis.c \
src/c/cscope.c \
src/c/summation.c \
src/c/selector.c \
src/c/deadband.c \
src/c/acosh_blk.c \
src/c/csslti4.c \
src/c/dsslti4.c \
src/c/step_func.c \
src/c/tcslti4.c \
src/c/sinh_blk.c \
src/c/dollar4.c \
src/c/evaluate_expr.c \
src/c/deriv.c \
src/c/extractor.c \
src/c/mswitch.c \
src/c/dmmul.c \
src/c/product.c \
src/c/writeau.c \
src/c/slider.c \
src/c/tanh_blk.c \
src/c/switch2.c \
src/c/cmscope.c \
src/c/asinh_blk.c \
src/c/cpass2.c \
src/c/ExistScicos.c \
src/c/haltscicos.c \
src/c/bouncexy.c

SCICOS_C_SOURCES_EXT = \
src/c/scicosclip.c \
src/c/blocks.c \
src/c/import.c \
src/c/scicos.c \
src/c/scicos_malloc.c \
src/c/scicos_free.c \
src/c/intconnection.c \
src/c/tree.c

SCICOS_FORTRAN_SOURCES = src/fortran/scitovv.f \
src/fortran/qzflr.f \
src/fortran/writef.f \
src/fortran/diffblk.f \
src/fortran/tcsltj.f \
src/fortran/dlradp.f \
src/fortran/sciblk.f \
src/fortran/lsplit.f \
src/fortran/evtdly.f \
src/fortran/mux.f \
src/fortran/ifthel.f \
src/fortran/tanblk.f \
src/fortran/intrp2.f \
src/fortran/hltblk.f \
src/fortran/zcross.f \
src/fortran/constraint.f \
src/fortran/mscope.f \
src/fortran/sum2.f \
src/fortran/scierr.f \
src/fortran/delay.f \
src/fortran/iocopy.f \
src/fortran/sawtth.f \
src/fortran/logblk.f \
src/fortran/maxblk.f \
src/fortran/sinblk.f \
src/fortran/eselect.f \
src/fortran/invblk.f \
src/fortran/ftree3.f \
src/fortran/scitoi.f \
src/fortran/memo.f \
src/fortran/qzrnd.f \
src/fortran/powblk.f \
src/fortran/fsv.f \
src/fortran/sqrblk.f \
src/fortran/pload.f \
src/fortran/dsslti.f \
src/fortran/skipvars.f \
src/fortran/tcslti.f \
src/fortran/intrpl.f \
src/fortran/scope.f \
src/fortran/list2vars.f \
src/fortran/intplt.f \
src/fortran/rndblk.f \
src/fortran/qztrn.f \
src/fortran/qzcel.f \
src/fortran/forblk.f \
src/fortran/trash.f \
src/fortran/gensin.f \
src/fortran/evscpe.f \
src/fortran/sctree.f \
src/fortran/sum3.f \
src/fortran/integr.f \
src/fortran/scifunc.f \
src/fortran/vvtosci.f \
src/fortran/mfclck.f \
src/fortran/gensqr.f \
src/fortran/csslti.f \
src/fortran/intpol.f \
src/fortran/scoxy.f \
src/fortran/samphold.f \
src/fortran/lookup.f \
src/fortran/minblk.f \
src/fortran/selblk.f \
src/fortran/dollar.f \
src/fortran/fscope.f \
src/fortran/affich.f \
src/fortran/readf.f \
src/fortran/demux.f \
src/fortran/cstblk.f \
src/fortran/timblk.f \
src/fortran/delayv.f \
src/fortran/scopxy.f \
src/fortran/scitod.f \
src/fortran/expblk.f \
src/fortran/ftree2.f \
src/fortran/lusat.f \
src/fortran/ftree4.f

SCICOS_FORTRAN_SOURCES_EXT = src/fortran/ddaskr.f \
src/fortran/lsodar2.f

GATEWAY_C_SOURCES = sci_gateway/c/sci_scicos_debug.c \
sci_gateway/c/sci_sci_tree3.c \
sci_gateway/c/sci_sci_tree4.c \
sci_gateway/c/gw_scicos.c \
sci_gateway/c/sci_scicos_debug_count.c \
sci_gateway/c/intcscicos.c \
sci_gateway/c/sci_curblock.c \
sci_gateway/c/gw_cscicos.c \
sci_gateway/c/sci_sctree.c \
sci_gateway/c/sci_var2vec.c \
sci_gateway/c/sci_vec2var.c \
sci_gateway/c/sci_getblocklabel.c \
sci_gateway/c/sci_sci_tree2.c

GATEWAY_FORTRAN_SOURCES =  sci_gateway/fortran/sci_debug_count.f \
sci_gateway/fortran/sci_tree2.f \
sci_gateway/fortran/sci_ctree.f \
sci_gateway/fortran/sci_tree3.f \
sci_gateway/fortran/sci_tree4.f \
sci_gateway/fortran/sci_curblock.f \
sci_gateway/fortran/sci_var2vec.f \
sci_gateway/fortran/sci_vec2var.f \
sci_gateway/fortran/sci_scicos_debug.f \
sci_gateway/fortran/sci_getblocklabel.f

BUILT_SOURCES = includes/blocks.h

includes/blocks.h:
	if test ! -d build; then mkdir build; fi
# Get the filename, strip the extension and put into a file
	for file in $(SCICOS_FORTRAN_SOURCES); do echo $$file |sed  's|.*/\([0-9A-Za-z_-]*\)\.f|\1|';done > build/Fblocknames 
	for file in $(SCICOS_C_SOURCES); do echo $$file |sed  's|.*/\([0-9A-Za-z_-]*\)\.c|\1|' ;done > build/Cblocknames
	$(top_srcdir)/modules/scicos/src/c/GenBlocks.sh build/Fblocknames build/Cblocknames includes/blocks.h


CLEANFILES=includes/blocks.h build/Cblocknames build/Fblocknames 

libsciscicos_la_CFLAGS=  -Iincludes/ -I$(top_srcdir)/libs/MALLOC/includes/
#libsciscicos_la_LDFLAGS = -lf2c

libsciscicos_la_SOURCES = $(SCICOS_C_SOURCES) $(SCICOS_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES) $(SCICOS_FORTRAN_SOURCES_EXT) $(SCICOS_C_SOURCES_EXT)

libsciscicos_la_LIBADD = \
	$(top_builddir)/modules/elementaries_functions/libscielementaries_functions.la \
	$(top_builddir)/modules/differential_equations/libscidifferential_equations.la \
	$(top_builddir)/modules/core/libscicore.la \
	$(top_builddir)/modules/time/libscitime.la \
	$(top_builddir)/modules/gui/libscigui.la \
	$(top_builddir)/modules/fileio/libscifileio.la \
	$(top_builddir)/modules/graphics/libscigraphics.la \
	$(top_builddir)/libs/MALLOC/libscimalloc.la


#### scicos : Conf files ####
libsciscicos_la_rootdir = $(mydatadir)
libsciscicos_la_root_DATA = changelog.txt licence.txt readme.txt VERSION.xml


#### scicos : init scripts #### 
libsciscicos_la_etcdir = $(mydatadir)/etc
libsciscicos_la_etc_DATA = etc/scicos.quit etc/scicos.start

#### scicos : gateway declaration #### 
libsciscicos_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciscicos_la_sci_gateway_DATA = sci_gateway/scicos_gateway.xml


#### scicos : include files ####
libsciscicos_la_includedir=$(pkgincludedir)/scicos
libsciscicos_la_include_HEADERS = includes/import.h \
includes/cc_pass2.h \
includes/scicos_block.h \
includes/scicos-def.h \
includes/gw_scicos.h \
includes/intcscicos.h \
includes/scicos_block4.h \
includes/gw_cscicos.h \
includes/scicos.h \
includes/blocks.h


#### scicos : demos files ####
libsciscicos_la_demosdir=$(mydatadir)
nobase_libsciscicos_la_demos_DATA = demos/CodeGen/Fibo_standalone.c \
demos/CodeGen/Therm.cosf \
demos/CodeGen/Therm_standalone.c \
demos/CodeGen/fibo.cosf \
demos/CodeGen/data \
demos/CodeGen/controller.cosf \
demos/CodeGen/foo.cosf \
demos/sound.cosf \
demos/dialogs_play.sci \
demos/Thermique_bloc.cosf \
demos/tut1 \
demos/tut2 \
demos/Temp.cosf \
demos/tut3 \
demos/demo2.cosf \
demos/tut4 \
demos/Ifsub.cosf \
demos/demo4.cosf \
demos/tut5 \
demos/Cont.Disc-Observer.cosf \
demos/tut6 \
demos/demo6.cosf \
demos/bounce.cosf \
demos/Rectifier.cosf \
demos/imppart_Hydraulics.mo \
demos/dialogs_learn.sci \
demos/scicos_learn.sci \
demos/scicos_play.sci \
demos/scicos.dem \
demos/rlc1.cosf \
demos/demo1.cosf \
demos/dialogs.sci \
demos/Hydraulics.cosf \
demos/demo3.cosf \
demos/System-Observer.cosf \
demos/demo5.cosf \
demos/demo7.cosf \
demos/live.dat \
demos/fibo.cosf \
demos/Controller.cosf

MACROSDIRSEXT= macros/scicos_blocks \
macros/scicos_blocks/Sources \
macros/scicos_blocks/Events \
macros/scicos_blocks/NonLinear \
macros/scicos_blocks/Electrical \
macros/scicos_blocks/Hydraulics \
macros/scicos_blocks/Sinks \
macros/scicos_blocks/PDE \
macros/scicos_blocks/Linear \
macros/scicos_blocks/Branching \
macros/scicos_blocks/Misc \
macros/scicos_blocks/Threshold \
macros/scicos

else

NO_SCICOS_C_SOURCES = src/c/noscicos/noscicos.c

libsciscicos_la_SOURCES = $(NO_SCICOS_C_SOURCES)

libsciscicos_la_includedir=$(pkgincludedir)/scicos
libsciscicos_la_include_HEADERS = src/c/noscicos/resource.h

#### scicos : init scripts #### 
libsciscicos_la_etcdir = $(mydatadir)/etc
libsciscicos_la_etc_DATA = etc/scicos.quit etc/scicos.start

endif


#### WORKAROUND ####
# Do not use the global clean-local rule because Scicos help is not managed
# like the rest of Scilab
clean-local:
	@for dir in $(MACRODIRS) $(MACROSDIRSEXT) ; do \
		echo "rm -f $(srcdir)/$$dir/$(MACROBINMASK)"; \
        rm -f $(srcdir)/$$dir/$(MACROBINMASK); \
    done; 
