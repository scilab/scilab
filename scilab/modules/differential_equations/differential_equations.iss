;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) DIGITEO - 2010 - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
;
;--------------------------------------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;--------------------------------------------------------------------------------------------------------------
; differential_equations module
;--------------------------------------------------------------------------------------------------------------
;
#define DIFFEQU "differential_equations"
;
Source: bin\{#DIFFEQU}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#DIFFEQU}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#DIFFEQU}\license.txt; DestDir: {app}\modules\{#DIFFEQU}; Components: {#COMPN_SCILAB}
;
Source: modules\{#DIFFEQU}\sci_gateway\{#DIFFEQU}_gateway.xml; DestDir: {app}\modules\{#DIFFEQU}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#DIFFEQU}\etc\{#DIFFEQU}.quit; DestDir: {app}\modules\{#DIFFEQU}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#DIFFEQU}\etc\{#DIFFEQU}.start; DestDir: {app}\modules\{#DIFFEQU}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#DIFFEQU}\includes\*.h; DestDir: {app}\modules\{#DIFFEQU}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#DIFFEQU}\macros\lib; DestDir: {app}\modules\{#DIFFEQU}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#DIFFEQU}\macros\names; DestDir: {app}\modules\{#DIFFEQU}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#DIFFEQU}\macros\*.bin; DestDir: {app}\modules\{#DIFFEQU}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#DIFFEQU}\macros\*.sci; DestDir: {app}\modules\{#DIFFEQU}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#DIFFEQU}\macros\buildmacros.sce; DestDir: {app}\modules\{#DIFFEQU}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#DIFFEQU}\macros\buildmacros.bat; DestDir: {app}\modules\{#DIFFEQU}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#DIFFEQU}\macros\cleanmacros.bat; DestDir: {app}\modules\{#DIFFEQU}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#DIFFEQU}\demos\*.*; DestDir: {app}\modules\{#DIFFEQU}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#DIFFEQU}\tests\*.*; DestDir: {app}\modules\{#DIFFEQU}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
