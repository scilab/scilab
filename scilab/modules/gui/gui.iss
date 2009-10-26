;##############################################################################################################
; Scilab ( http://mwww.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2008 - INRIA - llan CORNET
; Version TRUNK
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; gui module
;--------------------------------------------------------------------------------------------------------------
;
#define GUI "gui"
;
Source: bin\scigui.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#GUI}\jar\org.scilab.modules.gui.jar; DestDir: {app}\modules\{#GUI}\jar; Components: {#COMPN_SCILAB}
Source: thirdparty\flexdock-0.5.1.jar; DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\commons-logging-1.1.jar; DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\looks-2.1.1.jar; DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\skinlf.jar; DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
;
Source: modules\{#GUI}\VERSION.xml; DestDir: {app}\modules\{#GUI}; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\readme.txt; DestDir: {app}\modules\{#GUI}; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\license.txt; DestDir: {app}\modules\{#GUI}; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\changelog.txt; DestDir: {app}\modules\{#GUI}; Components: {#COMPN_SCILAB}
;
Source: modules\{#GUI}\sci_gateway\gui_gateway.xml; DestDir: {app}\modules\{#GUI}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#GUI}\etc\{#GUI}.quit; DestDir: {app}\modules\{#GUI}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\etc\{#GUI}.start; DestDir: {app}\modules\{#GUI}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#GUI}\etc\*.xml; DestDir: {app}\modules\{#GUI}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\images\icons\*.png; DestDir: {app}\modules\{#GUI}\images\icons; Components: {#COMPN_SCILAB}
;
Source: modules\{#GUI}\help\addchapter.sce; DestDir: {app}\modules\{#GUI}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
;Source: modules\{#GUI}\includes\*.h; DestDir: {app}\modules\{#GUI}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#GUI}\macros\lib; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\macros\names; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\macros\*.bin; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\macros\*.sci; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\macros\buildmacros.sce; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\macros\buildmacros.bat; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\macros\cleanmacros.bat; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#GUI}\demos\*.*; DestDir: {app}\modules\{#GUI}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#GUI}\tests\*.*; DestDir: {app}\modules\{#GUI}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
