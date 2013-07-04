;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2009 - Allan CORNET
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
; functions module
;--------------------------------------------------------------------------------------------------------------
;
#define FUNCTIONS "functions"
;
Source: bin\{#FUNCTIONS}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#FUNCTIONS}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#FUNCTIONS}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#FUNCTIONS}_f.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#FUNCTIONS}\license.txt; DestDir: {app}\modules\{#FUNCTIONS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#FUNCTIONS}\sci_gateway\{#FUNCTIONS}_gateway.xml; DestDir: {app}\modules\{#FUNCTIONS}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#FUNCTIONS}\etc\{#FUNCTIONS}.quit; DestDir: {app}\modules\{#FUNCTIONS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#FUNCTIONS}\etc\{#FUNCTIONS}.start; DestDir: {app}\modules\{#FUNCTIONS}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#FUNCTIONS}\includes\*.h; DestDir: {app}\modules\{#FUNCTIONS}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#FUNCTIONS}\macros\lib; DestDir: {app}\modules\{#FUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FUNCTIONS}\macros\names; DestDir: {app}\modules\{#FUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FUNCTIONS}\macros\*.bin; DestDir: {app}\modules\{#FUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FUNCTIONS}\macros\*.sci; DestDir: {app}\modules\{#FUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FUNCTIONS}\macros\buildmacros.sce; DestDir: {app}\modules\{#FUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FUNCTIONS}\macros\buildmacros.bat; DestDir: {app}\modules\{#FUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FUNCTIONS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#FUNCTIONS}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#FUNCTIONS}\xml\*.dtd; DestDir: {app}\modules\{#FUNCTIONS}\xml; Components: {#COMPN_SCILAB}
;
;Source: modules\{#FUNCTIONS}\demos\*.*; DestDir: {app}\modules\{#FUNCTIONS}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#FUNCTIONS}\scripts\buildmacros\*.sce; DestDir: {app}\modules\{#FUNCTIONS}\scripts\buildmacros; Components: {#COMPN_SCILAB}
Source: modules\{#FUNCTIONS}\scripts\buildmacros\*.bat; DestDir: {app}\modules\{#FUNCTIONS}\scripts\buildmacros; Components: {#COMPN_SCILAB}
;
Source: modules\{#FUNCTIONS}\examples\*.*; DestDir: {app}\modules\{#FUNCTIONS}\examples; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#FUNCTIONS}\tests\*.*; DestDir: {app}\modules\{#FUNCTIONS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
