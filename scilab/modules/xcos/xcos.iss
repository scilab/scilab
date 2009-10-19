;##############################################################################################################
; Scilab ( http://mwww.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2009 - DIGITEO - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; xcos module
;--------------------------------------------------------------------------------------------------------------
;
#define XCOS "xcos"
;
Source: bin\xcos.dll; DestDir: {app}\bin; Components: {#COMPN_XCOS}
;
Source: thirdparty\jgraphx.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
;
Source: modules\{#XCOS}\jar\org.scilab.modules.xcos.jar; DestDir: {app}\modules\{#XCOS}\jar; Components: {#COMPN_XCOS}
;
Source: modules\{#XCOS}\VERSION.xml; DestDir: {app}\modules\{#XCOS}; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\readme.txt; DestDir: {app}\modules\{#XCOS}; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\license.txt; DestDir: {app}\modules\{#XCOS}; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\changelog.txt; DestDir: {app}\modules\{#XCOS}; Components: {#COMPN_XCOS}
;
Source: modules\{#XCOS}\sci_gateway\xcos_gateway.xml; DestDir: {app}\modules\{#XCOS}\sci_gateway; Components: {#COMPN_XCOS}
;
Source: modules\{#XCOS}\etc\{#XCOS}.quit; DestDir: {app}\modules\{#XCOS}\etc; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\etc\{#XCOS}.start; DestDir: {app}\modules\{#XCOS}\etc; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\etc\Xcos-style.xml; DestDir: {app}\modules\{#XCOS}\etc; Components: {#COMPN_XCOS}
;
;Source: modules\{#XCOS}\help\addchapter.sce; DestDir: {app}\modules\{#XCOS}\help; Flags: recursesubdirs; Components:  {#COMPN_XCOS}
;
Source: modules\{#XCOS}\includes\*.h; DestDir: {app}\modules\{#XCOS}\includes; Components: {#COMPN_XCOS}
;
Source: modules\{#XCOS}\images\icons\*.png; DestDir: {app}\modules\{#XCOS}\images\icons; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\images\blocks\*.gif; DestDir: {app}\modules\{#XCOS}\images\blocks; Components: {#COMPN_XCOS}
;
Source: modules\{#XCOS}\macros\buildmacros.sce; DestDir: {app}\modules\{#XCOS}\macros; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\macros\buildmacros.bat; DestDir: {app}\modules\{#XCOS}\macros; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#XCOS}\macros; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\macros\lib; DestDir: {app}\modules\{#XCOS}\macros; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\macros\names; DestDir: {app}\modules\{#XCOS}\macros; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\macros\*.bin; DestDir: {app}\modules\{#XCOS}\macros; Components: {#COMPN_XCOS}
Source: modules\{#XCOS}\macros\*.sci; DestDir: {app}\modules\{#XCOS}\macros; Components: {#COMPN_XCOS}
;
;
;
Source: modules\{#XCOS}\demos\*.*; DestDir: {app}\modules\{#XCOS}\demos; Flags: recursesubdirs; Components: {#COMPN_XCOS}
;
Source: modules\{#XCOS}\tests\*.*; DestDir: {app}\modules\{#XCOS}\tests; Flags: recursesubdirs; Components: {#COMPN_XCOS} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
