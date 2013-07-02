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
; mexlib module
;--------------------------------------------------------------------------------------------------------------
;
#define MEXLIB "mexlib"
;
Source: bin\libmx.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libmx.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libmex.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libmex.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libmat.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libmat.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#MEXLIB}\license.txt; DestDir: {app}\modules\{#MEXLIB}; Components: {#COMPN_SCILAB}
;
Source: modules\{#MEXLIB}\etc\{#MEXLIB}.quit; DestDir: {app}\modules\{#MEXLIB}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#MEXLIB}\etc\{#MEXLIB}.start; DestDir: {app}\modules\{#MEXLIB}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#MEXLIB}\includes\*.h; DestDir: {app}\modules\{#MEXLIB}\includes; Components: {#COMPN_SCILAB}
;
;Source: modules\{#MEXLIB}\macros\lib; DestDir: {app}\modules\{#MEXLIB}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#MEXLIB}\macros\names; DestDir: {app}\modules\{#MEXLIB}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#MEXLIB}\macros\*.bin; DestDir: {app}\modules\{#MEXLIB}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#MEXLIB}\macros\*.sci; DestDir: {app}\modules\{#MEXLIB}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MEXLIB}\macros\buildmacros.sce; DestDir: {app}\modules\{#MEXLIB}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MEXLIB}\macros\buildmacros.bat; DestDir: {app}\modules\{#MEXLIB}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MEXLIB}\macros\cleanmacros.bat; DestDir: {app}\modules\{#MEXLIB}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#MEXLIB}\demos\*.*; DestDir: {app}\modules\{#MEXLIB}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#MEXLIB}\examples\*.*; DestDir: {app}\modules\{#MEXLIB}\examples; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#MEXLIB}\tests\*.*; DestDir: {app}\modules\{#MEXLIB}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
