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
modulename=scicos_blocks

#### Source listing ######

# All the filename are important here.
# The filename must be the same as the function name
# since they are used by genblocks.sh to generate the blocks.h
SCICOS_BLOCKS_C_SOURCES = \
    src/c/absblk.c \
    src/c/absolute_value.c \
    src/c/acos_blk.c \
    src/c/acosh_blk.c \
    src/c/andlog.c \
    src/c/asin_blk.c \
    src/c/asinh_blk.c \
    src/c/assignment.c \
    src/c/atan_blk.c \
    src/c/atanh_blk.c \
    src/c/automat.c \
    src/c/backlash.c \
    src/c/bidon.c \
    src/c/bit_clear_16.c \
    src/c/bit_clear_32.c \
    src/c/bit_clear_8.c \
    src/c/bit_set_16.c \
    src/c/bit_set_32.c \
    src/c/bit_set_8.c \
    src/c/bounce_ball.c \
    src/c/bouncexy.c \
    src/c/canimxy3d.c \
    src/c/canimxy.c \
    src/c/cdummy.c \
    src/c/cevscpe.c \
    src/c/cfscope.c \
    src/c/cmat3d.c \
    src/c/cmatview.c \
    src/c/cmscope.c \
    src/c/constraint_c.c \
    src/c/convert.c \
    src/c/cos_blk.c \
    src/c/cosblk.c \
    src/c/cosh_blk.c \
    src/c/counter.c \
    src/c/cscope.c \
    src/c/cscopxy3d.c \
    src/c/cscopxy.c \
    src/c/csslti4.c \
    src/c/cstblk4.c \
    src/c/cstblk4_m.c \
    src/c/cumsum_c.c \
    src/c/cumsum_m.c \
    src/c/cumsum_r.c \
    src/c/cumsumz_c.c \
    src/c/cumsumz_m.c \
    src/c/cumsumz_r.c \
    src/c/curve_c.c \
    src/c/dband.c \
    src/c/deadband.c \
    src/c/delay4.c \
    src/c/delay4_i16.c \
    src/c/delay4_i32.c \
    src/c/delay4_i8.c \
    src/c/delay4_ui16.c \
    src/c/delay4_ui32.c \
    src/c/delay4_ui8.c \
    src/c/deriv.c \
    src/c/diffblk_c.c \
    src/c/dmmul1.c \
    src/c/dmmul.c \
    src/c/dollar4.c \
    src/c/dollar4_m.c \
    src/c/dsslti4.c \
    src/c/edgetrig.c \
    src/c/evaluate_expr.c \
    src/c/evtdly4.c \
    src/c/evtvardly.c \
    src/c/expblk_m.c \
    src/c/extdiag.c \
    src/c/extdiagz.c \
    src/c/extract_bit_16_LH.c \
    src/c/extract_bit_16_LSB.c \
    src/c/extract_bit_16_MSB0.c \
    src/c/extract_bit_16_MSB1.c \
    src/c/extract_bit_16_RB0.c \
    src/c/extract_bit_16_RB1.c \
    src/c/extract_bit_16_UH0.c \
    src/c/extract_bit_16_UH1.c \
    src/c/extract_bit_32_LH.c \
    src/c/extract_bit_32_LSB.c \
    src/c/extract_bit_32_MSB0.c \
    src/c/extract_bit_32_MSB1.c \
    src/c/extract_bit_32_RB0.c \
    src/c/extract_bit_32_RB1.c \
    src/c/extract_bit_32_UH0.c \
    src/c/extract_bit_32_UH1.c \
    src/c/extract_bit_8_LH.c \
    src/c/extract_bit_8_LSB.c \
    src/c/extract_bit_8_MSB0.c \
    src/c/extract_bit_8_MSB1.c \
    src/c/extract_bit_8_RB0.c \
    src/c/extract_bit_8_RB1.c \
    src/c/extract_bit_8_UH0.c \
    src/c/extract_bit_8_UH1.c \
    src/c/extract_bit_u16_MSB1.c \
    src/c/extract_bit_u16_RB1.c \
    src/c/extract_bit_u16_UH1.c \
    src/c/extract_bit_u32_MSB1.c \
    src/c/extract_bit_u32_RB1.c \
    src/c/extract_bit_u32_UH1.c \
    src/c/extract_bit_u8_MSB1.c \
    src/c/extract_bit_u8_RB1.c \
    src/c/extract_bit_u8_UH1.c \
    src/c/extract.c \
    src/c/extractor.c \
    src/c/extractz.c \
    src/c/exttril.c \
    src/c/exttrilz.c \
    src/c/exttriu.c \
    src/c/exttriuz.c \
    src/c/foriterator.c \
    src/c/gainblk.c \
    src/c/gainblk_i16e.c \
    src/c/gainblk_i16n.c \
    src/c/gainblk_i16s.c \
    src/c/gainblk_i32e.c \
    src/c/gainblk_i32n.c \
    src/c/gainblk_i32s.c \
    src/c/gainblk_i8e.c \
    src/c/gainblk_i8n.c \
    src/c/gainblk_i8s.c \
    src/c/gainblk_ui16e.c \
    src/c/gainblk_ui16n.c \
    src/c/gainblk_ui16s.c \
    src/c/gainblk_ui32e.c \
    src/c/gainblk_ui32n.c \
    src/c/gainblk_ui32s.c \
    src/c/gainblk_ui8e.c \
    src/c/gainblk_ui8n.c \
    src/c/gainblk_ui8s.c \
    src/c/gain.c \
    src/c/hystheresis.c \
    src/c/integral_func.c \
    src/c/integralz_func.c \
    src/c/invblk4.c \
    src/c/logicalop.c \
    src/c/logicalop_i16.c \
    src/c/logicalop_i32.c \
    src/c/logicalop_i8.c \
    src/c/logicalop_m.c \
    src/c/logicalop_ui16.c \
    src/c/logicalop_ui32.c \
    src/c/logicalop_ui8.c \
    src/c/logic.c \
    src/c/lookup2d.c \
    src/c/lookup_c.c \
    src/c/mat_bksl.c \
    src/c/matbyscal.c \
    src/c/matbyscal_e.c \
    src/c/matbyscal_s.c \
    src/c/mat_cath.c \
    src/c/mat_catv.c \
    src/c/mat_det.c \
    src/c/mat_diag.c \
    src/c/mat_div.c \
    src/c/mat_expm.c \
    src/c/mathermit_m.c \
    src/c/mat_inv.c \
    src/c/mat_lu.c \
    src/c/matmul2_e.c \
    src/c/matmul2_m.c \
    src/c/matmul2_s.c \
    src/c/matmul_i16e.c \
    src/c/matmul_i16n.c \
    src/c/matmul_i16s.c \
    src/c/matmul_i32e.c \
    src/c/matmul_i32n.c \
    src/c/matmul_i32s.c \
    src/c/matmul_i8e.c \
    src/c/matmul_i8n.c \
    src/c/matmul_i8s.c \
    src/c/matmul_m.c \
    src/c/matmul_ui16e.c \
    src/c/matmul_ui16n.c \
    src/c/matmul_ui16s.c \
    src/c/matmul_ui32e.c \
    src/c/matmul_ui32n.c \
    src/c/matmul_ui32s.c \
    src/c/matmul_ui8e.c \
    src/c/matmul_ui8n.c \
    src/c/matmul_ui8s.c \
    src/c/mat_pinv.c \
    src/c/mat_reshape.c \
    src/c/mat_sing.c \
    src/c/mat_sqrt.c \
    src/c/mat_sum.c \
    src/c/mat_sumc.c \
    src/c/mat_suml.c \
    src/c/mat_svd.c \
    src/c/mattran_m.c \
    src/c/mat_vps.c \
    src/c/mat_vpv.c \
    src/c/matz_abs.c \
    src/c/matz_absc.c \
    src/c/matz_bksl.c \
    src/c/matz_cath.c \
    src/c/matz_catv.c \
    src/c/matz_conj.c \
    src/c/matz_det.c \
    src/c/matz_diag.c \
    src/c/matz_div.c \
    src/c/matz_expm.c \
    src/c/matz_inv.c \
    src/c/matz_lu.c \
    src/c/matzmul2_m.c \
    src/c/matzmul_m.c \
    src/c/matz_pinv.c \
    src/c/matz_reim.c \
    src/c/matz_reimc.c \
    src/c/matz_reshape.c \
    src/c/matz_sing.c \
    src/c/matz_sqrt.c \
    src/c/matz_sum.c \
    src/c/matz_sumc.c \
    src/c/matz_suml.c \
    src/c/matz_svd.c \
    src/c/matztran_m.c \
    src/c/matz_vps.c \
    src/c/matz_vpv.c \
    src/c/m_frequ.c \
    src/c/minmax.c \
    src/c/modulo_count.c \
    src/c/mswitch.c \
    src/c/multiplex.c \
    src/c/plusblk.c \
    src/c/prod.c \
    src/c/product.c \
    src/c/ramp.c \
    src/c/ratelimiter.c \
    src/c/readau.c \
    src/c/readc.c \
    src/c/relational_op.c \
    src/c/relationalop.c \
    src/c/relational_op_i16.c \
    src/c/relational_op_i32.c \
    src/c/relational_op_i8.c \
    src/c/relational_op_ui16.c \
    src/c/relational_op_ui32.c \
    src/c/relational_op_ui8.c \
    src/c/relay.c \
    src/c/ricc_m.c \
    src/c/rndblk_m.c \
    src/c/rndblkz_m.c \
    src/c/root_coef.c \
    src/c/rootz_coef.c \
    src/c/samphold4.c \
    src/c/samphold4_m.c \
    src/c/satur.c \
    src/c/scalar2vector.c \
    src/c/scicosexit.c \
    src/c/selector.c \
    src/c/selector_m.c \
    src/c/shift_16_LA.c \
    src/c/shift_16_LC.c \
    src/c/shift_16_RA.c \
    src/c/shift_16_RC.c \
    src/c/shift_32_LA.c \
    src/c/shift_32_LC.c \
    src/c/shift_32_RA.c \
    src/c/shift_32_RC.c \
    src/c/shift_8_LA.c \
    src/c/shift_8_LC.c \
    src/c/shift_8_RA.c \
    src/c/shift_8_RC.c \
    src/c/shift_u16_RA.c \
    src/c/shift_u32_RA.c \
    src/c/shift_u8_RA.c \
    src/c/signum.c \
    src/c/sin_blk.c \
    src/c/sinh_blk.c \
    src/c/step_func.c \
    src/c/submat.c \
    src/c/submatz.c \
    src/c/sum.c \
    src/c/summation.c \
    src/c/summation_i16e.c \
    src/c/summation_i16n.c \
    src/c/summation_i16s.c \
    src/c/summation_i32e.c \
    src/c/summation_i32n.c \
    src/c/summation_i32s.c \
    src/c/summation_i8e.c \
    src/c/summation_i8n.c \
    src/c/summation_i8s.c \
    src/c/summation_ui16e.c \
    src/c/summation_ui16n.c \
    src/c/summation_ui16s.c \
    src/c/summation_ui32e.c \
    src/c/summation_ui32n.c \
    src/c/summation_ui32s.c \
    src/c/summation_ui8e.c \
    src/c/summation_ui8n.c \
    src/c/summation_ui8s.c \
    src/c/summation_z.c \
    src/c/switch2.c \
    src/c/switch2_m.c \
    src/c/switchn.c \
    src/c/tablex2d_c.c \
    src/c/tan_blk.c \
    src/c/tanh_blk.c \
    src/c/tcslti4.c \
    src/c/tcsltj4.c \
    src/c/time_delay.c \
    src/c/tows_c.c \
    src/c/variable_delay.c \
    src/c/whileiterator.c \
    src/c/writeau.c \
    src/c/writec.c \
    src/c/zcross2.c

SCICOS_BLOCKS_CXX_SOURCES = \
    src/cpp/affich2.cpp \
    src/cpp/fromws_c.cpp

NON_BLOCK_C_SOURCES = \
    src/c/scicos_evalhermite.c \
    src/c/scicos_indexfinder.c

NON_BLOCK_CXX_SOURCES = \
    src/cpp/scoUtils.cpp

NON_BLOCK_JNI_SOURCES = \
    src/cpp/HelpersJNI.cpp \
    src/jni/AfficheBlock.cpp

NON_BLOCK_CLI_SOURCES = \
    src/cpp/HelpersCLI.cpp

GIWS_WRAPPERS = \
    src/jni/Blocks.giws.xml

SCICOS_BLOCKS_FORTRAN_SOURCES = \
    src/fortran/csslti.f \
    src/fortran/cstblk.f \
    src/fortran/delay.f \
    src/fortran/delayv.f \
    src/fortran/demux.f \
    src/fortran/diffblk.f \
    src/fortran/dlradp.f \
    src/fortran/dollar.f \
    src/fortran/dsslti.f \
    src/fortran/eselect.f \
    src/fortran/evtdly.f \
    src/fortran/expblk.f \
    src/fortran/forblk.f \
    src/fortran/fsv.f \
    src/fortran/gensin.f \
    src/fortran/gensqr.f \
    src/fortran/hltblk.f \
    src/fortran/ifthel.f \
    src/fortran/integr.f \
    src/fortran/intplt.f \
    src/fortran/intpol.f \
    src/fortran/intrp2.f \
    src/fortran/intrpl.f \
    src/fortran/invblk.f \
    src/fortran/iocopy.f \
    src/fortran/logblk.f \
    src/fortran/lookup.f \
    src/fortran/lsplit.f \
    src/fortran/lusat.f \
    src/fortran/maxblk.f \
    src/fortran/memo.f \
    src/fortran/mfclck.f \
    src/fortran/minblk.f \
    src/fortran/mux.f \
    src/fortran/pload.f \
    src/fortran/powblk.f \
    src/fortran/qzcel.f \
    src/fortran/qzflr.f \
    src/fortran/qzrnd.f \
    src/fortran/qztrn.f \
    src/fortran/readf.f \
    src/fortran/rndblk.f \
    src/fortran/samphold.f \
    src/fortran/sawtth.f \
    src/fortran/sciblk.f \
    src/fortran/selblk.f \
    src/fortran/sinblk.f \
    src/fortran/sqrblk.f \
    src/fortran/sum2.f \
    src/fortran/sum3.f \
    src/fortran/tanblk.f \
    src/fortran/tcslti.f \
    src/fortran/tcsltj.f \
    src/fortran/timblk.f \
    src/fortran/trash.f \
    src/fortran/writef.f \
    src/fortran/zcross.f

#### Multi-libraries handling ######

noinst_LTLIBRARIES = libsciscicos_blocks-algo.la
pkglib_LTLIBRARIES = libsciscicos_blocks-cli.la
if XCOS
if GUI
pkglib_LTLIBRARIES += libsciscicos_blocks.la
endif
endif

libsciscicos_blocks_algo_la_LDFLAGS = $(AM_LDFLAGS)
libsciscicos_blocks_la_LDFLAGS = $(AM_LDFLAGS)
libsciscicos_blocks_cli_la_LDFLAGS = $(AM_LDFLAGS)

libsciscicos_blocks_algo_la_CPPFLAGS= \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/c/ \
    -I$(srcdir)/src/cpp/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    -I$(top_srcdir)/modules/elementary_functions/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/graphics/includes/ \
    -I$(top_srcdir)/modules/graphic_objects/includes/ \
    -I$(top_srcdir)/modules/hdf5/includes \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/renderer/includes/ \
    -I$(top_srcdir)/modules/scicos/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    $(HDF5_CFLAGS) \
    $(AM_CPPFLAGS)

libsciscicos_blocks_la_CPPFLAGS= \
    -I$(srcdir)/src/jni/ \
    -I$(top_srcdir)/modules/commons/src/jni/ \
    -I$(top_srcdir)/modules/jvm/includes/ \
    $(JAVA_JNI_INCLUDE) \
    $(libsciscicos_blocks_algo_la_CPPFLAGS)

libsciscicos_blocks_cli_la_CPPFLAGS= $(libsciscicos_blocks_algo_la_CPPFLAGS)

libsciscicos_blocks_algo_la_SOURCES = \
    $(SCICOS_BLOCKS_C_SOURCES) \
    $(SCICOS_BLOCKS_CXX_SOURCES) \
    $(SCICOS_BLOCKS_FORTRAN_SOURCES) \
    $(NON_BLOCK_C_SOURCES) \
    $(NON_BLOCK_CXX_SOURCES)

libsciscicos_blocks_cli_la_SOURCES =
libsciscicos_blocks_la_SOURCES =

libsciscicos_blocks_algo_la_LIBADD =
libsciscicos_blocks_la_LIBADD =
libsciscicos_blocks_cli_la_LIBADD =

if XCOS
libsciscicos_blocks_cli_la_SOURCES += $(NON_BLOCK_CLI_SOURCES)
libsciscicos_blocks_cli_la_LIBADD += libsciscicos_blocks-algo.la
if GUI
libsciscicos_blocks_la_SOURCES += $(NON_BLOCK_JNI_SOURCES)
libsciscicos_blocks_la_LIBADD += libsciscicos_blocks-algo.la
endif
endif

libsciscicos_blocks_algo_la_LIBADD += $(RT_LIB)
libsciscicos_blocks_la_LIBADD += $(RT_LIB)
libsciscicos_blocks_cli_la_LIBADD += $(RT_LIB)

# For the code check (splint)
CHECK_SRC= $(SCICOS_BLOCKS_C_SOURCES) $(NON_BLOCK_C_SOURCES)
INCLUDE_FLAGS = $(libsciscicos_blocks_la_CPPFLAGS)


#### scicos_blocks : Generation of the  includes/blocks.h Files ####
BUILT_SOURCES =

if XCOS
BUILT_SOURCES += includes/blocks.h
endif

if GIWS
BUILT_SOURCES += giws
endif

$(top_builddir)/modules/scicos_blocks/build/Cblocknames:Makefile.am $(SCICOS_BLOCKS_C_SOURCES)
	@echo "-- Generates build/Cblocknames --"
	mkdir -p $(top_builddir)/modules/scicos_blocks/build;
# Get the filename, strip the extension and put into a file
	@for file in $(SCICOS_BLOCKS_C_SOURCES); do echo $$file |sed  's|.*/\([0-9A-Za-z_-]*\)\.c|\1|' ;done > $(top_builddir)/modules/scicos_blocks/build/Cblocknames

$(top_builddir)/modules/scicos_blocks/build/CPPblocknames:Makefile.am $(SCICOS_BLOCKS_CXX_SOURCES)
	@echo "-- Generates build/CPPblocknames --"
	mkdir -p $(top_builddir)/modules/scicos_blocks/build;
# Get the filename, strip the extension and put into a file
	@for file in $(SCICOS_BLOCKS_CXX_SOURCES); do echo $$file |sed  's|.*/\([0-9A-Za-z_-]*\)\.cpp|\1|' ;done > $(top_builddir)/modules/scicos_blocks/build/CPPblocknames

$(top_builddir)/modules/scicos_blocks/build/Fblocknames: Makefile.am $(SCICOS_BLOCKS_FORTRAN_SOURCES)
	@echo "-- Generates build/Fblocknames --"
	mkdir -p $(top_builddir)/modules/scicos_blocks/build;
# Get the filename, strip the extension and put into a file
	@for file in $(SCICOS_BLOCKS_FORTRAN_SOURCES); do echo $$file |sed  's|.*/\([0-9A-Za-z_-]*\)\.f|\1|';done > $(top_builddir)/modules/scicos_blocks/build/Fblocknames

includes/blocks.h: $(top_builddir)/modules/scicos_blocks/build/Cblocknames $(top_builddir)/modules/scicos_blocks/build/Fblocknames $(top_builddir)/modules/scicos_blocks/build/CPPblocknames
	@echo "-- Building includes/blocks.h --"
	mkdir -p $(top_builddir)/modules/scicos_blocks/includes;
	@$(top_srcdir)/modules/scicos_blocks/src/scripts/GenBlocks.sh $(top_builddir)/modules/scicos_blocks/build/Fblocknames $(top_builddir)/modules/scicos_blocks/build/Cblocknames $(top_builddir)/modules/scicos_blocks/build/CPPblocknames $(top_builddir)/modules/scicos_blocks/includes/blocks.h

CLEANFILES= \
    $(top_srcdir)/modules/scicos_blocks/includes/blocks.h \
	$(top_srcdir)/modules/scicos_blocks/build/Cblocknames \
	$(top_srcdir)/modules/scicos_blocks/build/Fblocknames \
	$(top_srcdir)/modules/scicos_blocks/build/CPPblocknames \
	$(srcdir)/macros/Electrical/*.moc \
    $(srcdir)/macros/*/models \
	$(srcdir)/macros/Hydraulics/*.moc \
	$(BASE_PATH)/linenum.ml

#### scicos_blocks : Conf files ####
libsciscicos_blocks_la_rootdir = $(mydatadir)
libsciscicos_blocks_la_root_DATA =  license.txt

#### scicos_blocks : init scripts ####
libsciscicos_blocks_la_etcdir = $(mydatadir)/etc
libsciscicos_blocks_la_etc_DATA = etc/scicos_blocks.quit etc/scicos_blocks.start

#### scicos_blocks : include files ####
libsciscicos_blocks_la_includedir=$(pkgincludedir)
libsciscicos_blocks_la_include_HEADERS = includes/scicos_block.h \
includes/scicos_block4.h \
includes/blocks.h


#### scicos_blocks : MACROS ####
MACROSDIRSEXT= macros/Branching \
macros/Electrical \
macros/Events \
macros/Hydraulics \
macros/IntegerOp \
macros/Linear \
macros/MatrixOp \
macros/Misc \
macros/NonLinear \
macros/PDE \
macros/Sinks \
macros/Sources \
macros/Threshold

MACROSSPECIALEXT=*.mo \
*.moc

TESTS_DIREXT= tests/unit_tests/Linear

include $(top_srcdir)/Makefile.incl.am

