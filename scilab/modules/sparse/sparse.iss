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
; sparse module
;--------------------------------------------------------------------------------------------------------------
;
#define SPARSE "sparse"
;
Source: bin\{#SPARSE}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SPARSE}
Source: bin\{#SPARSE}.dll; DestDir: {app}\bin; Components: {#COMPN_SPARSE}
;
Source: modules\{#SPARSE}\VERSION.xml; DestDir: {app}\modules\{#SPARSE}; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\readme.txt; DestDir: {app}\modules\{#SPARSE}; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\license.txt; DestDir: {app}\modules\{#SPARSE}; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\changelog.txt; DestDir: {app}\modules\{#SPARSE}; Components: {#COMPN_SPARSE}
;
Source: modules\{#SPARSE}\sci_gateway\{#SPARSE}_gateway.xml; DestDir: {app}\modules\{#SPARSE}\sci_gateway; Components: {#COMPN_SPARSE}
;
Source: modules\{#SPARSE}\etc\{#SPARSE}.quit; DestDir: {app}\modules\{#SPARSE}\etc; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\etc\{#SPARSE}.start; DestDir: {app}\modules\{#SPARSE}\etc; Components: {#COMPN_SPARSE}
;
;
;Source: modules\{#SPARSE}\includes\*.h; DestDir: {app}\modules\{#SPARSE}\includes; Components: {#COMPN_SPARSE}
;
Source: modules\{#SPARSE}\macros\buildmacros.sce; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\macros\buildmacros.bat; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\macros\lib; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\macros\names; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\macros\*.bin; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SPARSE}
Source: modules\{#SPARSE}\macros\*.sci; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SPARSE}
;
;Source: modules\{#SPARSE}\demos\*.*; DestDir: {app}\modules\{#SPARSE}\demos; Flags: recursesubdirs;  Components: {#COMPN_SPARSE}
;
Source: modules\{#SPARSE}\tests\*.*; DestDir: {app}\modules\{#SPARSE}\tests; Flags: recursesubdirs; Components: {#COMPN_SPARSE} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
