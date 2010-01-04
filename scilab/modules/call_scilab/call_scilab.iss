;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2009 - DIGITEO - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;

;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; call_scilab module
;--------------------------------------------------------------------------------------------------------------
;
#define MODULE_CALL_SCILAB "call_scilab"
;
Source: bin\call_scilab.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\call_scilab.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#MODULE_CALL_SCILAB}\changelog.txt; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}; Components: {#COMPN_SCILAB}
Source: modules\{#MODULE_CALL_SCILAB}\license.txt; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}; Components: {#COMPN_SCILAB}
Source: modules\{#MODULE_CALL_SCILAB}\readme.txt; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}; Components: {#COMPN_SCILAB}
Source: modules\{#MODULE_CALL_SCILAB}\version.xml; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}; Components: {#COMPN_SCILAB}
;
;Source: modules\{#MODULE_CALL_SCILAB}\includes\*.h; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#MODULE_CALL_SCILAB}\includes\call_scilab.h; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#MODULE_CALL_SCILAB}\includes\CallScilab.h; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#MODULE_CALL_SCILAB}\includes\dynlib_call_scilab.h; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#MODULE_CALL_SCILAB}\sci_gateway\call_scilab_gateway.xml; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#MODULE_CALL_SCILAB}\etc\{#MODULE_CALL_SCILAB}.quit; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#MODULE_CALL_SCILAB}\etc\{#MODULE_CALL_SCILAB}.start; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#MODULE_CALL_SCILAB}\examples\*.*; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}\examples; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
;Source: modules\{#MODULE_CALL_SCILAB}\help\addchapter.sce; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
Source: modules\{#MODULE_CALL_SCILAB}\macros\buildmacros.sce; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MODULE_CALL_SCILAB}\macros\buildmacros.bat; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MODULE_CALL_SCILAB}\macros\cleanmacros.bat; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#MODULE_CALL_SCILAB}\macros\*.bin; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#MODULE_CALL_SCILAB}\macros\*.sci; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#MODULE_CALL_SCILAB}\macros\lib; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#MODULE_CALL_SCILAB}\macros\names; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#MODULE_CALL_SCILAB}\tests\*.*; DestDir: {app}\modules\{#MODULE_CALL_SCILAB}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
