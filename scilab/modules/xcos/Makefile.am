#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) DIGITEO - 2009 - Allan CORNET
# 
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at    
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#

#### Target ######
modulename=xcos


#### xcos : Conf files ####
libscixcos_la_rootdir = $(mydatadir)
libscixcos_la_root_DATA = changelog.txt license.txt readme.txt version.xml


#### xcos : init scripts ####
libscixcos_la_etcdir = $(mydatadir)/etc
libscixcos_la_etc_DATA = etc/xcos.quit \
etc/xcos.start

XCOS_C_SOURCES = 

XCOS_CPP_SOURCES = 	src/cpp/callxcos.cpp \
			src/jni/Xcos.cpp \
			src/jni/Xcos.hxx \
			src/jni/Signal.cpp \
			src/jni/Signal.hxx

GIWS_WRAPPERS = src/jni/Xcos.giws.xml \
src/jni/XcosUtils.giws.xml

if GIWS
BUILT_SOURCES=giws
endif


GATEWAY_C_SOURCES = sci_gateway/c/gw_xcos.c

GATEWAY_CPP_SOURCES = sci_gateway/cpp/sci_Xcos.cpp \
sci_gateway/cpp/sci_xcosNotify.cpp 

libscixcos_la_CFLAGS= $(JAVA_JNI_INCLUDE) \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/modules/jvm/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/localization/includes \
			-I$(top_srcdir)/modules/api_scilab/includes	

libscixcos_la_CPPFLAGS = $(JAVA_JNI_INCLUDE) \
			-I$(srcdir)/includes/ \
			-I$(srcdir)/src/jni/ \
			-I$(srcdir)/src/cpp/ \
			-I$(srcdir)/src/c/ \
	 		-I$(top_srcdir)/modules/jvm/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/localization/includes \
                        -I$(top_srcdir)/modules/scicos_blocks/src/jni \
			-I$(top_srcdir)/modules/api_scilab/includes

# Without the xcos module 
libscixcos_disable_la_CFLAGS = -Iincludes/ \
			-I$(top_srcdir)/modules/output_stream/includes

XCOS_DISABLE_C_SOURCES = src/noxcos/noxcos.c
libscixcos_disable_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)
libscixcos_disable_la_SOURCES = $(XCOS_DISABLE_C_SOURCES)

pkglib_LTLIBRARIES = libscixcos-disable.la

if GUI
pkglib_LTLIBRARIES += libscixcos.la 
endif

libscixcos_la_LDFLAGS = -version-info $(SCILAB_LIBRARY_VERSION) $(LD_FLAGS)  $(X_LIBS) $(X_EXTRA_LIBS)

libscixcos_la_SOURCES = $(XCOS_C_SOURCES) $(GATEWAY_C_SOURCES) $(GATEWAY_CPP_SOURCES) $(XCOS_CPP_SOURCES)

# For the code check (splint)
CHECK_SRC= $(XCOS_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscixcos_la_CFLAGS)



libscixcos_la_LIBADD = $(top_builddir)/modules/core/libscicore.la \
$(top_builddir)/libs/MALLOC/libscimalloc.la \
$(top_builddir)/modules/output_stream/libscioutput_stream.la


#### xcos : gateway declaration ####
libscixcos_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscixcos_la_sci_gateway_DATA = sci_gateway/xcos_gateway.xml


#### xcos : include files ####
libscixcos_la_includedir=$(pkgincludedir)/xcos
libscixcos_la_include_HEADERS = includes/gw_xcos.h \
includes/dynlib_xcos.h \
includes/callxcos.h

#### xcos : icons files ####
libscixcos_la_iconsdir=$(mydatadir)/
nobase_libscixcos_la_icons_DATA = images/icons/document-new.png \
images/icons/edit-copy.png \
images/icons/edit-redo.png \
images/icons/media-playback-start.png \
images/icons/document-open.png \
images/icons/edit-cut.png \
images/icons/edit-undo.png \
images/icons/process-stop.png \
images/icons/document-print.png \
images/icons/edit-delete.png \
images/icons/list-add.png \
images/icons/view-fullscreen.png \
images/icons/document-save.png \
images/icons/edit-paste.png \
images/icons/list-remove.png \
images/blocks/ABSBLK_f.gif \
images/blocks/EXPRESSION.gif \
images/blocks/MFCLCK_f.gif \
images/blocks/ABS_VALUE.gif \
images/blocks/Extract_Activation.gif \
images/blocks/M_freq.gif \
images/blocks/ANDBLK.gif \
images/blocks/EXTRACT.gif \
images/blocks/MIN_f.gif \
images/blocks/ANDLOG_f.gif \
images/blocks/EXTRACTOR.gif \
images/blocks/MUX_f.gif \
images/blocks/AUTOMAT.gif \
images/blocks/EXTTRI.gif \
images/blocks/NEGTOPOS_f.gif \
images/blocks/Bache.gif \
images/blocks/fortran_block.gif \
images/blocks/OUT_f.gif \
images/blocks/BACKLASH.gif \
images/blocks/freq_div.gif \
images/blocks/PerteDP.gif \
images/blocks/BOUNCE.gif \
images/blocks/FROM.gif \
images/blocks/PID.gif \
images/blocks/BOUNCEXY.gif \
images/blocks/FROMMO.gif \
images/blocks/POSTONEG_f.gif \
images/blocks/BPLATFORM.gif \
images/blocks/FROMWSB.gif \
images/blocks/PotentialSensor.gif \
images/blocks/CANIMXY3D.gif \
images/blocks/FROMWS_c.gif \
images/blocks/POWBLK_f.gif \
images/blocks/CANIMXY.gif \
images/blocks/GAINBLK_f.gif \
images/blocks/PROD_f.gif \
images/blocks/c_block.gif \
images/blocks/GAINBLK.gif \
images/blocks/QUANT_f.gif \
images/blocks/CBLOCK.gif \
images/blocks/GENERAL_f.gif \
images/blocks/RAMP.gif \
images/blocks/CEVENTSCOPE.gif \
images/blocks/generic_block3.gif \
images/blocks/RAND_f.gif \
images/blocks/CFSCOPE.gif \
images/blocks/generic_block.gif \
images/blocks/RAND_m.gif \
images/blocks/CLINDUMMY_f.gif \
images/blocks/GENSIN_f.gif \
images/blocks/RATELIMITER.gif \
images/blocks/CLKFROM.gif \
images/blocks/GENSQR_f.gif \
images/blocks/READAU_f.gif \
images/blocks/CLKGOTO.gif \
images/blocks/GOTO.gif \
images/blocks/READC_f.gif \
images/blocks/CLKOUT_f.gif \
images/blocks/GOTOMO.gif \
images/blocks/RELAY_f.gif \
images/blocks/CLKOUTV_f.gif \
images/blocks/Ground.gif \
images/blocks/RFILE_f.gif \
images/blocks/CLKSPLIT_f.gif \
images/blocks/HALT_f.gif \
images/blocks/RICC.gif \
images/blocks/CLOCK_c.gif \
images/blocks/HYSTHERESIS.gif \
images/blocks/ROOTCOEF.gif \
images/blocks/CLOCK_f.gif \
images/blocks/IFTHEL_f.gif \
images/blocks/SAMPHOLD_m.gif \
images/blocks/CLR.gif \
images/blocks/IN_f.gif \
images/blocks/SampleCLK.gif \
images/blocks/CLSS.gif \
images/blocks/INTEGRAL_f.gif \
images/blocks/SAT_f.gif \
images/blocks/CMAT3D.gif \
images/blocks/INTEGRAL_m.gif \
images/blocks/SATURATION.gif \
images/blocks/CMATVIEW.gif \
images/blocks/INTMUL.gif \
images/blocks/SAWTOOTH_f.gif \
images/blocks/CMSCOPE.gif \
images/blocks/INTRP2BLK_f.gif \
images/blocks/SCALAR2VECTOR.gif \
images/blocks/CONSTRAINT_f.gif \
images/blocks/INTRPLBLK_f.gif \
images/blocks/scifunc_block_m.gif \
images/blocks/COSBLK_f.gif \
images/blocks/INVBLK.gif \
images/blocks/SELECT_m.gif \
images/blocks/CSCOPE.gif \
images/blocks/ISELECT_m.gif \
images/blocks/SIGNUM.gif \
images/blocks/CSCOPXY3D.gif \
images/blocks/LOGBLK_f.gif \
images/blocks/SINBLK_f.gif \
images/blocks/CSCOPXY.gif \
images/blocks/LOGIC.gif \
images/blocks/SPLIT_f.gif \
images/blocks/CUMSUM.gif \
images/blocks/MATBKSL.gif \
images/blocks/SQRT.gif \
images/blocks/CurrentSensor.gif \
images/blocks/MATCATH.gif \
images/blocks/STEP_FUNCTION.gif \
images/blocks/DEADBAND.gif \
images/blocks/MATCATV.gif \
images/blocks/SUBMAT.gif \
images/blocks/DEBUG_SCICOS.gif \
images/blocks/MATDET.gif \
images/blocks/SUPER_f.gif \
images/blocks/DELAY_f.gif \
images/blocks/MATDIAG.gif \
images/blocks/SWITCH2_m.gif \
images/blocks/DELAYV_f.gif \
images/blocks/MATDIV.gif \
images/blocks/Switch.gif \
images/blocks/DEMUX_f.gif \
images/blocks/MATEIG.gif \
images/blocks/TANBLK_f.gif \
images/blocks/DERIV.gif \
images/blocks/MATEXPM.gif \
images/blocks/TCLSS.gif \
images/blocks/DIFF_f.gif \
images/blocks/MATINV.gif \
images/blocks/TIME_DELAY.gif \
images/blocks/DLRADAPT_f.gif \
images/blocks/MATLU.gif \
images/blocks/TIME_f.gif \
images/blocks/DLR.gif \
images/blocks/MATMAGPHI.gif \
images/blocks/TKSCALE.gif \
images/blocks/DLSS.gif \
images/blocks/MATMUL.gif \
images/blocks/TOWS_c.gif \
images/blocks/DOLLAR_f.gif \
images/blocks/MATPINV.gif \
images/blocks/TRASH_f.gif \
images/blocks/DOLLAR_m.gif \
images/blocks/MATRESH.gif \
images/blocks/VanneReglante.gif \
images/blocks/EDGE_TRIGGER.gif \
images/blocks/MATSING.gif \
images/blocks/VARIABLE_DELAY.gif \
images/blocks/EDGETRIGGER.gif \
images/blocks/MATSUM.gif \
images/blocks/VariableResistor.gif \
images/blocks/ENDBLK.gif \
images/blocks/MATTRAN.gif \
images/blocks/VoltageSensor.gif \
images/blocks/END_c.gif \
images/blocks/MATZCONJ.gif \
images/blocks/WFILE_f.gif \
images/blocks/ESELECT_f.gif \
images/blocks/MATZREIM.gif \
images/blocks/WRITEAU_f.gif \
images/blocks/EVTDLY_c.gif \
images/blocks/MAX_f.gif \
images/blocks/WRITEC_f.gif \
images/blocks/EVTDLY_f.gif \
images/blocks/MBLOCK.gif \
images/blocks/ZCROSS_f.gif \
images/blocks/EVTVARDLY.gif \
images/blocks/MCLOCK_f.gif \
images/blocks/EXPBLK_m.gif \
images/blocks/MEMORY_f.gif



if GUI
if HDF5
USEANT=1
endif
endif

include $(top_srcdir)/Makefile.incl.am

