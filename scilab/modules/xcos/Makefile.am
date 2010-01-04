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
libscixcos_la_etc_DATA =	etc/xcos.quit \
				etc/xcos.start \
				etc/Xcos-style.xml \
				etc/xcosConfiguration.xml

XCOS_C_SOURCES = 

XCOS_CPP_SOURCES = 	src/cpp/callxcos.cpp \
			src/jni/Xcos.cpp \
			src/jni/Xcos.hxx \
			src/jni/Signal.cpp \
			src/jni/Signal.hxx \
			src/jni/GiwsException.cpp

GIWS_WRAPPERS = src/jni/Xcos.giws.xml \
src/jni/XcosUtils.giws.xml

if GIWS
BUILT_SOURCES=giws
endif


GATEWAY_C_SOURCES = sci_gateway/c/gw_xcos.c

GATEWAY_CPP_SOURCES = \
	sci_gateway/cpp/sci_Xcos.cpp \
	sci_gateway/cpp/sci_xcosNotify.cpp \
	sci_gateway/cpp/sci_warnBlockByUID.cpp \
	sci_gateway/cpp/sci_closeXcosFromScilab.cpp \
	sci_gateway/cpp/sci_xcosDiagramToHDF5.cpp \
	sci_gateway/cpp/sci_xcosDiagramOpen.cpp \
	sci_gateway/cpp/sci_xcosDiagramClose.cpp

libscixcos_la_CFLAGS= $(JAVA_JNI_INCLUDE) \
			-I$(top_srcdir)/libs/MALLOC/includes/ \
			-I$(top_srcdir)/modules/jvm/includes/ \
			-I$(top_srcdir)/modules/output_stream/includes \
			-I$(top_srcdir)/modules/fileio/includes \
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
			-I$(top_srcdir)/modules/fileio/includes \
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


#### xcos : images files ####
libscixcos_la_imagesdir=$(mydatadir)/
nobase_libscixcos_la_images_DATA = images/blocks/Bache.gif \
images/blocks/BIGSOM_f.gif \
images/blocks/BOUNCEXY.gif \
images/blocks/BPLATFORM.gif \
images/blocks/CANIMXY.gif \
images/blocks/CANIMXY3D.gif \
images/blocks/CCS.gif \
images/blocks/CEVENTSCOPE.gif \
images/blocks/CFSCOPE.gif \
images/blocks/CLKSOMV_f.gif \
images/blocks/CLOCK_c.gif \
images/blocks/CLOCK_f.gif \
images/blocks/CSCOPE.gif \
images/blocks/CSCOPXY.gif \
images/blocks/CSCOPXY3D.gif \
images/blocks/CVS.gif \
images/blocks/Capacitor.gif \
images/blocks/ConstantVoltage.gif \
images/blocks/CurrentSensor.gif \
images/blocks/DEADBAND.gif \
images/blocks/Diode.gif \
images/blocks/Ground.gif \
images/blocks/Gyrator.gif \
images/blocks/HYSTHERESIS.gif \
images/blocks/INTEGRAL_m.gif \
images/blocks/IdealTransformer.gif \
images/blocks/Inductor.gif \
images/blocks/LOOKUP_f.gif \
images/blocks/NMOS.gif \
images/blocks/NPN.gif \
images/blocks/PMOS.gif \
images/blocks/PNP.gif \
images/blocks/PRODUCT.gif \
images/blocks/PROD_f.gif \
images/blocks/PerteDP.gif \
images/blocks/PotentialSensor.gif \
images/blocks/PuitsP.gif \
images/blocks/QUANT_f.gif \
images/blocks/RAMP.gif \
images/blocks/Resistor.gif \
images/blocks/SATURATION.gif \
images/blocks/SAT_f.gif \
images/blocks/STEP_FUNCTION.gif \
images/blocks/SUMMATION.gif \
images/blocks/SUM_f.gif \
images/blocks/SUPER_f.gif \
images/blocks/SampleCLK.gif \
images/blocks/SourceP.gif \
images/blocks/Switch.gif \
images/blocks/TIME_f.gif \
images/blocks/VanneReglante.gif \
images/blocks/VariableResistor.gif \
images/blocks/VoltageSensor.gif \
images/icons/document-new.png \
images/icons/document-open.png \
images/icons/document-print.png \
images/icons/document-save-as.png \
images/icons/edit-copy.png \
images/icons/edit-cut.png \
images/icons/edit-delete.png \
images/icons/edit-paste.png \
images/icons/edit-redo.png \
images/icons/edit-undo.png \
images/icons/list-add.png \
images/icons/list-remove.png \
images/icons/media-floppy.png \
images/icons/media-playback-start.png \
images/icons/process-stop.png \
images/icons/view-fullscreen.png \
images/icons/applications-system.png \
images/icons/help-browser.png \
images/palettes/ABS_VALUE.jpg \
images/palettes/AFFICH_m.jpg \
images/palettes/ANDBLK.jpg \
images/palettes/ANDLOG_f.jpg \
images/palettes/AUTOMAT.jpg \
images/palettes/Bache.jpg \
images/palettes/BACKLASH.jpg \
images/palettes/BIGSOM_f.jpg \
images/palettes/BITCLEAR.jpg \
images/palettes/BITSET.jpg \
images/palettes/BOUNCE.jpg \
images/palettes/BOUNCEXY.jpg \
images/palettes/BPLATFORM.jpg \
images/palettes/c_block.jpg \
images/palettes/CANIMXY.jpg \
images/palettes/CANIMXY3D.jpg \
images/palettes/Capacitor.jpg \
images/palettes/CBLOCK.jpg \
images/palettes/CCS.jpg \
images/palettes/CEVENTSCOPE.jpg \
images/palettes/CFSCOPE.jpg \
images/palettes/CLINDUMMY_f.jpg \
images/palettes/CLKFROM.jpg \
images/palettes/CLKGOTO.jpg \
images/palettes/CLKGotoTagVisibility.jpg \
images/palettes/CLKINV_f.jpg \
images/palettes/CLKOUTV_f.jpg \
images/palettes/CLKSOMV_f.jpg \
images/palettes/CLOCK_c.jpg \
images/palettes/CLR.jpg \
images/palettes/CLSS.jpg \
images/palettes/CMAT3D.jpg \
images/palettes/CMATVIEW.jpg \
images/palettes/CMSCOPE.jpg \
images/palettes/CONST_m.jpg \
images/palettes/ConstantVoltage.jpg \
images/palettes/CONSTRAINT_f.jpg \
images/palettes/CONVERT.jpg \
images/palettes/COSBLK_f.jpg \
images/palettes/Counter.jpg \
images/palettes/CSCOPE.jpg \
images/palettes/CSCOPXY.jpg \
images/palettes/CSCOPXY3D.jpg \
images/palettes/CUMSUM.jpg \
images/palettes/CurrentSensor.jpg \
images/palettes/CURV_f.jpg \
images/palettes/CVS.jpg \
images/palettes/DEADBAND.jpg \
images/palettes/DEBUG_SCICOS.jpg \
images/palettes/DELAY_f.jpg \
images/palettes/DELAYV_f.jpg \
images/palettes/DEMUX.jpg \
images/palettes/DERIV.jpg \
images/palettes/DFLIPFLOP.jpg \
images/palettes/Diode.jpg \
images/palettes/DLATCH.jpg \
images/palettes/DLR.jpg \
images/palettes/DLRADAPT_f.jpg \
images/palettes/DLSS.jpg \
images/palettes/DOLLAR_f.jpg \
images/palettes/EDGE_TRIGGER.jpg \
images/palettes/ENDBLK.jpg \
images/palettes/END_c.jpg \
images/palettes/ESELECT_f.jpg \
images/palettes/EVTDLY_c.jpg \
images/palettes/EVTGEN_f.jpg \
images/palettes/EVTVARDLY.jpg \
images/palettes/EXPBLK_m.jpg \
images/palettes/EXPRESSION.jpg \
images/palettes/EXTRACT.jpg \
images/palettes/Extract_Activation.jpg \
images/palettes/EXTRACTBITS.jpg \
images/palettes/EXTRACTOR.jpg \
images/palettes/EXTTRI.jpg \
images/palettes/fortran_block.jpg \
images/palettes/freq_div.jpg \
images/palettes/FROM.jpg \
images/palettes/FROMMO.jpg \
images/palettes/FROMWSB.jpg \
images/palettes/GAINBLK_f.jpg \
images/palettes/GENERAL_f.jpg \
images/palettes/generic_block3.jpg \
images/palettes/GENSIN_f.jpg \
images/palettes/GENSQR_f.jpg \
images/palettes/GOTO.jpg \
images/palettes/GOTOMO.jpg \
images/palettes/GotoTagVisibility.jpg \
images/palettes/GotoTagVisibilityMO.jpg \
images/palettes/Ground.jpg \
images/palettes/Gyrator.jpg \
images/palettes/HALT_f.jpg \
images/palettes/HYSTHERESIS.jpg \
images/palettes/IdealTransformer.jpg \
images/palettes/IFTHEL_f.jpg \
images/palettes/IN_f.jpg \
images/palettes/Inductor.jpg \
images/palettes/INIMPL_f.jpg \
images/palettes/INTEGRAL_f.jpg \
images/palettes/INTEGRAL_m.jpg \
images/palettes/INTMUL.jpg \
images/palettes/INTRP2BLK_f.jpg \
images/palettes/INTRPLBLK_f.jpg \
images/palettes/INVBLK.jpg \
images/palettes/ISELECT_m.jpg \
images/palettes/JKFLIPFLOP.jpg \
images/palettes/LOGBLK_f.jpg \
images/palettes/LOGIC.jpg \
images/palettes/LOGICAL_OP.jpg \
images/palettes/LOOKUP_f.jpg \
images/palettes/M_freq.jpg \
images/palettes/M_SWITCH.jpg \
images/palettes/MATBKSL.jpg \
images/palettes/MATCATH.jpg \
images/palettes/MATCATV.jpg \
images/palettes/MATDET.jpg \
images/palettes/MATDIAG.jpg \
images/palettes/MATDIV.jpg \
images/palettes/MATEIG.jpg \
images/palettes/MATEXPM.jpg \
images/palettes/MATINV.jpg \
images/palettes/MATLU.jpg \
images/palettes/MATMAGPHI.jpg \
images/palettes/MATMUL.jpg \
images/palettes/MATPINV.jpg \
images/palettes/MATRESH.jpg \
images/palettes/MATSING.jpg \
images/palettes/MATSUM.jpg \
images/palettes/MATTRAN.jpg \
images/palettes/MATZCONJ.jpg \
images/palettes/MATZREIM.jpg \
images/palettes/MAX_f.jpg \
images/palettes/MAXMIN.jpg \
images/palettes/MBLOCK.jpg \
images/palettes/MCLOCK_f.jpg \
images/palettes/MFCLCK_f.jpg \
images/palettes/MIN_f.jpg \
images/palettes/Modulo_Count.jpg \
images/palettes/MUX.jpg \
images/palettes/NEGTOPOS_f.jpg \
images/palettes/NMOS.jpg \
images/palettes/NPN.jpg \
images/palettes/NRMSOM_f.jpg \
images/palettes/OpAmp.jpg \
images/palettes/OUT_f.jpg \
images/palettes/OUTIMPL_f.jpg \
images/palettes/PDE.jpg \
images/palettes/PerteDP.jpg \
images/palettes/PID.jpg \
images/palettes/PMOS.jpg \
images/palettes/PNP.jpg \
images/palettes/POSTONEG_f.jpg \
images/palettes/PotentialSensor.jpg \
images/palettes/POWBLK_f.jpg \
images/palettes/PROD_f.jpg \
images/palettes/PRODUCT.jpg \
images/palettes/PuitsP.jpg \
images/palettes/QUANT_f.jpg \
images/palettes/RAMP.jpg \
images/palettes/RAND_m.jpg \
images/palettes/RATELIMITER.jpg \
images/palettes/READAU_f.jpg \
images/palettes/READC_f.jpg \
images/palettes/REGISTER.jpg \
images/palettes/RELATIONALOP.jpg \
images/palettes/RELAY_f.jpg \
images/palettes/Resistor.jpg \
images/palettes/RFILE_f.jpg \
images/palettes/RICC.jpg \
images/palettes/ROOTCOEF.jpg \
images/palettes/SAMPHOLD_m.jpg \
images/palettes/SampleCLK.jpg \
images/palettes/SATURATION.jpg \
images/palettes/SAWTOOTH_f.jpg \
images/palettes/scifunc_block_m.jpg \
images/palettes/SELECT_m.jpg \
images/palettes/SHIFT.jpg \
images/palettes/Sigbuilder.jpg \
images/palettes/SIGNUM.jpg \
images/palettes/SINBLK_f.jpg \
images/palettes/SineVoltage.jpg \
images/palettes/SourceP.jpg \
images/palettes/SQRT.jpg \
images/palettes/SRFLIPFLOP.jpg \
images/palettes/STEP_FUNCTION.jpg \
images/palettes/SUBMAT.jpg \
images/palettes/SUM_f.jpg \
images/palettes/SUMMATION.jpg \
images/palettes/SUPER_f.jpg \
images/palettes/Switch.jpg \
images/palettes/SWITCH2_m.jpg \
images/palettes/SWITCH_f.jpg \
images/palettes/TANBLK_f.jpg \
images/palettes/TCLSS.jpg \
images/palettes/TEXT_f.jpg \
images/palettes/TIME_DELAY.jpg \
images/palettes/TIME_f.jpg \
images/palettes/TKSCALE.jpg \
images/palettes/TOWS_c.jpg \
images/palettes/TRASH_f.jpg \
images/palettes/TrigFun.jpg \
images/palettes/VanneReglante.jpg \
images/palettes/VARIABLE_DELAY.jpg \
images/palettes/VariableResistor.jpg \
images/palettes/VoltageSensor.jpg \
images/palettes/VsourceAC.jpg \
images/palettes/VVsourceAC.jpg \
images/palettes/WFILE_f.jpg \
images/palettes/WRITEAU_f.jpg \
images/palettes/WRITEC_f.jpg \
images/palettes/ZCROSS_f.jpg

if GUI
if HDF5
USEANT=1
endif
endif

include $(top_srcdir)/Makefile.incl.am
