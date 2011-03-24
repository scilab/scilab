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
; UMFPACK module
;--------------------------------------------------------------------------------------------------------------
;
#define UMFPACK "umfpack"
;
Source: bin\no{#UMFPACK}.dll; DestDir: {app}\bin; DestName: {#UMFPACK}.dll; Components: not ({#COMPN_UMFPACK})
Source: bin\lib{#UMFPACK}.dll; DestDir: {app}\bin; Components: {#COMPN_UMFPACK}
Source: bin\{#UMFPACK}.dll; DestDir: {app}\bin; Components: {#COMPN_UMFPACK}
;
Source: modules\{#UMFPACK}\VERSION.xml; DestDir: {app}\modules\{#UMFPACK}; Components: {#COMPN_UMFPACK}
Source: modules\{#UMFPACK}\license.txt; DestDir: {app}\modules\{#UMFPACK}; Components: {#COMPN_UMFPACK}
;
Source: modules\{#UMFPACK}\sci_gateway\{#UMFPACK}_gateway.xml; DestDir: {app}\modules\{#UMFPACK}\sci_gateway; Components: {#COMPN_UMFPACK}
;
Source: modules\{#UMFPACK}\etc\{#UMFPACK}.quit; DestDir: {app}\modules\{#UMFPACK}\etc; Components: {#COMPN_UMFPACK}
Source: modules\{#UMFPACK}\etc\{#UMFPACK}.start; DestDir: {app}\modules\{#UMFPACK}\etc; Components: {#COMPN_UMFPACK}
;
;Source: modules\{#UMFPACK}\includes\*.h; DestDir: {app}\modules\{#UMFPACK}\includes; Components: {#COMPN_UMFPACK}
;
Source: modules\{#UMFPACK}\macros\buildmacros.sce; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_UMFPACK}
Source: modules\{#UMFPACK}\macros\buildmacros.bat; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_UMFPACK}
Source: modules\{#UMFPACK}\macros\cleanmacros.bat; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_UMFPACK}
Source: modules\{#UMFPACK}\macros\lib; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_UMFPACK}
Source: modules\{#UMFPACK}\macros\names; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_UMFPACK}
Source: modules\{#UMFPACK}\macros\*.bin; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_UMFPACK}
Source: modules\{#UMFPACK}\macros\*.sci; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_UMFPACK}
;
Source: modules\{#UMFPACK}\examples\*.*; DestDir: {app}\modules\{#UMFPACK}\examples; Flags: recursesubdirs;  Components: {#COMPN_UMFPACK}
;
;Source: modules\{#UMFPACK}\demos\*.*; DestDir: {app}\modules\{#UMFPACK}\demos; Flags: recursesubdirs;  Components: {#COMPN_UMFPACK}
;
Source: modules\{#UMFPACK}\tests\*.*; DestDir: {app}\modules\{#UMFPACK}\tests; Flags: recursesubdirs; Components: {#COMPN_UMFPACK} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
