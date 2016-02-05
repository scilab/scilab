;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) DIGITEO - 2010 - Allan CORNET
;
; Copyright (C) 2012 - 2016 - Scilab Enterprises
;
; This file is hereby licensed under the terms of the GNU GPL v2.0,
; pursuant to article 5.3.4 of the CeCILL v.2.1.
; This file was originally licensed under the terms of the CeCILL v2.1,
; and continues to be available under such terms.
; For more information, see the COPYING file which you should have received
; along with this program.
;
;--------------------------------------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;--------------------------------------------------------------------------------------------------------------
; helptools module
;--------------------------------------------------------------------------------------------------------------
;
#define HELPTOOLS "helptools"

Source: bin\{#HELPTOOLS}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#HELPTOOLS}\jar\org.scilab.modules.helptools.jar; DestDir: {app}\modules\{#HELPTOOLS}\jar; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#HELPTOOLS}\jar\scilab_*_*_help.jar; DestDir: {app}\modules\{#HELPTOOLS}\jar; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#HELPTOOLS}\jar\scilab_images.jar; DestDir: {app}\modules\{#HELPTOOLS}\jar; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#HELPTOOLS}\sci_gateway\{#HELPTOOLS}_gateway.xml; DestDir: {app}\modules\{#HELPTOOLS}\sci_gateway; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: thirdparty\docbook\*.*;DestDir: {app}\thirdparty\docbook; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: thirdparty\jhall.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\avalon-framework.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\commons-io.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\commons-logging.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\docbook-xsl-saxon.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\fop.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\fontbox.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\jeuclid-core.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\jeuclid.LICENSE;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\jeuclid.README;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\saxon9he.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\saxon.LICENSE;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\saxon.README;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\xml-apis.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\xml-apis-ext.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\xmlgraphics-commons.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\batik-all.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#HELPTOOLS}\license.txt; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#HELPTOOLS}\etc\fopconf.xml; DestDir: {app}\modules\{#HELPTOOLS}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#HELPTOOLS}\etc\SciDocConf.xml; DestDir: {app}\modules\{#HELPTOOLS}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#HELPTOOLS}\help.dtd; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#HELPTOOLS}\schema\*.*; DestDir: {app}\modules\{#HELPTOOLS}\schema; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#HELPTOOLS}\xsl\*.*; DestDir: {app}\modules\{#HELPTOOLS}\xsl; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#HELPTOOLS}\data\*.*; DestDir: {app}\modules\{#HELPTOOLS}\data; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#HELPTOOLS}\etc\{#HELPTOOLS}.quit; DestDir: {app}\modules\{#HELPTOOLS}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#HELPTOOLS}\etc\{#HELPTOOLS}.start; DestDir: {app}\modules\{#HELPTOOLS}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#HELPTOOLS}\etc\MAIN_CHAPTERS; DestDir: {app}\modules\{#HELPTOOLS}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#HELPTOOLS}\macros\buildmacros.sce; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#HELPTOOLS}\macros\buildmacros.bat; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#HELPTOOLS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#HELPTOOLS}\macros\lib; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#HELPTOOLS}\macros\*.sci; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#HELPTOOLS}\macros\*.bin; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#HELPTOOLS}\examples\*.*; DestDir: {app}\modules\{#HELPTOOLS}\examples; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#HELPTOOLS}\tests\*.*; DestDir: {app}\modules\{#HELPTOOLS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS} and {#COMPN_JVM_MODULE}
;
;--------------------------------------------------------------------------------------------------------------
