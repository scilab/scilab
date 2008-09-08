# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2008 - INRIA - Sylvestre Ledru <sylvestre.ledru@inria.fr>
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

# SUBDIRS = src/ocaml/

#### Target ######
modulename=scicos
DISABLE_HELP_CLEAN=yes

libsciscicos_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(RT_LIB)


include $(top_srcdir)/Makefile.incl.am


if SCICOS
# Code not factorized with the one on the bottom since sundials must be
# compiled first
pkglib_LTLIBRARIES = libscisundials.la libsciscicos.la

SCICOS_C_SOURCES = src/c/tree.c \
src/c/sciblk2i.c \
src/c/scicos_malloc.c \
src/c/import.c \
src/c/slider.c \
src/c/scicos.c \
src/c/scicos_free.c \
src/c/sciblk2.c \
src/c/scicosclip.c \
src/c/sciblk4.c


SCICOS_FORTRAN_SOURCES = src/fortran/scitovv.f \
src/fortran/vvtosci.f \
src/fortran/skipvars.f \
src/fortran/coselm.f \
src/fortran/scitod.f \
src/fortran/sctree.f \
src/fortran/scierr.f \
src/fortran/ftree2.f \
src/fortran/ftree3.f \
src/fortran/list2vars.f \
src/fortran/ftree4.f \
src/fortran/scitoi.f \
src/fortran/scifunc.f

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
sci_gateway/c/sci_haltscicos.c


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
			-I$(top_srcdir)/modules/scicos_blocks/includes \
			-I$(top_srcdir)/modules/action_binding/includes

libsciscicos_la_SOURCES = $(SCICOS_C_SOURCES) $(SCICOS_FORTRAN_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)

libsciscicos_la_LIBADD =  $(top_builddir)/modules/string/libscistring.la $(top_builddir)/modules/mexlib/libmat.la $(top_builddir)/modules/mexlib/libmx.la $(top_builddir)/modules/intersci/libsciintersci.la $(top_builddir)/modules/graphics/libscigraphics.la $(top_builddir)/modules/elementary_functions/libscielementary_functions.la $(top_builddir)/modules/time/libscitime.la $(top_builddir)/modules/dynamic_link/libscidynamic_link.la $(top_builddir)/modules/scicos/libscisundials.la $(top_builddir)/modules/scicos_blocks/libsciscicos_blocks.la $(top_builddir)/modules/mexlib/libmex.la $(top_builddir)/modules/core/libscicore.la $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/output_stream/libscioutput_stream.la $(top_builddir)/modules/arnoldi/libsciarnoldi.la 

if USE_EMBEDDED_BLAS
libsciscicos_la_LIBADD +=  $(top_builddir)/libs/blas/libsciblas.la
endif

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
includes/gw_scicos.h \
includes/scicos-def.h \
includes/scicos_free.h \
includes/scicos_malloc.h

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
nobase_libsciscicos_la_demos_DATA = demos/anim_pen.sci \
demos/basic_controller.dem.sce \
demos/bounce.cosf \
demos/bounce.dem.sce \
demos/Bouncing_ball.cosf \
demos/bouncing_ball.dem.sce \
demos/Cont.Disc-Observer.cosf \
demos/cont_sys_disc_cont.dem.sce \
demos/control_systems.dem.gateway.sce \
demos/Controller.cosf \
demos/data_type.dem.sce \
demos/demo_Datatype.cosf \
demos/demo_Datatype.dem.sce \
demos/demo_flipflop.cosf \
demos/demo_flipflop.dem.sce \
demos/demo_watertank.cosf \
demos/demo_watertank.dem.sce \
demos/dialogs.sci \
demos/dialogs_learn.sci \
demos/dialogs_play.sci \
demos/Discrete-KalmanFilter.cosf \
demos/discrete_controller.dem.sce \
demos/discrete_kalman.dem.sce \
demos/electrical_systems.dem.gateway.sce \
demos/fibo.cosf \
demos/fibo.dem.sce \
demos/Fibonacci.cosf \
demos/fibonacci.dem.sce \
demos/ModelicaBlocks/Finite_state_Buck.cosf \
demos/ModelicaBlocks/Finite_state_SwingUp.cosf \
demos/flip_flop_logic.dem.sce \
demos/go_pend_5.sci \
demos/goto_demo.cosf \
demos/goto_demo.dem.sce \
demos/goto_from.dem.sce \
demos/ModelicaBlocks/Hydraulics.cosf \
demos/IF_block.cosf \
demos/if_scicos_block.dem.sce \
demos/Ifsub.cosf \
demos/imppart_Hydraulics.mo \
demos/inv_pend_anim.dem.sce \
demos/inverted_pend.dem.sce \
demos/Inverted_pendulum.cosf \
demos/inverted_pendulum.dem.sce \
demos/Kalman.cosf \
demos/kalman.dem.sce \
demos/Kalman_1.cosf \
demos/live.dat \
demos/live.dem.gateway.sce \
demos/Lorentz.cosf \
demos/lorentz.dem.sce \
demos/mechanical_systems.dem.gateway.sce \
demos/modelica_demos.dem.gateway.sce \
demos/old_demos.dem.gateway.sce \
demos/old_gain_block.dem.sce \
demos/OldGainTest.cosf \
demos/PENDULUM_ANIM.sci \
demos/pendulum_anim5.cosf \
demos/Plant_DiscreteController.cosf \
demos/scicos.dem \
demos/scicos.dem.gateway.sce \
demos/scicos_learn.sci \
demos/scicos_play.sci \
demos/Scilab_Block.cosf \
demos/scilab_block.dem.sce \
demos/setw.sci \
demos/Signal_Builder.cosf \
demos/signal_builder.dem.sce \
demos/Simple_Demo.cosf \
demos/simple_demo.dem.sce \
demos/Simple_Thermostat.cosf \
demos/simple_thermostat.dem.sce \
demos/sound.cosf \
demos/standard.dem.gateway.sce \
demos/susp.cosf \
demos/susp.dem.sce \
demos/System-Observer.cosf \
demos/Table_Lookup.cosf \
demos/table_lookup.dem.sce \
demos/Temperature_Controller.cosf \
demos/temp_controller.dem.sce \
demos/Thermique_bloc.cosf \
demos/Threshold_ZeroCrossing.cosf \
demos/tut1 \
demos/tut1.dem.sce \
demos/tut2 \
demos/tut2.dem.sce \
demos/tut3 \
demos/tut3.dem.sce \
demos/tut4 \
demos/tut4.dem.sce \
demos/tut5 \
demos/tut5.dem.sce \
demos/tut6 \
demos/tut6.dem.sce \
demos/zero_crossing.dem.sce

libsciscicos_la_demosCodeGendir=$(mydatadir)
nobase_libsciscicos_la_demosCodeGen_DATA = demos/CodeGen/controller.cosf \
demos/CodeGen/data \
demos/CodeGen/fibo.cosf \
demos/CodeGen/Fibo_standalone.c \
demos/CodeGen/Therm.cosf \
demos/CodeGen/Therm_standalone.c

libsciscicos_la_demosElectricaldir=$(mydatadir)
nobase_libsciscicos_la_demosElectrical_DATA = demos/Electrical/AND_Gate.cosf \
demos/Electrical/boost_conv.dem.sce \
demos/Electrical/Boost_Converter.cosf \
demos/Electrical/Bridge_Rectifier.cosf \
demos/Electrical/bridge_rectifier.dem.sce \
demos/Electrical/buck_conv.dem.sce \
demos/Electrical/colpitts_osc.dem.sce \
demos/Electrical/Colpitts_Oscillator.cosf \
demos/Electrical/DC_DC_Buck_Converter.cosf \
demos/Electrical/Difference_amplifier.cosf \
demos/Electrical/differential_amp.dem.sce \
demos/Electrical/logic_and.dem.sce \
demos/Electrical/logic_nor.dem.sce \
demos/Electrical/NOR_Gate.cosf \
demos/Electrical/op_amplier.dem.sce \
demos/Electrical/Opamp_Amplifier.cosf \
demos/Electrical/RLC_circuit.cosf \
demos/Electrical/rlc_circuit.dem.sce \
demos/Electrical/switched_cap_filter.dem.sce \
demos/Electrical/Switched_capacitor_integrator.cosf \
demos/Electrical/Transformer.cosf \
demos/Electrical/transformer.dem.sce


libsciscicos_la_demosModelicaBlocksdir=$(mydatadir)
nobase_libsciscicos_la_demosModelicaBlocks_DATA = demos/ModelicaBlocks/Ball_Platform.cosf \
demos/ModelicaBlocks/BouncingBall_Modelica.cosf \
demos/ModelicaBlocks/Chaos_Modelica.cosf \
demos/ModelicaBlocks/modelica_hydraulics.dem.sce \
demos/ModelicaBlocks/ball_platform.dem.sce \
demos/ModelicaBlocks/Hydraulics.cosf \
demos/ModelicaBlocks/modelica_fsm_buck.dem.sce \
demos/ModelicaBlocks/modelica_fsm_boost.dem.sce \
demos/ModelicaBlocks/modelica_chaos.dem.sce \
demos/ModelicaBlocks/modelica_bouncing_ball.dem.sce \
demos/ModelicaBlocks/Finite_state_Buck.cosf \
demos/ModelicaBlocks/Finite_state_SwingUp.cosf \
demos/ModelicaBlocks/modelica_rlc_circuit.dem.sce \
demos/ModelicaBlocks/Chaos_Modelica.cosf \
demos/ModelicaBlocks/RLC_Modelica.cosf


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


#### scicos : help files scripts ####

libsciscicos_la_help_en_USdir = $(mydatadir)/help/en_US
libsciscicos_la_help_en_US_DATA = help/en_US/scicos_help.jar

libsciscicos_la_help_fr_FRdir = $(mydatadir)/help/fr_FR
libsciscicos_la_help_fr_FR_DATA = help/fr_FR/scicos_help.jar

#########�Modelica #############

if OCAML
TARGET = modelicac

bin_PROGRAMS = $(TARGET)
modelicac_SOURCES = $(MLS)

OCAMLLIBS=nums.cma

MLS = src/modelica_compiler/parseTree.ml src/modelica_compiler/linenum.ml src/modelica_compiler/parser.ml src/modelica_compiler/lexer.ml\
	src/modelica_compiler/precompilation.ml src/modelica_compiler/compilation.ml src/modelica_compiler/instantiation.ml\
	src/modelica_compiler/graphNodeSet.ml src/modelica_compiler/symbolicExpression.ml\
	src/modelica_compiler/squareSparseMatrix.ml src/modelica_compiler/bipartiteGraph.ml src/modelica_compiler/hungarianMethod.ml\
	src/modelica_compiler/causalityGraph.ml\
	src/modelica_compiler/optimization.ml  src/modelica_compiler/xMLCodeGeneration.ml src/modelica_compiler/optimizingCompiler.ml\
	src/modelica_compiler/scicosCodeGeneration.ml src/modelica_compiler/scicosOptimizingCompiler.ml

MLIS = src/modelica_compiler/parseTree.mli \
	src/modelica_compiler/precompilation.mli src/modelica_compiler/compilation.mli src/modelica_compiler/instantiation.mli\
	src/modelica_compiler/graphNodeSet.mli src/modelica_compiler/symbolicExpression.mli \
	src/modelica_compiler/squareSparseMatrix.mli src/modelica_compiler/bipartiteGraph.mli src/modelica_compiler/hungarianMethod.mli \
	src/modelica_compiler/causalityGraph.mli \
	src/modelica_compiler/optimization.mli \
	src/modelica_compiler/xMLCodeGeneration.mli \
	src/modelica_compiler/optimizingCompiler.mli \
	src/modelica_compiler/scicosCodeGeneration.mli


PARSER_SRC = src/modelica_compiler/parser.mly
LEXER_SRC = src/modelica_compiler/lexer.mll
BASE_PATH = src/modelica_compiler
INCLUDE = -I $(BASE_PATH)

SUFFIXES += .ml .mli .mll .mly .cmo .cmi .cmx

# @TODO check how to avoid the rebuild of the parser/lexer

.ml.cmo:
	$(OCAMLC) $(INCLUDE)  -c $<

.ml.cmx:
	$(OCAMLOPT) $(INCLUDE) -c $<

.mli.cmi:
	$(OCAMLC) $(INCLUDE) -c $<

.mll.ml:
	$(OCAMLLEX) $<

.mly.ml:
	$(OCAMLYACC) $<

# call the ocaml/Yacc
generate-parser: $(PARSER_SRC)
	$(OCAMLYACC) $(<)
	$(RM) -f $(<:.mly=.mli)

# Call the lexer
generate-lexer: $(LEXER_SRC)
	$(OCAMLLEX) $(<)

# Build the binary
$(TARGET): generate-parser generate-lexer $(MLIS:.mli=.cmi) $(MLS:.ml=.cmo) $(MLS:.ml=.cmx)
	$(OCAMLOPT) -o $(TARGET) $(INCLUDE) $(OCAMLLIBS:.cma=.cmxa) $(MLS:.ml=.cmx)


CLEANFILES = $(BASE_PATH)/*.cmi $(BASE_PATH)/*.o $(BASE_PATH)/*.cmo $(BASE_PATH)/*.cm[aiox] $(TARGET) $(BASE_PATH)/parser.mli $(BASE_PATH)/lexer.mli $(BASE_PATH)/parser.ml $(BASE_PATH)/lexer.ml
endif # OCAML

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

.NOTPARALLEL: libscisundials.la libsciscicos.la
