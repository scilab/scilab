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
; parallel module
;--------------------------------------------------------------------------------------------------------------
;
#define PARALLEL "parallel"
;
Source: bin\{#PARALLEL}.dll; DestDir: {app}\bin; Components: {#COMPN_PARALLEL}
;
;Source: modules\{#PARALLEL}\changelog.txt; DestDir: {app}\modules\{#PARALLEL}; Components: {#COMPN_PARALLEL};
Source: modules\{#PARALLEL}\license.txt; DestDir: {app}\modules\{#PARALLEL}; Components: {#COMPN_PARALLEL}
Source: modules\{#PARALLEL}\readme.txt; DestDir: {app}\modules\{#PARALLEL}; Components: {#COMPN_PARALLEL}
Source: modules\{#PARALLEL}\version.xml; DestDir: {app}\modules\{#PARALLEL}; Components: {#COMPN_PARALLEL}
;
Source: modules\{#PARALLEL}\sci_gateway\{#PARALLEL}_gateway.xml; DestDir: {app}\modules\{#PARALLEL}\sci_gateway; Components: {#COMPN_PARALLEL}
;
;Source: modules\{#PARALLEL}\includes\*.h; DestDir: {app}\modules\{#PARALLEL}\includes; Components: {#COMPN_PARALLEL}
;
Source: modules\{#PARALLEL}\etc\{#PARALLEL}.quit; DestDir: {app}\modules\{#PARALLEL}\etc; Components: {#COMPN_PARALLEL}
Source: modules\{#PARALLEL}\etc\{#PARALLEL}.start; DestDir: {app}\modules\{#PARALLEL}\etc; Components: {#COMPN_PARALLEL}
;
;
Source: modules\{#PARALLEL}\macros\buildmacros.sce; DestDir: {app}\modules\{#PARALLEL}\macros; Components: {#COMPN_PARALLEL}
Source: modules\{#PARALLEL}\macros\buildmacros.bat; DestDir: {app}\modules\{#PARALLEL}\macros; Components: {#COMPN_PARALLEL}
Source: modules\{#PARALLEL}\macros\cleanmacros.bat; DestDir: {app}\modules\{#PARALLEL}\macros; Components: {#COMPN_PARALLEL}
;Source: modules\{#PARALLEL}\macros\*.bin; DestDir: {app}\modules\{#PARALLEL}\macros; Components: {#COMPN_PARALLEL}
;Source: modules\{#PARALLEL}\macros\*.sci; DestDir: {app}\modules\{#PARALLEL}\macros; Components: {#COMPN_PARALLEL}
;Source: modules\{#PARALLEL}\macros\lib; DestDir: {app}\modules\{#PARALLEL}\macros; Components: {#COMPN_PARALLEL}
;Source: modules\{#PARALLEL}\macros\names; DestDir: {app}\modules\{#PARALLEL}\macros; Components: {#COMPN_PARALLEL}
;
;Source: modules\{#PARALLEL}\demos\*.*; DestDir: {app}\modules\{#PARALLEL}\demos; Flags: recursesubdirs; Components: {#COMPN_PARALLEL}
;
Source: modules\{#PARALLEL}\tests\*.*; DestDir: {app}\modules\{#PARALLEL}\tests; Flags: recursesubdirs; Components: {#COMPN_TESTS} and {#COMPN_PARALLEL}
;--------------------------------------------------------------------------------------------------------------
