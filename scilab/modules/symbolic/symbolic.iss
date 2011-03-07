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
; symbolic module
;--------------------------------------------------------------------------------------------------------------
;
#define SYMBOLIC "symbolic"
;
Source: bin\{#SYMBOLIC}.dll; DestDir: {app}\bin; DestName: {#SYMBOLIC}.dll; Components: {#COMPN_SYMB}
Source: bin\{#SYMBOLIC}_f.dll; DestDir: {app}\bin; DestName: {#SYMBOLIC}_f.dll; Components: {#COMPN_SYMB}
;
Source: modules\{#SYMBOLIC}\VERSION.xml; DestDir: {app}\modules\{#SYMBOLIC}; Components: {#COMPN_SYMB}
Source: modules\{#SYMBOLIC}\license.txt; DestDir: {app}\modules\{#SYMBOLIC}; Components: {#COMPN_SYMB}
;
Source: modules\{#SYMBOLIC}\sci_gateway\{#SYMBOLIC}_gateway.xml; DestDir: {app}\modules\{#SYMBOLIC}\sci_gateway; Components: {#COMPN_SYMB}
;
Source: modules\{#SYMBOLIC}\etc\{#SYMBOLIC}.quit; DestDir: {app}\modules\{#SYMBOLIC}\etc; Components: {#COMPN_SYMB}
Source: modules\{#SYMBOLIC}\etc\{#SYMBOLIC}.start; DestDir: {app}\modules\{#SYMBOLIC}\etc; Components: {#COMPN_SYMB}
;
;Source: modules\{#SYMBOLIC}\includes\*.h; DestDir: {app}\modules\{#SYMBOLIC}\includes; Components: {#COMPN_SYMB}
;
;Source: modules\{#SYMBOLIC}\macros\lib; DestDir: {app}\modules\{#SYMBOLIC}\macros; Flags: recursesubdirs; Components: {#COMPN_SYMB}
;Source: modules\{#SYMBOLIC}\macros\names; DestDir: {app}\modules\{#SYMBOLIC}\macros; Flags: recursesubdirs; Components: {#COMPN_SYMB}
;Source: modules\{#SYMBOLIC}\macros\*.bin; DestDir: {app}\modules\{#SYMBOLIC}\macros; Flags: recursesubdirs; Components: {#COMPN_SYMB}
;Source: modules\{#SYMBOLIC}\macros\*.sci; DestDir: {app}\modules\{#SYMBOLIC}\macros; Flags: recursesubdirs; Components: {#COMPN_SYMB}
Source: modules\{#SYMBOLIC}\macros\buildmacros.sce; DestDir: {app}\modules\{#SYMBOLIC}\macros; Flags: recursesubdirs; Components: {#COMPN_SYMB}
Source: modules\{#SYMBOLIC}\macros\buildmacros.bat; DestDir: {app}\modules\{#SYMBOLIC}\macros; Flags: recursesubdirs; Components: {#COMPN_SYMB}
Source: modules\{#SYMBOLIC}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SYMBOLIC}\macros; Flags: recursesubdirs; Components: {#COMPN_SYMB}
;
;Source: modules\{#SYMBOLIC}\demos\*.*; DestDir: {app}\modules\{#SYMBOLIC}\demos; Flags: recursesubdirs; Components: {#COMPN_SYMB}
;
Source: modules\{#SYMBOLIC}\tests\*.*; DestDir: {app}\modules\{#SYMBOLIC}\tests; Flags: recursesubdirs; Components: {#COMPN_SYMB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
