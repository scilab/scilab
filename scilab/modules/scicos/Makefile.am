##########
### Sylvestre Ledru <sylvestre.ledru@inria.fr>
### INRIA - Scilab 2006/2008
##########

# SUBDIRS = src/ocaml/

#### Target ######
modulename=scicos

if SCICOS

pkglib_LTLIBRARIES =  libscisundials.la libsciscicos.la

SCICOS_C_SOURCES = src/c/tree.c \
src/c/sciblk2i.c \
src/c/scicos_malloc.c \
src/c/import.c \
src/c/scicosexit.c \
src/c/slider.c \
src/c/scicos.c \
src/c/scicos_free.c \
src/c/sciblk2.c \
src/c/scicosclip.c \
src/c/sciblk4.c \
src/c/haltscicos.c

SCICOS_FORTRAN_SOURCES = src/fortran/scitovv.f \
src/fortran/vvtosci.f \
src/fortran/skipvars.f \
src/fortran/coselm.f \
src/fortran/scitod.f \
src/fortran/dtosci.f \
src/fortran/sctree.f \
src/fortran/scierr.f \
src/fortran/ftree2.f \
src/fortran/ftree3.f \
src/fortran/list2vars.f \
src/fortran/ftree4.f \
src/fortran/scitoi.f \
src/fortran/scifunc.f \
src/fortran/itosci.f

GATEWAY_C_SOURCES = sci_gateway/c/intcscicos.c \
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

GATEWAY_FORTRAN_SOURCES = sci_gateway/fortran/sci_ctree.f \
sci_gateway/fortran/sci_curblock.f \
sci_gateway/fortran/sci_debug_count.f \
sci_gateway/fortran/sci_getblocklabel.f \
sci_gateway/fortran/sci_scicos_debug.f \
sci_gateway/fortran/sci_tree2.f \
sci_gateway/fortran/sci_tree3.f \
sci_gateway/fortran/sci_tree4.f \
sci_gateway/fortran/sci_var2vec.f \
sci_gateway/fortran/sci_vec2var.f

#CLEANFILES=includes/blocks.h build/Cblocknames build/Fblocknames

libsciscicos_la_CFLAGS= -Iincludes/ \
			-Isrc/scicos_sundials/ \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/libs/doublylinkedlist/includes \
			-I$(top_srcdir)/modules/graphics/includes \
			-I$(top_srcdir)/modules/dynamic_link/includes \
			-I$(top_srcdir)/modules/string/includes \
			-I$(top_srcdir)/modules/intersci/src/lib \
			-I$(top_srcdir)/modules/scicos_blocks/includes

#libsciscicos_la_LDFLAGS = -lf2c

libsciscicos_la_SOURCES = $(SCICOS_C_SOURCES) $(SCICOS_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libsciscicos_la_LIBADD = \
	$(top_builddir)/modules/scicos/libscisundials.la \
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


#### scicos : init scripts ####
libsciscicos_la_etcdir = $(mydatadir)/etc
libsciscicos_la_etc_DATA = etc/scicos.quit etc/scicos.start


#### scicos : gateway declaration ####
libsciscicos_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciscicos_la_sci_gateway_DATA = sci_gateway/scicos_gateway.xml


#### scicos : include files ####
libsciscicos_la_includedir=$(pkgincludedir)/scicos
libsciscicos_la_include_HEADERS = includes/intcscicos.h \
includes/scicos.h \
includes/gw_scicos.h


#### scicos : cosf files ####
libsciscicos_la_cosfdir=$(mydatadir)
nobase_libsciscicos_la_cosf_DATA = macros/scicos_scicos/Branching.cosf \
macros/scicos_scicos/DemoBlocks.cosf \
macros/scicos_scicos/Electrical.cosf \
macros/scicos_scicos/Events.cosf \
macros/scicos_scicos/Integer.cosf \
macros/scicos_scicos/Linear.cosf \
macros/scicos_scicos/Matrix.cosf \
macros/scicos_scicos/Non_linear.cosf \
macros/scicos_scicos/OldBlocks.cosf \
macros/scicos_scicos/Others.cosf \
macros/scicos_scicos/Sinks.cosf \
macros/scicos_scicos/Sources.cosf \
macros/scicos_scicos/ThermoHydraulics.cosf \
macros/scicos_scicos/Threshold.cosf


#### scicos : demos files ####
libsciscicos_la_demosdir=$(mydatadir)
nobase_libsciscicos_la_demos_DATA = demos/bounce.cosf \
demos/Bouncing_ball.cosf \
demos/CodeGen \
demos/Cont.Disc-Observer.cosf \
demos/Controller.cosf \
demos/demo_Datatype.cosf \
demos/demo_flipflop.cosf \
demos/demo_watertank.cosf \
demos/dialogs_learn.sci \
demos/dialogs_play.sci \
demos/dialogs.sci \
demos/Discrete-KalmanFilter.cosf \
demos/Electrical \
demos/fibo.cosf \
demos/Finite_state_Buck.cosf \
demos/Finite_state_SwingUp.cosf \
demos/goto_demo.cosf \
demos/Hydraulics.cosf \
demos/Ifsub.cosf \
demos/imppart_Hydraulics.mo \
demos/Inverted_pendulum.cosf \
demos/Kalman_1.cosf \
demos/live.dat \
demos/Lorentz.cosf \
demos/MBlock \
demos/Plant_DiscreteController.cosf \
demos/scicos.dem \
demos/scicos_learn.sci \
demos/scicos_play.sci \
demos/Scilab_Block.cosf \
demos/setw.sci \
demos/Signal_Builder.cosf \
demos/Simple_Demo.cosf \
demos/Simple_Thermostat.cosf \
demos/sound.cosf \
demos/System-Observer.cosf \
demos/Table_Lookup.cosf \
demos/Temp.cosf \
demos/Thermique_bloc.cosf \
demos/Threshold_ZeroCrossing.cosf \
demos/tut1 \
demos/tut2 \
demos/tut3 \
demos/tut4 \
demos/tut5 \
demos/tut6


libsciscicos_la_demosCodeGendir=$(mydatadir)/demos/CodeGen/
nobase_libsciscicos_la_demosCodeGen_DATA = demos/CodeGen/Fibo_standalone.c \
demos/CodeGen/Therm.cosf \
demos/CodeGen/Therm_standalone.c \
demos/CodeGen/fibo.cosf \
demos/CodeGen/data \
demos/CodeGen/controller.cosf


libsciscicos_la_demosElectricaldir=$(mydatadir)/demos/Electrical/
nobase_libsciscicos_la_demosElectrical_DATA = demos/Electrical/DC_DC_Buck_Converter.cosf \
demos/Electrical/Bridge_Rectifier.cosf \
demos/Electrical/Colpitts_Oscillator.cosf \
demos/Electrical/Boost_Converter.cosf \
demos/Electrical/Switched_capacitor_integrator.cosf \
demos/Electrical/NOR_Gate.cosf \
demos/Electrical/AND_Gate.cosf \
demos/Electrical/Difference_amplifier.cosf \
demos/Electrical/RLC_circuit.cosf \
demos/Electrical/Transformer.cosf \
demos/Electrical/Opamp_Amplifier.cosf


libsciscicos_la_demosMBlockdir=$(mydatadir)/demos/MBlock/
nobase_libsciscicos_la_demosMBlock_DATA = demos/MBlock/Chaos_Modelica.cosf \
demos/MBlock/BouncingBall_Modelica.cosf \
demos/MBlock/RLC_Modelica.cosf \
demos/MBlock/Ball_Platform.cosf


#### scicos : MACROS ####
MACROSDIRSEXT= macros/scicos_auto \
macros/scicos_menus \
macros/scicos_scicos \
macros/scicos_utils


#### scicos : SUNDIALS STUFF ####
SUNDIALS_SOURCES = src/scicos_sundials/cvode.c \
src/scicos_sundials/sundials_dense.c \
src/scicos_sundials/ida_ic.c \
src/scicos_sundials/ida_dense.c \
src/scicos_sundials/cvode_dense.c \
src/scicos_sundials/nvector_serial.c \
src/scicos_sundials/sundials_nvector.c \
src/scicos_sundials/ida_io.c \
src/scicos_sundials/cvode_io.c \
src/scicos_sundials/sundials_smalldense.c \
src/scicos_sundials/sundials_math.c \
src/scicos_sundials/ida.c


libscisundials_la_SOURCES = $(SUNDIALS_SOURCES)

else

pkglib_LTLIBRARIES = libsciscicos.la 
NO_SCICOS_C_SOURCES = src/c/noscicos/noscicos.c

libsciscicos_la_SOURCES = $(NO_SCICOS_C_SOURCES)

libsciscicos_la_CFLAGS= -I$(top_srcdir)/modules/output_stream/includes \
-I$(top_srcdir)/modules/scicos/includes

libsciscicos_la_includedir=$(pkgincludedir)/scicos
libsciscicos_la_include_HEADERS = src/c/noscicos/resource.h

#### scicos : init scripts ####
libsciscicos_la_etcdir = $(mydatadir)/etc
libsciscicos_la_etc_DATA = etc/scicos.quit etc/scicos.start

endif

include $(top_srcdir)/Makefile.incl.am

#### WORKAROUND ####
# Do not use the global clean-local rule because Scicos help is not managed
# like the rest of Scilab
#clean-local:
#	@for dir in $(MACRODIRS) $(MACROSDIRSEXT) ; do \
#		echo "rm -f $(srcdir)/$$dir/$(MACROBINMASK)"; \
#        rm -f $(srcdir)/$$dir/$(MACROBINMASK); \
#    done;

.NOTPARALLEL: libscisundials.la libsciscicos.la
