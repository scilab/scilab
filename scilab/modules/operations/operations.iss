;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2014 - Scilab Enterprises
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
; operations module
;--------------------------------------------------------------------------------------------------------------
;
#define OPERATIONS "operations"
;
Source: bin\{#OPERATIONS}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#OPERATIONS}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#OPERATIONS}\includes\doublecomplex.h; DestDir: {app}\modules\{#OPERATIONS}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#OPERATIONS}\includes\dynlib_operations.hxx; DestDir: {app}\modules\{#OPERATIONS}\includes; Components: {#COMPN_SCILAB}
;
;Source: modules\{#OPERATIONS}\license.txt; DestDir: {app}\modules\{#OPERATIONS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#OPERATIONS}\etc\{#OPERATIONS}.quit; DestDir: {app}\modules\{#OPERATIONS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#OPERATIONS}\etc\{#OPERATIONS}.start; DestDir: {app}\modules\{#OPERATIONS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#OPERATIONS}\macros\lib; DestDir: {app}\modules\{#OPERATIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OPERATIONS}\macros\*.sci; DestDir: {app}\modules\{#OPERATIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OPERATIONS}\macros\*.bin; DestDir: {app}\modules\{#OPERATIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OPERATIONS}\macros\buildmacros.sce; DestDir: {app}\modules\{#OPERATIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OPERATIONS}\macros\buildmacros.bat; DestDir: {app}\modules\{#OPERATIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OPERATIONS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#OPERATIONS}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#OPERATIONS}\tests\*.*; DestDir: {app}\modules\{#OPERATIONS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
