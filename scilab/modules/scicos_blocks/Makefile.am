# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2008 - INRIA - Sylvestre Ledru <sylvestre.ledru@inria.fr>
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

pkglib_LTLIBRARIES = libsciscicos_blocks.la

libsciscicos_blocks_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(RT_LIB) 

#### Target ######
modulename=scicos_blocks

include $(top_srcdir)/Makefile.incl.am

if SCICOS

# All the filename are important here.
# The filename must be the same as the function name
# since they are used by genblocks.sh to generate the blocks.h
SCICOS_BLOCKS_C_SOURCES = src/c/absolute_value.c \
src/c/absblk.c \
src/c/andlog.c \
src/c/gain.c \
src/c/dband.c \
src/c/cosblk.c \
src/c/acos_blk.c \
src/c/acosh_blk.c \
src/c/asin_blk.c \
src/c/asinh_blk.c \
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
src/c/convert.c \
src/c/cos_blk.c \
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
src/c/deadband.c \
src/c/delay4.c \
src/c/delay4_i16.c \
src/c/delay4_i32.c \
src/c/delay4_i8.c \
src/c/delay4_ui16.c \
src/c/delay4_ui32.c \
src/c/delay4_ui8.c \
src/c/deriv.c \
src/c/dmmul1.c \
src/c/dmmul.c \
src/c/dollar4.c \
src/c/dollar4_m.c \
src/c/dsslti4.c \
src/c/edgetrig.c \
src/c/evaluate_expr.c \
src/c/evtdly4.c \
src/c/evtvardly.c \
src/c/scicosexit.c \
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
src/c/fromws_c.c \
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
src/c/mat_bksl.c \
src/c/mat_cath.c \
src/c/mat_catv.c \
src/c/mat_det.c \
src/c/mat_diag.c \
src/c/mat_div.c \
src/c/mat_expm.c \
src/c/mathermit_m.c \
src/c/mat_inv.c \
src/c/mat_lu.c \
src/c/matmul2_m.c \
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
src/c/tan_blk.c \
src/c/tanh_blk.c \
src/c/tcslti4.c \
src/c/tcsltj4.c \
src/c/time_delay.c \
src/c/tows_c.c \
src/c/variable_delay.c \
src/c/writeau.c \
src/c/writec.c \
src/c/zcross2.c

SCICOS_BLOCK_CPP_SOURCES = src/jni/XcosDiagram.cpp

NON_BLOCK_C_SOURCES =  src/c/scoGetProperty.c \
src/c/scoSetProperty.c \
src/c/scoMisc.c \
src/c/scoMemoryScope.c\
src/c/scoWindowScope.c \
src/c/affich.c

SCICOS_BLOCKS_FORTRAN_SOURCES = src/fortran/affich2.f \
src/fortran/affich.f \
src/fortran/constraint.f \
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



#### scicos_blocks : GATEWAY_C_SOURCES ####

#### scicos_blocks : GATEWAY_FORTRAN_SOURCES ####




libsciscicos_blocks_la_CFLAGS= -I$(srcdir)/includes/ \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/libs/doublylinkedlist/includes/ \
			-I$(top_srcdir)/modules/graphics/includes/ \
			-I$(top_srcdir)/modules/renderer/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/scicos/includes \
            -I$(top_srcdir)/modules/elementary_functions/includes

libsciscicos_blocks_la_CPPFLAGS = $(JAVA_JNI_INCLUDE) \
			-I$(srcdir)/includes/ \
			-I$(srcdir)/src/jni/ \
			-I$(srcdir)/src/cpp/ \
			-I$(srcdir)/src/c/ \
	 		-I$(top_srcdir)/modules/jvm/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/localization/includes \
			-I$(top_srcdir)/modules/api_scilab/includes

libsciscicos_blocks_la_SOURCES = $(SCICOS_BLOCKS_C_SOURCES) \
$(SCICOS_BLOCKS_FORTRAN_SOURCES) \
$(NON_BLOCK_C_SOURCES) \
$(SCICOS_BLOCK_CPP_SOURCES)


# For the code check (splint)
CHECK_SRC= $(SCICOS_BLOCKS_C_SOURCES) $(NON_BLOCK_C_SOURCES)
INCLUDE_FLAGS = $(libsciscicos_blocks_la_CFLAGS)


#$(GATEWAY_C_SOURCES) $(GATEWAY_FORTRAN_SOURCES)


#### scicos_blocks : Generation of the  includes/blocks.h Files ####
BUILT_SOURCES = includes/blocks.h

build/Cblocknames:Makefile.am
	@echo "-- Generates build/Cblocknames --"
	@if test ! -d build; then mkdir build; fi
# Get the filename, strip the extension and put into a file
	@for file in $(SCICOS_BLOCKS_C_SOURCES); do echo $$file |sed  's|.*/\([0-9A-Za-z_-]*\)\.c|\1|' ;done > build/Cblocknames

build/Fblocknames: Makefile.am
	@echo "-- Generates build/Fblocknames --"
	@if test ! -d build; then mkdir build; fi
# Get the filename, strip the extension and put into a file
	@for file in $(SCICOS_BLOCKS_FORTRAN_SOURCES); do echo $$file |sed  's|.*/\([0-9A-Za-z_-]*\)\.f|\1|';done > build/Fblocknames

includes/blocks.h: build/Cblocknames build/Fblocknames
	@echo "-- Building includes/blocks.h --"
	@$(top_srcdir)/modules/scicos_blocks/src/scripts/GenBlocks.sh $(top_srcdir)/modules/scicos_blocks/build/Fblocknames $(top_srcdir)/modules/scicos_blocks/build/Cblocknames $(top_srcdir)/modules/scicos_blocks/includes/blocks.h

CLEANFILES=$(top_srcdir)/modules/scicos_blocks/includes/blocks.h \
	$(top_srcdir)/modules/scicos_blocks/build/Cblocknames \
	$(top_srcdir)/modules/scicos_blocks/build/Fblocknames \
	$(srcdir)/macros/Electrical/*.moc \
    $(srcdir)/macros/*/models \
	$(srcdir)/macros/Hydraulics/*.moc \
	$(BASE_PATH)/linenum.ml




# cyclic deps $(top_builddir)/modules/scicos/libsciscicos.la
libsciscicos_blocks_la_LIBADD =  $(top_builddir)/modules/core/libscicore.la $(top_builddir)/modules/cacsd/libscicacsd.la  $(top_builddir)/modules/elementary_functions/libscielementary_functions.la $(top_builddir)/modules/renderer/libscirenderer.la $(top_builddir)/modules/polynomials/libscipolynomials.la $(top_builddir)/modules/fileio/libscifileio.la  $(top_builddir)/modules/linear_algebra/libscilinear_algebra.la $(top_builddir)/modules/arnoldi/libsciarnoldi.la $(top_builddir)/modules/string/libscistring.la $(top_builddir)/libs/MALLOC/libscimalloc.la $(top_builddir)/modules/output_stream/libscioutput_stream.la


#### scicos_blocks : Conf files ####
libsciscicos_blocks_la_rootdir = $(mydatadir)
libsciscicos_blocks_la_root_DATA = changelog.txt license.txt readme.txt version.xml

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


#### xcos : hdf5 blocks files ####
libsciscicos_blocks_la_hdf5blocksdir=$(mydatadir)/
nobase_libsciscicos_blocks_la_hdf5blocks_DATA = blocks/ABSBLK_f.h5 \
blocks/EVTVARDLY.h5 \
blocks/NEGTOPOS_f.h5 \
blocks/ABS_VALUE.h5 \
blocks/EXPBLK_m.h5 \
blocks/NMOS.h5 \
blocks/AFFICH_m.h5 \
blocks/EXPRESSION.h5 \
blocks/NPN.h5 \
blocks/ANDBLK.h5 \
blocks/Extract_Activation.h5 \
blocks/NRMSOM_f.h5 \
blocks/ANDLOG_f.h5 \
blocks/EXTRACTBITS.h5 \
blocks/OpAmp.h5 \
blocks/AUTOMAT.h5 \
blocks/EXTRACT.h5 \
blocks/OUT_f.h5 \
blocks/Bache.h5 \
blocks/EXTRACTOR.h5 \
blocks/OUTIMPL_f.h5 \
blocks/BACKLASH.h5 \
blocks/EXTTRI.h5 \
blocks/PDE.h5 \
blocks/BIGSOM_f.h5 \
blocks/fortran_block.h5 \
blocks/PerteDP.h5 \
blocks/BITCLEAR.h5 \
blocks/freq_div.h5 \
blocks/PID.h5 \
blocks/BITSET.h5 \
blocks/FROM.h5 \
blocks/PMOS.h5 \
blocks/BOUNCE.h5 \
blocks/FROMMO.h5 \
blocks/PNP.h5 \
blocks/BOUNCEXY.h5 \
blocks/FROMWSB.h5 \
blocks/POSTONEG_f.h5 \
blocks/BPLATFORM.h5 \
blocks/GAINBLK_f.h5 \
blocks/PotentialSensor.h5 \
blocks/CANIMXY3D.h5 \
blocks/GAINBLK.h5 \
blocks/POWBLK_f.h5 \
blocks/CANIMXY.h5 \
blocks/GENERAL_f.h5 \
blocks/PROD_f.h5 \
blocks/Capacitor.h5 \
blocks/generic_block3.h5 \
blocks/PRODUCT.h5 \
blocks/c_block.h5 \
blocks/generic_block.h5 \
blocks/PuitsP.h5 \
blocks/CBLOCK.h5 \
blocks/GENSIN_f.h5 \
blocks/QUANT_f.h5 \
blocks/CCS.h5 \
blocks/GENSQR_f.h5 \
blocks/RAMP.h5 \
blocks/CEVENTSCOPE.h5 \
blocks/GOTO.h5 \
blocks/RAND_f.h5 \
blocks/CFSCOPE.h5 \
blocks/GOTOMO.h5 \
blocks/RAND_m.h5 \
blocks/CLINDUMMY_f.h5 \
blocks/GotoTagVisibility.h5 \
blocks/RATELIMITER.h5 \
blocks/CLKFROM.h5 \
blocks/GotoTagVisibilityMO.h5 \
blocks/READAU_f.h5 \
blocks/CLKGOTO.h5 \
blocks/Ground.h5 \
blocks/READC_f.h5 \
blocks/CLKGotoTagVisibility.h5 \
blocks/Gyrator.h5 \
blocks/REGISTER.h5 \
blocks/CLKINV_f.h5 \
blocks/HALT_f.h5 \
blocks/RELATIONALOP.h5 \
blocks/CLKOUT_f.h5 \
blocks/HYSTHERESIS.h5 \
blocks/RELAY_f.h5 \
blocks/CLKOUTV_f.h5 \
blocks/IdealTransformer.h5 \
blocks/Resistor.h5 \
blocks/CLKSOM_f.h5 \
blocks/IFTHEL_f.h5 \
blocks/RFILE_f.h5 \
blocks/CLKSOMV_f.h5 \
blocks/Inductor.h5 \
blocks/RICC.h5 \
blocks/CLKSPLIT_f.h5 \
blocks/IN_f.h5 \
blocks/ROOTCOEF.h5 \
blocks/CLOCK_c.h5 \
blocks/INIMPL_f.h5 \
blocks/SAMPHOLD_m.h5 \
blocks/CLOCK_f.h5 \
blocks/INTEGRAL_f.h5 \
blocks/SampleCLK.h5 \
blocks/CLR.h5 \
blocks/INTEGRAL_m.h5 \
blocks/SAT_f.h5 \
blocks/CLSS.h5 \
blocks/INTMUL.h5 \
blocks/SATURATION.h5 \
blocks/CMAT3D.h5 \
blocks/INTRP2BLK_f.h5 \
blocks/SAWTOOTH_f.h5 \
blocks/CMATVIEW.h5 \
blocks/INTRPLBLK_f.h5 \
blocks/SCALAR2VECTOR.h5 \
blocks/CMSCOPE.h5 \
blocks/INVBLK.h5 \
blocks/scifunc_block_m.h5 \
blocks/ConstantVoltage.h5 \
blocks/ISELECT_m.h5 \
blocks/SELECT_m.h5 \
blocks/CONST_m.h5 \
blocks/JKFLIPFLOP.h5 \
blocks/SHIFT.h5 \
blocks/CONSTRAINT_f.h5 \
blocks/LOGBLK_f.h5 \
blocks/Sigbuilder.h5 \
blocks/CONVERT.h5 \
blocks/LOGICAL_OP.h5 \
blocks/SIGNUM.h5 \
blocks/COSBLK_f.h5 \
blocks/LOGIC.h5 \
blocks/SINBLK_f.h5 \
blocks/Counter.h5 \
blocks/LOOKUP_f.h5 \
blocks/SineVoltage.h5 \
blocks/CSCOPE.h5 \
blocks/MATBKSL.h5 \
blocks/SourceP.h5 \
blocks/CSCOPXY3D.h5 \
blocks/MATCATH.h5 \
blocks/SPLIT_f.h5 \
blocks/CSCOPXY.h5 \
blocks/MATCATV.h5 \
blocks/SQRT.h5 \
blocks/CUMSUM.h5 \
blocks/MATDET.h5 \
blocks/SRFLIPFLOP.h5 \
blocks/CurrentSensor.h5 \
blocks/MATDIAG.h5 \
blocks/STEP_FUNCTION.h5 \
blocks/CURV_f.h5 \
blocks/MATDIV.h5 \
blocks/SUBMAT.h5 \
blocks/CVS.h5 \
blocks/MATEIG.h5 \
blocks/SUM_f.h5 \
blocks/DEADBAND.h5 \
blocks/MATEXPM.h5 \
blocks/SUMMATION.h5 \
blocks/DEBUG_SCICOS.h5 \
blocks/MATINV.h5 \
blocks/SUPER_f.h5 \
blocks/DELAY_f.h5 \
blocks/MATLU.h5 \
blocks/SWITCH2_m.h5 \
blocks/DELAYV_f.h5 \
blocks/MATMAGPHI.h5 \
blocks/SWITCH_f.h5 \
blocks/DEMUX_f.h5 \
blocks/MATMUL.h5 \
blocks/Switch.h5 \
blocks/DEMUX.h5 \
blocks/MATPINV.h5 \
blocks/TANBLK_f.h5 \
blocks/DERIV.h5 \
blocks/MATRESH.h5 \
blocks/TCLSS.h5 \
blocks/DFLIPFLOP.h5 \
blocks/MATSING.h5 \
blocks/TIME_DELAY.h5 \
blocks/DIFF_f.h5 \
blocks/MATSUM.h5 \
blocks/TIME_f.h5 \
blocks/Diode.h5 \
blocks/MATTRAN.h5 \
blocks/TKSCALE.h5 \
blocks/DLATCH.h5 \
blocks/MATZCONJ.h5 \
blocks/TOWS_c.h5 \
blocks/DLRADAPT_f.h5 \
blocks/MATZREIM.h5 \
blocks/TRASH_f.h5 \
blocks/DLR.h5 \
blocks/MAX_f.h5 \
blocks/TrigFun.h5 \
blocks/DLSS.h5 \
blocks/MAXMIN.h5 \
blocks/VanneReglante.h5 \
blocks/DOLLAR_f.h5 \
blocks/MBLOCK.h5 \
blocks/VARIABLE_DELAY.h5 \
blocks/DOLLAR_m.h5 \
blocks/MCLOCK_f.h5 \
blocks/VariableResistor.h5 \
blocks/EDGE_TRIGGER.h5 \
blocks/MEMORY_f.h5 \
blocks/VoltageSensor.h5 \
blocks/EDGETRIGGER.h5 \
blocks/MFCLCK_f.h5 \
blocks/VsourceAC.h5 \
blocks/ENDBLK.h5 \
blocks/M_freq.h5 \
blocks/VVsourceAC.h5 \
blocks/END_c.h5 \
blocks/MIN_f.h5 \
blocks/WFILE_f.h5 \
blocks/ESELECT_f.h5 \
blocks/Modulo_Count.h5 \
blocks/WRITEAU_f.h5 \
blocks/EVTDLY_c.h5 \
blocks/M_SWITCH.h5 \
blocks/WRITEC_f.h5 \
blocks/EVTDLY_f.h5 \
blocks/MUX_f.h5 \
blocks/ZCROSS_f.h5 \
blocks/EVTGEN_f.h5 \
blocks/MUX.h5


else

libsciscicos_blocks_la_SOURCES =

endif

.NOTPARALLEL: build/Fblocknames build/Cblocknames
