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
; compatibility_functions module
;--------------------------------------------------------------------------------------------------------------
;
#define COMPATFUNCTIONS "compatibility_functions"
;
Source: modules\{#COMPATFUNCTIONS}\VERSION.xml; DestDir: {app}\modules\{#COMPATFUNCTIONS}; Components: {#COMPN_COMPATFUNCTIONS}
Source: modules\{#COMPATFUNCTIONS}\readme.txt; DestDir: {app}\modules\{#COMPATFUNCTIONS}; Components: {#COMPN_COMPATFUNCTIONS}
Source: modules\{#COMPATFUNCTIONS}\license.txt; DestDir: {app}\modules\{#COMPATFUNCTIONS}; Components: {#COMPN_COMPATFUNCTIONS}
Source: modules\{#COMPATFUNCTIONS}\changelog.txt; DestDir: {app}\modules\{#COMPATFUNCTIONS}; Components: {#COMPN_COMPATFUNCTIONS}
;
Source: modules\{#COMPATFUNCTIONS}\etc\{#COMPATFUNCTIONS}.quit; DestDir: {app}\modules\{#COMPATFUNCTIONS}\etc; Components: {#COMPN_COMPATFUNCTIONS}
Source: modules\{#COMPATFUNCTIONS}\etc\{#COMPATFUNCTIONS}.start; DestDir: {app}\modules\{#COMPATFUNCTIONS}\etc; Components: {#COMPN_COMPATFUNCTIONS}
;
;Source: modules\{#COMPATFUNCTIONS}\includes\*.h; DestDir: {app}\modules\{#COMPATFUNCTIONS}\includes; Components: {#COMPN_COMPATFUNCTIONS}
;
Source: modules\{#COMPATFUNCTIONS}\macros\lib; DestDir: {app}\modules\{#COMPATFUNCTIONS}\macros; Components: {#COMPN_COMPATFUNCTIONS}
Source: modules\{#COMPATFUNCTIONS}\macros\names; DestDir: {app}\modules\{#COMPATFUNCTIONS}\macros; Components: {#COMPN_COMPATFUNCTIONS}
Source: modules\{#COMPATFUNCTIONS}\macros\*.bin; DestDir: {app}\modules\{#COMPATFUNCTIONS}\macros; Components: {#COMPN_COMPATFUNCTIONS}
Source: modules\{#COMPATFUNCTIONS}\macros\*.sci; DestDir: {app}\modules\{#COMPATFUNCTIONS}\macros; Components: {#COMPN_COMPATFUNCTIONS}
Source: modules\{#COMPATFUNCTIONS}\macros\buildmacros.sce; DestDir: {app}\modules\{#COMPATFUNCTIONS}\macros; Components: {#COMPN_COMPATFUNCTIONS}
Source: modules\{#COMPATFUNCTIONS}\macros\buildmacros.bat; DestDir: {app}\modules\{#COMPATFUNCTIONS}\macros; Components: {#COMPN_COMPATFUNCTIONS}
Source: modules\{#COMPATFUNCTIONS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#COMPATFUNCTIONS}\macros; Components: {#COMPN_COMPATFUNCTIONS}
;
;Source: modules\{#COMPATFUNCTIONS}\demos\*.*; DestDir: {app}\modules\{#COMPATFUNCTIONS}\demos; Flags: recursesubdirs; Components: {#COMPN_COMPATFUNCTIONS}
;
Source: modules\{#COMPATFUNCTIONS}\tests\*.*; DestDir: {app}\modules\{#COMPATFUNCTIONS}\tests; Flags: recursesubdirs; Components: {#COMPN_COMPATFUNCTIONS} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
