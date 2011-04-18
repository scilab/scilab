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
; intersci module
;--------------------------------------------------------------------------------------------------------------
;
#define INTERSCI "intersci"
;
Source: bin\{#INTERSCI}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#INTERSCI}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTERSCI}\license.txt; DestDir: {app}\modules\{#INTERSCI}; Components: {#COMPN_INTERSCI}
;
Source: modules\{#INTERSCI}\etc\{#INTERSCI}.quit; DestDir: {app}\modules\{#INTERSCI}\etc; Components: {#COMPN_INTERSCI}
Source: modules\{#INTERSCI}\etc\{#INTERSCI}.start; DestDir: {app}\modules\{#INTERSCI}\etc; Components: {#COMPN_INTERSCI}
;
Source: modules\{#INTERSCI}\includes\*.h; DestDir: {app}\modules\{#INTERSCI}\includes; Components: {#COMPN_INTERSCI}
;
;Source: modules\{#INTERSCI}\macros\lib; DestDir: {app}\modules\{#INTERSCI}\macros; Components: {#COMPN_INTERSCI}
;Source: modules\{#INTERSCI}\macros\names; DestDir: {app}\modules\{#INTERSCI}\macros; Components: {#COMPN_INTERSCI}
;Source: modules\{#INTERSCI}\macros\*.bin; DestDir: {app}\modules\{#INTERSCI}\macros; Components: {#COMPN_INTERSCI}
;Source: modules\{#INTERSCI}\macros\*.sci; DestDir: {app}\modules\{#INTERSCI}\macros; Components: {#COMPN_INTERSCI}
Source: modules\{#INTERSCI}\macros\buildmacros.sce; DestDir: {app}\modules\{#INTERSCI}\macros; Components: {#COMPN_INTERSCI}
Source: modules\{#INTERSCI}\macros\buildmacros.bat; DestDir: {app}\modules\{#INTERSCI}\macros; Components: {#COMPN_INTERSCI}
Source: modules\{#INTERSCI}\macros\cleanmacros.bat; DestDir: {app}\modules\{#INTERSCI}\macros; Components: {#COMPN_INTERSCI}
;
;Source: modules\{#INTERSCI}\demos\*.*; DestDir: {app}\modules\{#INTERSCI}\demos; Flags: recursesubdirs; Components: {#COMPN_INTERSCI}
;
Source: modules\{#INTERSCI}\bin\*.exe; DestDir: {app}\modules\{#INTERSCI}\bin; Components: {#COMPN_INTERSCI}
;
;Source: modules\{#INTERSCI}\examples\*.*; DestDir: {app}\modules\{#INTERSCI}\examples; Flags: recursesubdirs; Components: {#COMPN_INTERSCI}
;
Source: modules\{#INTERSCI}\tests\*.*; DestDir: {app}\modules\{#INTERSCI}\tests; Flags: recursesubdirs; Components: {#COMPN_INTERSCI} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
