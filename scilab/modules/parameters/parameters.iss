;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
; parameters module
;--------------------------------------------------------------------------------------------------------------
;
#define PARAMETERS "parameters"
;
Source: bin\{#PARAMETERS}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#PARAMETERS}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#PARAMETERS}\includes\parameters.h; DestDir: {app}\modules\{#PARAMETERS}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#PARAMETERS}\license.txt; DestDir: {app}\modules\{#PARAMETERS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#PARAMETERS}\etc\{#PARAMETERS}.quit; DestDir: {app}\modules\{#PARAMETERS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#PARAMETERS}\etc\{#PARAMETERS}.start; DestDir: {app}\modules\{#PARAMETERS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#PARAMETERS}\macros\lib; DestDir: {app}\modules\{#PARAMETERS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#PARAMETERS}\macros\names; DestDir: {app}\modules\{#PARAMETERS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#PARAMETERS}\macros\*.bin; DestDir: {app}\modules\{#PARAMETERS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#PARAMETERS}\macros\*.sci; DestDir: {app}\modules\{#PARAMETERS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#PARAMETERS}\macros\buildmacros.sce; DestDir: {app}\modules\{#PARAMETERS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#PARAMETERS}\macros\buildmacros.bat; DestDir: {app}\modules\{#PARAMETERS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#PARAMETERS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#PARAMETERS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#PARAMETERS}\tests\*.*; DestDir: {app}\modules\{#PARAMETERS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
