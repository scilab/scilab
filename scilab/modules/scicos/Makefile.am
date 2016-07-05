# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2008 - INRIA - Sylvestre Ledru <sylvestre.ledru@inria.fr>
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.

#### Target ######
modulename=scicos
DISABLE_HELP_CLEAN=yes

#### Source listing ######

SCICOS_CXX_SOURCES = \
src/cpp/il_state.cpp \
src/cpp/il_sim.cpp \
src/cpp/var2vec.cpp \
src/cpp/vec2var.cpp \
src/cpp/XMIResource.cpp \
src/cpp/XMIResource_load.cpp \
src/cpp/XMIResource_save.cpp \
src/cpp/Controller.cpp \
src/cpp/controller_helpers.cpp \
src/cpp/LoggerView.cpp \
src/cpp/Model.cpp \
src/cpp/Model_getObjectProperties.cpp \
src/cpp/Model_setObjectProperties.cpp \
src/cpp/view_scilab/Adapters.cpp \
src/cpp/view_scilab/BlockAdapter.cpp \
src/cpp/view_scilab/CprAdapter.cpp \
src/cpp/view_scilab/DiagramAdapter.cpp \
src/cpp/view_scilab/GraphicsAdapter.cpp \
src/cpp/view_scilab/LinkAdapter.cpp \
src/cpp/view_scilab/ModelAdapter.cpp \
src/cpp/view_scilab/ParamsAdapter.cpp \
src/cpp/view_scilab/ScsAdapter.cpp \
src/cpp/view_scilab/StateAdapter.cpp \
src/cpp/view_scilab/TextAdapter.cpp \
src/cpp/sciblk2.cpp \
src/cpp/sciblk4.cpp \
src/cpp/createblklist.cpp \
src/cpp/extractblklist.cpp


SCICOS_C_SOURCES = \
src/c/tree.c \
src/c/scicos_malloc.c \
src/c/scicos_print.c \
src/c/import.c \
src/c/scicos.c \
src/c/scicos_free.c \
src/c/ezxml.c \
src/c/var2sci.c \
src/c/MlistGetFieldNumber.c \
src/c/lsodar.c \
src/c/ddaskr.c

SCICOS_FORTRAN_SOURCES = \
src/fortran/sctree.f \
src/fortran/ftree2.f \
src/fortran/ftree3.f \
src/fortran/ftree4.f \
src/fortran/scifunc.f

NO_SCICOS_C_SOURCES = src/c/noscicos/noscicos.c

GATEWAY_CXX_SOURCES = \
sci_gateway/cpp/sci_scicos_debug.cpp \
sci_gateway/cpp/sci_scicos_debug_count.cpp \
sci_gateway/cpp/sci_scicos_log.cpp \
sci_gateway/cpp/sci_buildouttb.cpp \
sci_gateway/cpp/sci_ctree2.cpp \
sci_gateway/cpp/sci_ctree3.cpp \
sci_gateway/cpp/sci_ctree4.cpp \
sci_gateway/cpp/sci_coserror.cpp \
sci_gateway/cpp/sci_curblock.cpp \
sci_gateway/cpp/sci_data2sig.cpp \
sci_gateway/cpp/sci_diffobjs.cpp \
sci_gateway/cpp/sci_duplicate.cpp \
sci_gateway/cpp/sci_end_scicosim.cpp \
sci_gateway/cpp/sci_getblocklabel.cpp \
sci_gateway/cpp/sci_getscicosvars.cpp \
sci_gateway/cpp/sci_phase_simulation.cpp \
sci_gateway/cpp/sci_pointer_xproperty.cpp \
sci_gateway/cpp/sci_scicos_new.cpp \
sci_gateway/cpp/sci_scicos_setfield.cpp \
sci_gateway/cpp/sci_scicos_time.cpp \
sci_gateway/cpp/sci_scicosim.cpp \
sci_gateway/cpp/sci_set_blockerror.cpp \
sci_gateway/cpp/sci_set_xproperty.cpp \
sci_gateway/cpp/sci_sig2data.cpp \
sci_gateway/cpp/sci_validvar.cpp \
sci_gateway/cpp/sci_var2vec.cpp \
sci_gateway/cpp/sci_vec2var.cpp \
sci_gateway/cpp/sci_callblk.cpp \
sci_gateway/cpp/sci_model2blk.cpp \
sci_gateway/cpp/sci_scicosDiagramToScilab.cpp

GATEWAY_C_SOURCES = \
sci_gateway/c/sci_ftree2.c \
sci_gateway/c/sci_ftree3.c \
sci_gateway/c/sci_ftree4.c \
sci_gateway/c/sci_loadScicos.c \
sci_gateway/c/sci_sctree.c

GATEWAY_FORTRAN_SOURCES = \
sci_gateway/fortran/sci_f_tree2.f \
sci_gateway/fortran/sci_f_tree3.f \
sci_gateway/fortran/sci_f_tree4.f

#### Multi-libraries handling ######

noinst_LTLIBRARIES = libsciscicos-algo.la
if XCOS
pkglib_LTLIBRARIES = libscisundials.la \
                     libsciscicos-cli.la
if GUI
pkglib_LTLIBRARIES += libsciscicos.la
endif
endif

libsciscicos_algo_la_LDFLAGS = $(AM_LDFLAGS)
libsciscicos_cli_la_LDFLAGS = $(AM_LDFLAGS)
libsciscicos_la_LDFLAGS = $(AM_LDFLAGS)

if XCOS
libsciscicos_algo_la_CPPFLAGS= -I$(srcdir)/includes/ \
			-I$(srcdir)/src/c/ \
			-I$(srcdir)/src/cpp/ \
			-I$(srcdir)/src/scicos_sundials/include/ \
			-I$(srcdir)/src/scicos_sundials/include/sundials/ \
			-I$(srcdir)/src/scicos_sundials/src/ida/ \
			-I$(top_srcdir)/modules/output_stream/includes/ \
			-I$(top_srcdir)/modules/dynamic_link/includes/ \
			-I$(top_srcdir)/modules/string/includes/ \
			-I$(top_srcdir)/modules/localization/includes/ \
			-I$(top_srcdir)/modules/scicos_blocks/includes/ \
			-I$(top_srcdir)/modules/action_binding/includes/ \
			-I$(top_srcdir)/modules/time/includes/ \
			-I$(top_srcdir)/modules/api_scilab/includes/ \
			-I$(top_srcdir)/modules/ast/includes/ast/ \
			-I$(top_srcdir)/modules/ast/includes/exps/ \
			-I$(top_srcdir)/modules/ast/includes/symbol/ \
			-I$(top_srcdir)/modules/ast/includes/operations/ \
			-I$(top_srcdir)/modules/ast/includes/system_env/ \
			-I$(top_srcdir)/modules/ast/includes/types/ \
			-I$(top_srcdir)/modules/ast/includes/analysis/ \
			-I$(top_srcdir)/modules/threads/includes/ \
			-I$(top_srcdir)/modules/console/includes/ \
			-I$(top_srcdir)/modules/differential_equations/includes/ \
			$(XML_FLAGS) \
			$(AM_CPPFLAGS)
else
libsciscicos_algo_la_CPPFLAGS= -I$(top_srcdir)/modules/output_stream/includes/ \
			-I$(top_srcdir)/modules/api_scilab/includes/ \
			-I$(top_srcdir)/modules/scicos/includes/ \
			$(XML_FLAGS) \
			$(AM_CPPFLAGS)
endif
libsciscicos_cli_la_CPPFLAGS = $(libsciscicos_algo_la_CPPFLAGS)
libsciscicos_la_CPPFLAGS = $(libsciscicos_algo_la_CPPFLAGS)

if XCOS
libsciscicos_algo_la_SOURCES = $(SCICOS_CXX_SOURCES) $(SCICOS_C_SOURCES) $(SCICOS_FORTRAN_SOURCES)
libsciscicos_cli_la_SOURCES = $(GATEWAY_CXX_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)
libsciscicos_la_SOURCES = $(GATEWAY_CXX_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)
else
libsciscicos_algo_la_SOURCES = $(NO_SCICOS_C_SOURCES)
libsciscicos_cli_la_SOURCES =
libsciscicos_la_SOURCES =
endif

libsciscicos_algo_la_LIBADD =  \
		$(RT_LIB) \
		$(LIBXML_LIBS)
libsciscicos_cli_la_LIBADD = libsciscicos-algo.la
libsciscicos_la_LIBADD = libsciscicos-algo.la
if XCOS
libsciscicos_cli_la_LIBADD += libscisundials.la \
    $(top_builddir)/modules/scicos_blocks/libsciscicos_blocks-cli.la
if GUI
libsciscicos_la_LIBADD += libscisundials.la \
    $(top_builddir)/modules/scicos_blocks/libsciscicos_blocks.la
endif
endif

# For the code check (splint)
CHECK_SRC= $(SCICOS_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libsciscicos_la_CFLAGS)

#### scicos : Conf files ####
libsciscicos_la_rootdir = $(mydatadir)
libsciscicos_la_root_DATA =  license.txt


#### scicos : init scripts ####
libsciscicos_la_etcdir = $(mydatadir)/etc
libsciscicos_la_etc_DATA = etc/scicos.quit etc/scicos.start

#### scicos : tcl scripts ####
libsciscicos_la_tcldir = $(mydatadir)/tcl
libsciscicos_la_tcl_DATA = tcl/MIHM.tcl \
tcl/scicos.gif \
tcl/scicos2.gif

#### scicos : gateway declaration ####
libsciscicos_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libsciscicos_la_sci_gateway_DATA = sci_gateway/scicos_gateway.xml


#### scicos : include files ####
libsciscicos_la_includedir=$(pkgincludedir)/
libsciscicos_la_include_HEADERS = includes/scicos.h \
includes/scicos-def.h \
includes/scicos_free.h \
includes/scicos_malloc.h \
includes/scicos_math.h \
includes/scicos_print.h \
includes/dynlib_scicos.h \
includes/import.h



#### scicos : cosf files ####
libsciscicos_la_cosfdir=$(mydatadir)
nobase_libsciscicos_la_cosf_DATA = palettes/Branching.cosf \
palettes/DemoBlocks.cosf \
palettes/Events.cosf \
palettes/Integer.cosf \
palettes/Linear.cosf \
palettes/Matrix.cosf \
palettes/Non_linear.cosf \
palettes/OldBlocks.cosf \
palettes/Others.cosf \
palettes/Sinks.cosf \
palettes/Sources.cosf \
palettes/Threshold.cosf

#### scicos : MACROS ####
MACROSDIRSEXT= macros/scicos_auto \
macros/scicos_scicos \
macros/scicos_utils


#### scicos : SUNDIALS STUFF ####
SUNDIALS_SOURCES = src/scicos_sundials/src/cvode/cvode.c \
src/scicos_sundials/src/cvode/cvode_dense.c \
src/scicos_sundials/src/cvode/cvode_direct.c \
src/scicos_sundials/src/cvode/cvode_io.c \
src/scicos_sundials/src/ida/ida.c \
src/scicos_sundials/src/ida/ida_dense.c \
src/scicos_sundials/src/ida/ida_direct.c \
src/scicos_sundials/src/ida/ida_ic.c \
src/scicos_sundials/src/ida/ida_io.c \
src/scicos_sundials/src/kinsol/kinsol.c \
src/scicos_sundials/src/kinsol/kinsol_dense.c \
src/scicos_sundials/src/kinsol/kinsol_direct.c \
src/scicos_sundials/src/kinsol/kinsol_io.c \
src/scicos_sundials/src/nvec_ser/nvector_serial.c \
src/scicos_sundials/src/sundials/sundials_dense.c \
src/scicos_sundials/src/sundials/sundials_direct.c \
src/scicos_sundials/src/sundials/sundials_math.c \
src/scicos_sundials/src/sundials/sundials_nvector.c \
src/scicos_sundials/src/sundials/sundials_extension.c

libscisundials_la_CFLAGS = -I$(top_srcdir)/modules/scicos/src/scicos_sundials/include \
-I$(top_srcdir)/modules/scicos/src/scicos_sundials/include/cvode \
-I$(top_srcdir)/modules/scicos/src/scicos_sundials/include/ida \
-I$(top_srcdir)/modules/scicos/src/scicos_sundials/include/kinsol \
-I$(top_srcdir)/modules/scicos/src/scicos_sundials/include/nvec_ser \
-I$(top_srcdir)/modules/scicos/src/scicos_sundials/include/sundials \
-I$(top_srcdir)/modules/scicos/src/scicos_sundials/src/cvode \
-I$(top_srcdir)/modules/scicos/src/scicos_sundials/src/ida \
-I$(top_srcdir)/modules/scicos/src/scicos_sundials/src/kinsol \
-I$(top_srcdir)/modules/scicos/src/scicos_sundials/src/nvec_ser \
-I$(top_srcdir)/modules/scicos/src/scicos_sundials/src/sundials


libscisundials_la_SOURCES = $(SUNDIALS_SOURCES)

include $(top_srcdir)/Makefile.incl.am

# Build modelica stuff
include $(srcdir)/Makefile.modelica.am

