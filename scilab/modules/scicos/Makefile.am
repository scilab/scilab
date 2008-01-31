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

SCICOS_C_SOURCES = src/c/ExistScicos.c

GATEWAY_C_SOURCES = sci_gateway/c/gw_cscicos.c \
sci_gateway/c/gw_scicos.c \
sci_gateway/c/sci_buildouttb.c \
sci_gateway/c/sci_ctree2.c \
sci_gateway/c/sci_ctree3.c \
sci_gateway/c/sci_ctree4.c \
sci_gateway/c/sci_curblock.c \
sci_gateway/c/sci_curblockc.c \
sci_gateway/c/sci_diffobjs.c \
sci_gateway/c/sci_duplicate.c \
sci_gateway/c/sci_end_scicosim.c \
sci_gateway/c/sci_getblocklabel.c \
sci_gateway/c/sci_getscicosvars.c \
sci_gateway/c/sci_permutobj.c \
sci_gateway/c/sci_phase_simulation.c \
sci_gateway/c/sci_pointer_xproperty.c \
sci_gateway/c/sci_sci_tree2.c \
sci_gateway/c/sci_sci_tree3.c \
sci_gateway/c/sci_sci_tree4.c \
sci_gateway/c/sci_scicos_debug.c \
sci_gateway/c/sci_scicos_debug_count.c \
sci_gateway/c/sci_scicos_time.c \
sci_gateway/c/sci_scicosim.c \
sci_gateway/c/sci_sctree.c \
sci_gateway/c/sci_set_blockerror.c \
sci_gateway/c/sci_set_xproperty.c \
sci_gateway/c/sci_var2vec.c \
sci_gateway/c/sci_vec2var.c \
sci_gateway/c/sci_xstringb2.c 



#BUILT_SOURCES = includes/blocks.h

#includes/blocks.h:
#	if test ! -d build; then mkdir build; fi
# Get the filename, strip the extension and put into a file
#	for file in $(SCICOS_FORTRAN_SOURCES); do echo $$file |sed  's|.*/\([0-9A-Za-z_-]*\)\.f|\1|';done > build/#Fblocknames
#	for file in $(SCICOS_C_SOURCES); do echo $$file |sed  's|.*/\([0-9A-Za-z_-]*\)\.c|\1|' ;done > build/Cblocknames
#	$(top_srcdir)/modules/scicos/src/c/GenBlocks.sh build/Fblocknames build/Cblocknames includes/blocks.h


#CLEANFILES=includes/blocks.h build/Cblocknames build/Fblocknames

libsciscicos_la_CFLAGS= -Iincludes/ \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes
#libsciscicos_la_LDFLAGS = -lf2c

libsciscicos_la_SOURCES = $(SCICOS_C_SOURCES) $(GATEWAY_C_SOURCES) 

libsciscicos_la_LIBADD = \
	$(top_builddir)/modules/elementary_functions/libscielementary_functions.la \
	$(top_builddir)/modules/differential_equations/libscidifferential_equations.la \
	$(top_builddir)/modules/core/libscicore.la \
	$(top_builddir)/modules/time/libscitime.la \
	$(top_builddir)/modules/gui/libscigui.la \
	$(top_builddir)/modules/fileio/libscifileio.la \
	$(top_builddir)/modules/graphics/libscigraphics.la \
	$(top_builddir)/libs/MALLOC/libscimalloc.la


#### scicos : Conf files ####
libsciscicos_la_rootdir = $(mydatadir)
libsciscicos_la_root_DATA = changelog.txt license.txt readme.txt version.xml


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

libsciscicos_la_CFLAGS= -I$(top_srcdir)/modules/output_stream/includes \
-I$(top_srcdir)/modules/scicos/includes

libsciscicos_la_includedir=$(pkgincludedir)/scicos
libsciscicos_la_include_HEADERS = src/c/noscicos/resource.h

#### scicos : init scripts ####
libsciscicos_la_etcdir = $(mydatadir)/etc
libsciscicos_la_etc_DATA = etc/scicos.quit etc/scicos.start

endif


#### WORKAROUND ####
# Do not use the global clean-local rule because Scicos help is not managed
# like the rest of Scilab
#clean-local:
#	@for dir in $(MACRODIRS) $(MACROSDIRSEXT) ; do \
#		echo "rm -f $(srcdir)/$$dir/$(MACROBINMASK)"; \
#        rm -f $(srcdir)/$$dir/$(MACROBINMASK); \
#    done;
