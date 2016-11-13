#
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) DIGITEO - 2009-2010 - Allan CORNET <allan.cornet@scilab.org>
# Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.
#

#### Target ######
modulename=xcos


#### xcos : Conf files ####
libscixcos_la_rootdir = $(mydatadir)
libscixcos_la_root_DATA =  license.txt


#### xcos : init scripts ####
libscixcos_la_etcdir = $(mydatadir)/etc
libscixcos_la_etc_DATA = \
	etc/xcos.quit \
	etc/xcos.start \
	etc/Xcos-style.xml \
	etc/XcosFile.xsd \
	etc/xcos.xml \
	etc/XcosConfiguration.xsd \
	etc/palettes.xml \
	etc/PaletteConfiguration.xsd \
	etc/Modelica.xsd \
	etc/XConfiguration-xcos.xsl \
	etc/XConfiguration-xcos.xml

XCOS_CXX_SOURCES =	\
    src/jni/Xcos.cpp \
    src/jni/Xcos.hxx \
    src/jni/XcosCellFactory.cpp \
    src/jni/XcosCellFactory.hxx \
    src/jni/Palette.cpp \
    src/jni/Palette.hxx \
    src/jni/Modelica.cpp \
    src/jni/Modelica.hxx \
    src/jni/JavaController_wrap.cxx \
    src/cpp/xcosUtilities.cpp \
    src/cpp/loadStatus.cpp \
    src/jni/JavaXMIResource_wrap.cxx

GIWS_WRAPPERS = \
    src/jni/Xcos.giws.xml \
    src/jni/XcosCellFactory.giws.xml \
    src/jni/Palette.giws.xml \
    src/jni/Modelica.giws.xml
SWIG_WRAPPERS = \
    src/jni/JavaController.i \
    src/jni/JavaXMIResource.i
    
# FORCE directors due to a bug into swig
SWIG_OPTIONS=-c++ -directors
BUILT_SOURCES=
if GIWS
BUILT_SOURCES+=giws
endif
if SWIG
BUILT_SOURCES+=swig
endif


GATEWAY_C_SOURCES =

GATEWAY_CXX_SOURCES = \
	sci_gateway/cpp/sci_Xcos.cpp \
	sci_gateway/cpp/sci_warnBlockByUID.cpp \
	sci_gateway/cpp/sci_closeXcosFromScilab.cpp \
	sci_gateway/cpp/sci_xcosCellCreated.cpp \
	sci_gateway/cpp/sci_xcosDiagramToScilab.cpp \
	sci_gateway/cpp/sci_xcosPalLoad.cpp \
	sci_gateway/cpp/sci_xcosPalCategoryAdd.cpp \
	sci_gateway/cpp/sci_xcosPalDelete.cpp \
	sci_gateway/cpp/sci_xcosPalMove.cpp \
	sci_gateway/cpp/sci_xcosPalEnable.cpp \
	sci_gateway/cpp/sci_xcosPalDisable.cpp \
	sci_gateway/cpp/sci_xcosPalGenerateIcon.cpp \
	sci_gateway/cpp/sci_xcosPalGet.cpp \
	sci_gateway/cpp/sci_xcosConfigureXmlFile.cpp \
	sci_gateway/cpp/sci_xcosAddToolsMenu.cpp \
	sci_gateway/cpp/sci_loadXcos.cpp \
	sci_gateway/cpp/sci_xcosSimulationStarted.cpp

libscixcos_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(srcdir)/src/jni/ \
    -I$(srcdir)/src/cpp/ \
    -I$(srcdir)/src/c/ \
    -I$(top_srcdir)/modules/scicos/includes/ \
    -I$(top_srcdir)/modules/scicos_blocks/includes/ \
    -I$(top_srcdir)/modules/scicos_blocks/src/jni/ \
    -I$(top_srcdir)/modules/dynamic_link/includes/ \
    -I$(top_srcdir)/modules/string/includes/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/ast/includes/analysis/ \
    -I$(top_srcdir)/modules/threads/includes/ \
    -I$(top_srcdir)/modules/console/includes/ \
    -I$(top_srcdir)/modules/jvm/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes/ \
    -I$(top_srcdir)/modules/commons/src/jni/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/fileio/includes/ \
    -I$(top_srcdir)/modules/api_scilab/includes/ \
    $(XML_FLAGS) \
    $(JAVA_JNI_INCLUDE) \
    $(AM_CPPFLAGS)

# Without the xcos module
libscixcos_disable_la_CPPFLAGS = \
    -I$(srcdir)/includes/ \
    -I$(top_srcdir)/modules/ast/includes/ast/ \
    -I$(top_srcdir)/modules/ast/includes/exps/ \
    -I$(top_srcdir)/modules/ast/includes/operations/ \
    -I$(top_srcdir)/modules/ast/includes/parse/ \
    -I$(top_srcdir)/modules/ast/includes/symbol/ \
    -I$(top_srcdir)/modules/ast/includes/system_env/ \
    -I$(top_srcdir)/modules/ast/includes/types/ \
    -I$(top_srcdir)/modules/localization/includes/ \
    -I$(top_srcdir)/modules/output_stream/includes \
    $(XML_FLAGS) \
    $(AM_CPPFLAGS)

XCOS_DISABLE_C_SOURCES = src/noxcos/noxcos.c
libscixcos_disable_la_SOURCES = $(XCOS_DISABLE_C_SOURCES)

pkglib_LTLIBRARIES = libscixcos-disable.la

if XCOS
if GUI
noinst_LTLIBRARIES = libscixcos-algo.la
pkglib_LTLIBRARIES += libscixcos.la
endif
endif


libscixcos_algo_la_SOURCES = $(XCOS_C_SOURCES) $(XCOS_CXX_SOURCES)
libscixcos_la_SOURCES =  $(GATEWAY_C_SOURCES) $(GATEWAY_CXX_SOURCES)
libscixcos_algo_la_CPPFLAGS = $(libscixcos_la_CPPFLAGS)

# For the code check (splint)
CHECK_SRC= $(XCOS_C_SOURCES) $(GATEWAY_C_SOURCES)
INCLUDE_FLAGS = $(libscixcos_la_CPPFLAGS)



libscixcos_la_LIBADD = libscixcos-algo.la \
		$(top_builddir)/modules/scicos/libsciscicos.la \
		$(top_builddir)/modules/commons/libscicommons.la \
		$(top_builddir)/modules/jvm/libscijvm.la \
		$(LIBXML_LIBS)

#### xcos : gateway declaration ####
libscixcos_la_sci_gatewaydir = $(mydatadir)/sci_gateway
libscixcos_la_sci_gateway_DATA = sci_gateway/xcos_gateway.xml


#### xcos : images files ####
#
# Generated with:
#   $ find images/ -type f -printf "%p \\\\\n" -name *.svg -or -name *.gif -or -name *.jpg -or -name *.png -not -name *_pal.jpg -not -name gui |sort |awk '$0 !~ /gui/{print;}' |awk '$0 !~ /_pal.jpg/{print;}'
libscixcos_la_imagesdir=$(mydatadir)/
nobase_libscixcos_la_images_DATA = \
images/blocks/3DSCOPE.svg \
images/blocks/ANDBLK.svg \
images/blocks/ASCOPE.svg \
images/blocks/BACHE.svg \
images/blocks/BARXY.svg \
images/blocks/BPLATFORM.svg \
images/blocks/Capacitor.svg \
images/blocks/CCS.svg \
images/blocks/CLOCK_c.svg \
images/blocks/CLOCK_f.svg \
images/blocks/ConstantVoltage.svg \
images/blocks/CSCOPXY.svg \
images/blocks/CSCOPXY3D.svg \
images/blocks/CurrentSensor.svg \
images/blocks/CVS.svg \
images/blocks/DEADBAND.svg \
images/blocks/Diode.svg \
images/blocks/DSCOPE.svg \
images/blocks/Flowmeter.svg \
images/blocks/Ground.svg \
images/blocks/Gyrator.svg \
images/blocks/HYSTHERESIS.svg \
images/blocks/IdealTransformer.svg \
images/blocks/Inductor.svg \
images/blocks/INTEGRAL.svg \
images/blocks/NMOS.svg \
images/blocks/NPN.svg \
images/blocks/PerteDP.svg \
images/blocks/PMOS.svg \
images/blocks/PNP.svg \
images/blocks/PotentialSensor.svg \
images/blocks/PRODUCT.svg \
images/blocks/PuitP.svg \
images/blocks/PULSE_SC.svg \
images/blocks/QUANT_f.svg \
images/blocks/RAMP.svg \
images/blocks/Resistor.svg \
images/blocks/SampleCLK.svg \
images/blocks/SATURATION.svg \
images/blocks/sawtooth.svg \
images/blocks/Self_Switch_off.svg \
images/blocks/Self_Switch_on.svg \
images/blocks/SELF_SWITCH.svg \
images/blocks/SINUS_f.svg \
images/blocks/SourceP.svg \
images/blocks/SQUARE_WAVE_f.svg \
images/blocks/STEP_FUNCTION.svg \
images/blocks/SUM.svg \
images/blocks/SUPER.svg \
images/blocks/SWITCH.svg \
images/blocks/VanneReglante.svg \
images/blocks/VariableResistor.svg \
images/blocks/VoltageSensor.svg \
images/icons/16x16/actions/align-horizontal-center.png \
images/icons/16x16/actions/align-horizontal-left.png \
images/icons/16x16/actions/align-horizontal-right.png \
images/icons/16x16/actions/align-vertical-bottom.png \
images/icons/16x16/actions/align-vertical-center.png \
images/icons/16x16/actions/align-vertical-top.png \
images/icons/16x16/actions/color-fill.png \
images/icons/16x16/actions/draw-brush.png \
images/icons/16x16/actions/object-group.png \
images/icons/16x16/actions/process-stop.png \
images/icons/16x16/actions/select-by-color.png \
images/icons/16x16/actions/transform-flip.png \
images/icons/16x16/actions/transform-mirror.png \
images/icons/16x16/categories/preferences-desktop.png \
images/palettes/ABS_VALUE.png \
images/palettes/AFFICH_m.png \
images/palettes/ANDBLK.png \
images/palettes/ANDLOG_f.png \
images/palettes/AUTOMAT.png \
images/palettes/Bache.png \
images/palettes/BACKLASH.png \
images/palettes/BARXY.png \
images/palettes/BIGSOM_f.png \
images/palettes/BITCLEAR.png \
images/palettes/BITSET.png \
images/palettes/BOUNCE.png \
images/palettes/BOUNCEXY.png \
images/palettes/BPLATFORM.png \
images/palettes/CANIMXY3D.png \
images/palettes/CANIMXY.png \
images/palettes/Capacitor.png \
images/palettes/c_block.png \
images/palettes/CBLOCK.png \
images/palettes/CCS.png \
images/palettes/CEVENTSCOPE.png \
images/palettes/CFSCOPE.png \
images/palettes/CLINDUMMY_f.png \
images/palettes/CLKFROM.png \
images/palettes/CLKGOTO.png \
images/palettes/CLKGotoTagVisibility.png \
images/palettes/CLKINV_f.png \
images/palettes/CLKOUTV_f.png \
images/palettes/CLKSOMV_f.png \
images/palettes/CLOCK_c.png \
images/palettes/CLR.png \
images/palettes/CLSS.png \
images/palettes/CMAT3D.png \
images/palettes/CMATVIEW.png \
images/palettes/CMSCOPE.png \
images/palettes/ConstantVoltage.png \
images/palettes/CONST_f.png \
images/palettes/CONST_m.png \
images/palettes/CONST.png \
images/palettes/CONSTRAINT2_c.png \
images/palettes/CONSTRAINT_c.png \
images/palettes/CONVERT.png \
images/palettes/COSBLK_f.png \
images/palettes/Counter.png \
images/palettes/CSCOPE.png \
images/palettes/CSCOPXY3D.png \
images/palettes/CSCOPXY.png \
images/palettes/CUMSUM.png \
images/palettes/CurrentSensor.png \
images/palettes/CURV_f.png \
images/palettes/CVS.png \
images/palettes/DEADBAND.png \
images/palettes/DEBUG.png \
images/palettes/DELAY_f.png \
images/palettes/DELAYV_f.png \
images/palettes/DEMUX_f.png \
images/palettes/DEMUX.png \
images/palettes/DERIV.png \
images/palettes/DFLIPFLOP.png \
images/palettes/DIFF_f.png \
images/palettes/Diode.png \
images/palettes/DLATCH.png \
images/palettes/DLRADAPT_f.png \
images/palettes/DLR.png \
images/palettes/DLSS.png \
images/palettes/DOLLAR_f.png \
images/palettes/DOLLAR_m.png \
images/palettes/DOLLAR.png \
images/palettes/EDGE_TRIGGER.png \
images/palettes/ENDBLK.png \
images/palettes/END_c.png \
images/palettes/ESELECT_f.png \
images/palettes/EVTDLY_c.png \
images/palettes/EVTGEN_f.png \
images/palettes/EVTVARDLY.png \
images/palettes/EXPBLK_m.png \
images/palettes/EXPRESSION.png \
images/palettes/Extract_Activation.png \
images/palettes/EXTRACTBITS.png \
images/palettes/EXTRACTOR.png \
images/palettes/EXTRACT.png \
images/palettes/EXTTRI.png \
images/palettes/Flowmeter.png \
images/palettes/fortran_block.png \
images/palettes/freq_div.png \
images/palettes/FROMMO.png \
images/palettes/FROM.png \
images/palettes/FROMWSB.png \
images/palettes/GAINBLK_f.png \
images/palettes/GAINBLK.png \
images/palettes/GAIN_f.png \
images/palettes/GENERAL_f.png \
images/palettes/generic_block3.png \
images/palettes/GENSIN_f.png \
images/palettes/GENSQR_f.png \
images/palettes/GOTOMO.png \
images/palettes/GOTO.png \
images/palettes/GotoTagVisibilityMO.png \
images/palettes/GotoTagVisibility.png \
images/palettes/Ground.png \
images/palettes/Gyrator.png \
images/palettes/HALT_f.png \
images/palettes/HYSTHERESIS.png \
images/palettes/IdealTransformer.png \
images/palettes/IFTHEL_f.png \
images/palettes/Inductor.png \
images/palettes/IN_f.png \
images/palettes/INIMPL_f.png \
images/palettes/INTEGRAL_f.png \
images/palettes/INTEGRAL_m.png \
images/palettes/INTMUL.png \
images/palettes/INTRP2BLK_f.png \
images/palettes/INTRPLBLK_f.png \
images/palettes/INVBLK.png \
images/palettes/ISELECT_m.png \
images/palettes/JKFLIPFLOP.png \
images/palettes/LOGBLK_f.png \
images/palettes/LOGICAL_OP.png \
images/palettes/LOGIC.png \
images/palettes/LOOKUP_f.png \
images/palettes/MATBKSL.png \
images/palettes/MATCATH.png \
images/palettes/MATCATV.png \
images/palettes/MATDET.png \
images/palettes/MATDIAG.png \
images/palettes/MATDIV.png \
images/palettes/MATEIG.png \
images/palettes/MATEXPM.png \
images/palettes/MATINV.png \
images/palettes/MATLU.png \
images/palettes/MATMAGPHI.png \
images/palettes/MATMUL.png \
images/palettes/MATPINV.png \
images/palettes/MATRESH.png \
images/palettes/MATSING.png \
images/palettes/MATSUM.png \
images/palettes/MATTRAN.png \
images/palettes/MATZCONJ.png \
images/palettes/MATZREIM.png \
images/palettes/MAX_f.png \
images/palettes/MAXMIN.png \
images/palettes/MBLOCK.png \
images/palettes/MCLOCK_f.png \
images/palettes/MFCLCK_f.png \
images/palettes/M_freq.png \
images/palettes/MIN_f.png \
images/palettes/Modulo_Count.png \
images/palettes/M_SWITCH.png \
images/palettes/MUX_f.png \
images/palettes/MUX.png \
images/palettes/NEGTOPOS_f.png \
images/palettes/NMOS.png \
images/palettes/NPN.png \
images/palettes/NRMSOM_f.png \
images/palettes/OpAmp.png \
images/palettes/OUT_f.png \
images/palettes/OUTIMPL_f.png \
images/palettes/PDE.png \
images/palettes/PerteDP.png \
images/palettes/PID.png \
images/palettes/PMOS.png \
images/palettes/PNP.png \
images/palettes/POSTONEG_f.png \
images/palettes/PotentialSensor.png \
images/palettes/POWBLK_f.png \
images/palettes/PROD_f.png \
images/palettes/PRODUCT.png \
images/palettes/PuitsP.png \
images/palettes/PULSE_SC.png \
images/palettes/QUANT_f.png \
images/palettes/RAMP.png \
images/palettes/RAND_m.png \
images/palettes/RATELIMITER.png \
images/palettes/READAU_f.png \
images/palettes/READC_f.png \
images/palettes/REGISTER.png \
images/palettes/RELATIONALOP.png \
images/palettes/RELAY_f.png \
images/palettes/Resistor.png \
images/palettes/RFILE_f.png \
images/palettes/RICC.png \
images/palettes/ROOTCOEF.png \
images/palettes/SAMPHOLD_m.png \
images/palettes/SampleCLK.png \
images/palettes/SATURATION.png \
images/palettes/SAWTOOTH_f.png \
images/palettes/SCALAR2VECTOR.png \
images/palettes/scifunc_block_m.png \
images/palettes/SELECT_m.png \
images/palettes/SELF_SWITCH.png \
images/palettes/SHIFT.png \
images/palettes/Sigbuilder.png \
images/palettes/SIGNUM.png \
images/palettes/SINBLK_f.png \
images/palettes/SineVoltage.png \
images/palettes/SOM_f.png \
images/palettes/SourceP.png \
images/palettes/SQRT.png \
images/palettes/SRFLIPFLOP.png \
images/palettes/STEP_FUNCTION.png \
images/palettes/SUBMAT.png \
images/palettes/SUM_f.png \
images/palettes/SUMMATION.png \
images/palettes/SUPER_f.png \
images/palettes/SWITCH2_m.png \
images/palettes/SWITCH_f.png \
images/palettes/Switch.png \
images/palettes/TANBLK_f.png \
images/palettes/TCLSS.png \
images/palettes/TEXT_f.png \
images/palettes/TIME_DELAY.png \
images/palettes/TIME_f.png \
images/palettes/TKSCALE.png \
images/palettes/TOWS_c.png \
images/palettes/TRASH_f.png \
images/palettes/TrigFun.png \
images/palettes/VanneReglante.png \
images/palettes/VARIABLE_DELAY.png \
images/palettes/VariableResistor.png \
images/palettes/VirtualCLK0.png \
images/palettes/VoltageSensor.png \
images/palettes/VsourceAC.png \
images/palettes/VVsourceAC.png \
images/palettes/WRITEAU_f.png \
images/palettes/WRITEC_f.png \
images/palettes/ZCROSS_f.png

if XCOS
if GUI
USEANT=1
endif
endif

include $(top_srcdir)/Makefile.incl.am
