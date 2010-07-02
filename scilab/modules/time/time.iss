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
; time module
;--------------------------------------------------------------------------------------------------------------
;
#define TIME "time"
;
Source: bin\{#TIME}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#TIME}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#TIME}\changelog.txt; DestDir: {app}\modules\{#TIME}; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\license.txt; DestDir: {app}\modules\{#TIME}; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\readme.txt; DestDir: {app}\modules\{#TIME}; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\VERSION.xml; DestDir: {app}\modules\{#TIME}; Components: {#COMPN_SCILAB}
;
;Source: modules\{#TIME}\includes\*.h; DestDir: {app}\modules\{#TIME}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\includes\realtime.h; DestDir: {app}\modules\{#TIME}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\includes\timer.h; DestDir: {app}\modules\{#TIME}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\includes\dynlib_time.h; DestDir: {app}\modules\{#TIME}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#TIME}\sci_gateway\{#TIME}_gateway.xml; DestDir: {app}\modules\{#TIME}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#TIME}\etc\{#TIME}.quit; DestDir: {app}\modules\{#TIME}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\etc\{#TIME}.start; DestDir: {app}\modules\{#TIME}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#TIME}\macros\buildmacros.sce; DestDir: {app}\modules\{#TIME}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\macros\buildmacros.bat; DestDir: {app}\modules\{#TIME}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\macros\cleanmacros.bat; DestDir: {app}\modules\{#TIME}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\macros\*.bin; DestDir: {app}\modules\{#TIME}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\macros\*.sci; DestDir: {app}\modules\{#TIME}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\macros\lib; DestDir: {app}\modules\{#TIME}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\macros\names; DestDir: {app}\modules\{#TIME}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#TIME}\tests\*.*; DestDir: {app}\modules\{#TIME}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
