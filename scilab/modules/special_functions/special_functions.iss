;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) DIGITEO - 2010 - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;
;--------------------------------------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;--------------------------------------------------------------------------------------------------------------
; special_functions module
;--------------------------------------------------------------------------------------------------------------
;
#define SPFUNCTIONS "special_functions"
;
Source: bin\{#SPFUNCTIONS}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#SPFUNCTIONS}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPFUNCTIONS}\VERSION.xml; DestDir: {app}\modules\{#SPFUNCTIONS}; Components: {#COMPN_SCILAB}
Source: modules\{#SPFUNCTIONS}\readme.txt; DestDir: {app}\modules\{#SPFUNCTIONS}; Components: {#COMPN_SCILAB}
Source: modules\{#SPFUNCTIONS}\license.txt; DestDir: {app}\modules\{#SPFUNCTIONS}; Components: {#COMPN_SCILAB}
Source: modules\{#SPFUNCTIONS}\changelog.txt; DestDir: {app}\modules\{#SPFUNCTIONS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPFUNCTIONS}\sci_gateway\{#SPFUNCTIONS}_gateway.xml; DestDir: {app}\modules\{#SPFUNCTIONS}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPFUNCTIONS}\etc\{#SPFUNCTIONS}.quit; DestDir: {app}\modules\{#SPFUNCTIONS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#SPFUNCTIONS}\etc\{#SPFUNCTIONS}.start; DestDir: {app}\modules\{#SPFUNCTIONS}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#SPFUNCTIONS}\includes\*.h; DestDir: {app}\modules\{#SPFUNCTIONS}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPFUNCTIONS}\macros\lib; DestDir: {app}\modules\{#SPFUNCTIONS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SPFUNCTIONS}\macros\names; DestDir: {app}\modules\{#SPFUNCTIONS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SPFUNCTIONS}\macros\*.bin; DestDir: {app}\modules\{#SPFUNCTIONS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SPFUNCTIONS}\macros\*.sci; DestDir: {app}\modules\{#SPFUNCTIONS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SPFUNCTIONS}\macros\buildmacros.sce; DestDir: {app}\modules\{#SPFUNCTIONS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SPFUNCTIONS}\macros\buildmacros.bat; DestDir: {app}\modules\{#SPFUNCTIONS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SPFUNCTIONS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SPFUNCTIONS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;Source: modules\{#SPFUNCTIONS}\demos\*.*; DestDir: {app}\modules\{#SPFUNCTIONS}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPFUNCTIONS}\tests\*.*; DestDir: {app}\modules\{#SPFUNCTIONS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
