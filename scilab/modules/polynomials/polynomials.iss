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
; polynomials module
;--------------------------------------------------------------------------------------------------------------
;
#define POLYNOMIALS "polynomials"
;
Source: bin\{#POLYNOMIALS}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#POLYNOMIALS}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#POLYNOMIALS}\license.txt; DestDir: {app}\modules\{#POLYNOMIALS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#POLYNOMIALS}\sci_gateway\{#POLYNOMIALS}_gateway.xml; DestDir: {app}\modules\{#POLYNOMIALS}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#POLYNOMIALS}\etc\{#POLYNOMIALS}.quit; DestDir: {app}\modules\{#POLYNOMIALS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#POLYNOMIALS}\etc\{#POLYNOMIALS}.start; DestDir: {app}\modules\{#POLYNOMIALS}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#POLYNOMIALS}\includes\*.h; DestDir: {app}\modules\{#POLYNOMIALS}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#POLYNOMIALS}\macros\buildmacros.sce; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#POLYNOMIALS}\macros\buildmacros.bat; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#POLYNOMIALS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#POLYNOMIALS}\macros\lib; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#POLYNOMIALS}\macros\names; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#POLYNOMIALS}\macros\*.bin; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#POLYNOMIALS}\macros\*.sci; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#POLYNOMIALS}\demos\*.*; DestDir: {app}\modules\{#POLYNOMIALS}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#POLYNOMIALS}\tests\*.*; DestDir: {app}\modules\{#POLYNOMIALS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
