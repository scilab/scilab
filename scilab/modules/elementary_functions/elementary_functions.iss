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
; elementary_functions module
;--------------------------------------------------------------------------------------------------------------
;
#define ELEMFUNCTIONS "elementary_functions"
;
Source: bin\slatec_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\linpack_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\linpack_f.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\eispack_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#ELEMFUNCTIONS}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#ELEMFUNCTIONS}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#ELEMFUNCTIONS}\VERSION.xml; DestDir: {app}\modules\{#ELEMFUNCTIONS}; Components: {#COMPN_SCILAB}
Source: modules\{#ELEMFUNCTIONS}\license.txt; DestDir: {app}\modules\{#ELEMFUNCTIONS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#ELEMFUNCTIONS}\sci_gateway\{#ELEMFUNCTIONS}_gateway.xml; DestDir: {app}\modules\{#ELEMFUNCTIONS}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#ELEMFUNCTIONS}\etc\{#ELEMFUNCTIONS}.quit; DestDir: {app}\modules\{#ELEMFUNCTIONS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#ELEMFUNCTIONS}\etc\{#ELEMFUNCTIONS}.start; DestDir: {app}\modules\{#ELEMFUNCTIONS}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#ELEMFUNCTIONS}\includes\*.h; DestDir: {app}\modules\{#ELEMFUNCTIONS}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#ELEMFUNCTIONS}\macros\buildmacros.sce; DestDir: {app}\modules\{#ELEMFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ELEMFUNCTIONS}\macros\buildmacros.bat; DestDir: {app}\modules\{#ELEMFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ELEMFUNCTIONS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#ELEMFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ELEMFUNCTIONS}\macros\lib; DestDir: {app}\modules\{#ELEMFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ELEMFUNCTIONS}\macros\names; DestDir: {app}\modules\{#ELEMFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ELEMFUNCTIONS}\macros\*.bin; DestDir: {app}\modules\{#ELEMFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ELEMFUNCTIONS}\macros\*.sci; DestDir: {app}\modules\{#ELEMFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#ELEMFUNCTIONS}\demos\*.*; DestDir: {app}\modules\{#ELEMFUNCTIONS}\demos; Flags: recursesubdirs;  Components: {#COMPN_SCILAB}
;
Source: modules\{#ELEMFUNCTIONS}\tests\*.*; DestDir: {app}\modules\{#ELEMFUNCTIONS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
