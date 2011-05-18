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
; overloading module
;--------------------------------------------------------------------------------------------------------------
;
#define OVERLOADING "overloading"
;
Source: modules\{#OVERLOADING}\license.txt; DestDir: {app}\modules\{#OVERLOADING}; Components: {#COMPN_SCILAB}
;
Source: modules\{#OVERLOADING}\etc\{#OVERLOADING}.quit; DestDir: {app}\modules\{#OVERLOADING}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\etc\{#OVERLOADING}.start; DestDir: {app}\modules\{#OVERLOADING}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#OVERLOADING}\includes\*.h; DestDir: {app}\modules\{#OVERLOADING}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#OVERLOADING}\macros\buildmacros.sce; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\buildmacros.bat; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\cleanmacros.bat; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\lib; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\names; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\*.bin; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OVERLOADING}\macros\*.sci; DestDir: {app}\modules\{#OVERLOADING}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#OVERLOADING}\demos\*.*; DestDir: {app}\modules\{#OVERLOADING}\demos; Flags: recursesubdirs;  Components: {#COMPN_SCILAB}
;
Source: modules\{#OVERLOADING}\tests\*.*; DestDir: {app}\modules\{#OVERLOADING}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
