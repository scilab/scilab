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
; spreadsheet module
;--------------------------------------------------------------------------------------------------------------
;
#define SPREADSHEET "spreadsheet"
;
Source: bin\{#SPREADSHEET}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPREADSHEET}\license.txt; DestDir: {app}\modules\{#SPREADSHEET}; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPREADSHEET}\sci_gateway\{#SPREADSHEET}_gateway.xml; DestDir: {app}\modules\{#SPREADSHEET}\sci_gateway; Components: {#COMPN_SCILAB}
;
;Source: modules\{#SPREADSHEET}\includes\*.h; DestDir: {app}\modules\{#SPREADSHEET}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPREADSHEET}\etc\{#SPREADSHEET}.quit; DestDir: {app}\modules\{#SPREADSHEET}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#SPREADSHEET}\etc\{#SPREADSHEET}.start; DestDir: {app}\modules\{#SPREADSHEET}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPREADSHEET}\macros\buildmacros.sce; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SPREADSHEET}\macros\buildmacros.bat; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SPREADSHEET}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SPREADSHEET}\macros\*.bin; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SPREADSHEET}\macros\*.sci; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SPREADSHEET}\macros\lib; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SPREADSHEET}\macros\names; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPREADSHEET}\demos\*.*; DestDir: {app}\modules\{#SPREADSHEET}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPREADSHEET}\tests\*.*; DestDir: {app}\modules\{#SPREADSHEET}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
