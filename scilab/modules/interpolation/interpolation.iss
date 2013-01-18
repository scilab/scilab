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
; interpolation module
;--------------------------------------------------------------------------------------------------------------
;
#define INTERPOLATION "interpolation"
;
Source: bin\{#INTERPOLATION}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#INTERPOLATION}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTERPOLATION}\license.txt; DestDir: {app}\modules\{#INTERPOLATION}; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTERPOLATION}\sci_gateway\{#INTERPOLATION}_gateway.xml; DestDir: {app}\modules\{#INTERPOLATION}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTERPOLATION}\etc\{#INTERPOLATION}.quit; DestDir: {app}\modules\{#INTERPOLATION}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#INTERPOLATION}\etc\{#INTERPOLATION}.start; DestDir: {app}\modules\{#INTERPOLATION}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#INTERPOLATION}\includes\*.h; DestDir: {app}\modules\{#INTERPOLATION}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTERPOLATION}\macros\lib; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#INTERPOLATION}\macros\names; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#INTERPOLATION}\macros\*.bin; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#INTERPOLATION}\macros\*.sci; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#INTERPOLATION}\macros\buildmacros.sce; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#INTERPOLATION}\macros\buildmacros.bat; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#INTERPOLATION}\macros\cleanmacros.bat; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTERPOLATION}\demos\*.*; DestDir: {app}\modules\{#INTERPOLATION}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTERPOLATION}\tests\*.*; DestDir: {app}\modules\{#INTERPOLATION}\tests; Flags: recursesubdirs; Components:{#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
