;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;
;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Version TRUNK
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; helptools module
;--------------------------------------------------------------------------------------------------------------
;
#define HELPTOOLS "helptools"

Source: bin\helptools.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\jar\*.jar; DestDir: {app}\modules\{#HELPTOOLS}\jar; Components: {#COMPN_SCILAB}

#ifdef WITH_CHM
Source: modules\{#HELPTOOLS}\chm\*.chm; DestDir: {app}\modules\{#HELPTOOLS}\chm; Components: {#COMPN_SCILAB}
#endif

Source: modules\{#HELPTOOLS}\sci_gateway\helptools_gateway.xml; DestDir: {app}\modules\{#HELPTOOLS}\sci_gateway; Components: {#COMPN_SCILAB}
Source: thirdparty\docbook\*.*;DestDir: {app}\thirdparty\docbook; Flags: recursesubdirs; Components: {#COMPN_SCILAB}

Source: thirdparty\jhall.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\avalon-framework.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\avalon-framework.LICENSE;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\avalon-framework.README;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\batik.LICENSE;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\batik.README;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\commons-io.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\commons-io.LICENSE;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\commons-io.README;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\commons-logging.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\commons-logging.LICENSE;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\commons-logging.README;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\docbook-xsl-saxon.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\fop-hyph.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\fop-hyph.LICENSE;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\fop-hyph.README;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\fop.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\fop.LICENSE;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\fop.README;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\isorelax.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\jeuclid-core.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\jeuclid.LICENSE;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\jeuclid.README;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\jimi.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\jimi.LICENSE;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\jimi.README;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\jing.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\jing.LICENSE;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\jing.README;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\js.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\js.LICENSE;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\js.README;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\saxon.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\saxon.LICENSE;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\saxon.README;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\xercesImpl.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\xml-apis.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\xml-apis-ext.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\xmlgraphics-commons-1.3.1.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\xmlgraphics-commons.LICENSE;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\xmlgraphics-commons.README;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
;
Source: thirdparty\batik-all-1.7.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\VERSION.xml; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\readme.txt; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\license.txt; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\changelog.txt; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
;
;Source: modules\{#HELPTOOLS}\*.xsl; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
;Source: modules\{#HELPTOOLS}\help.dtd; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\css\*.*; DestDir: {app}\modules\{#HELPTOOLS}\css; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\doc\*.*; DestDir: {app}\modules\{#HELPTOOLS}\doc; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\schema\*.*; DestDir: {app}\modules\{#HELPTOOLS}\schema; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\xsl\*.*; DestDir: {app}\modules\{#HELPTOOLS}\xsl; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\etc\{#HELPTOOLS}.quit; DestDir: {app}\modules\{#HELPTOOLS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\etc\{#HELPTOOLS}.start; DestDir: {app}\modules\{#HELPTOOLS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\help\addchapter.sce; DestDir: {app}\modules\{#HELPTOOLS}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\macros\buildmacros.sce; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\macros\buildmacros.bat; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\macros\lib; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\macros\names; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\macros\*.bin; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\macros\*.sci; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\bin\man2sci.bat; DestDir: {app}\modules\{#HELPTOOLS}\bin; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\bin\manrev2sci.bat; DestDir: {app}\modules\{#HELPTOOLS}\bin; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\bin\scivalid.bat; DestDir: {app}\modules\{#HELPTOOLS}\bin; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\bin\sciviewhelp.bat; DestDir: {app}\modules\{#HELPTOOLS}\bin; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\examples\*.*; DestDir: {app}\modules\{#HELPTOOLS}\examples; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\tests\*.*; DestDir: {app}\modules\{#HELPTOOLS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
