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
; linear_algebra module
;--------------------------------------------------------------------------------------------------------------
;
#define LINEAR_ALGEBRA "linear_algebra"
;
Source: bin\{#LINEAR_ALGEBRA}.dll; DestDir: {app}\bin; Components: {#COMPN_LINALG}
Source: bin\{#LINEAR_ALGEBRA}_f.dll; DestDir: {app}\bin; Components: {#COMPN_LINALG}
;
Source: modules\{#LINEAR_ALGEBRA}\VERSION.xml; DestDir: {app}\modules\{#LINEAR_ALGEBRA}; Components: {#COMPN_LINALG}
Source: modules\{#LINEAR_ALGEBRA}\license.txt; DestDir: {app}\modules\{#LINEAR_ALGEBRA}; Components: {#COMPN_LINALG}
;
Source: modules\{#LINEAR_ALGEBRA}\sci_gateway\{#LINEAR_ALGEBRA}_gateway.xml; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\sci_gateway; Components: {#COMPN_LINALG}
;
Source: modules\{#LINEAR_ALGEBRA}\etc\{#LINEAR_ALGEBRA}.quit; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\etc; Components: {#COMPN_LINALG}
Source: modules\{#LINEAR_ALGEBRA}\etc\{#LINEAR_ALGEBRA}.start; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\etc; Components: {#COMPN_LINALG}
;
;Source: modules\{#LINEAR_ALGEBRA}\includes\*.h; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\includes; Components: {#COMPN_LINALG}
;
Source: modules\{#LINEAR_ALGEBRA}\macros\buildmacros.sce; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\macros; Components: {#COMPN_LINALG}
Source: modules\{#LINEAR_ALGEBRA}\macros\buildmacros.bat; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\macros; Components: {#COMPN_LINALG}
Source: modules\{#LINEAR_ALGEBRA}\macros\cleanmacros.bat; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\macros; Components: {#COMPN_LINALG}
Source: modules\{#LINEAR_ALGEBRA}\macros\lib; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\macros; Components: {#COMPN_LINALG}
Source: modules\{#LINEAR_ALGEBRA}\macros\names; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\macros; Components: {#COMPN_LINALG}
Source: modules\{#LINEAR_ALGEBRA}\macros\*.bin; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\macros; Components: {#COMPN_LINALG}
Source: modules\{#LINEAR_ALGEBRA}\macros\*.sci; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\macros; Components: {#COMPN_LINALG}
;
;Source: modules\{#LINEAR_ALGEBRA}\demos\*.*; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\demos; Flags: recursesubdirs;  Components: {#COMPN_LINALG}
;
Source: modules\{#LINEAR_ALGEBRA}\examples\*.*; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\examples; Flags: recursesubdirs;  Components: {#COMPN_LINALG}
;
Source: modules\{#LINEAR_ALGEBRA}\tests\*.*; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\tests; Flags: recursesubdirs; Components: {#COMPN_LINALG} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
