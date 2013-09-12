;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) DIGITEO - 2010 - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
;
;--------------------------------------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;--------------------------------------------------------------------------------------------------------------
; boolean module
;--------------------------------------------------------------------------------------------------------------
;
#define BOOLEAN "boolean"
;
Source: bin\{#BOOLEAN}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#BOOLEAN}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#BOOLEAN}\license.txt; DestDir: {app}\modules\{#BOOLEAN}; Components: {#COMPN_SCILAB}
;
Source: modules\{#BOOLEAN}\sci_gateway\{#BOOLEAN}_gateway.xml; DestDir: {app}\modules\{#BOOLEAN}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#BOOLEAN}\etc\{#BOOLEAN}.quit; DestDir: {app}\modules\{#BOOLEAN}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#BOOLEAN}\etc\{#BOOLEAN}.start; DestDir: {app}\modules\{#BOOLEAN}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#BOOLEAN}\includes\*.h; DestDir: {app}\modules\{#BOOLEAN}\includes; Components: {#COMPN_SCILAB}
;
;Source: modules\{#BOOLEAN}\macros\lib; DestDir: {app}\modules\{#BOOLEAN}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#BOOLEAN}\macros\names; DestDir: {app}\modules\{#BOOLEAN}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#BOOLEAN}\macros\*.bin; DestDir: {app}\modules\{#BOOLEAN}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#BOOLEAN}\macros\*.sci; DestDir: {app}\modules\{#BOOLEAN}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#BOOLEAN}\macros\buildmacros.sce; DestDir: {app}\modules\{#BOOLEAN}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#BOOLEAN}\macros\buildmacros.bat; DestDir: {app}\modules\{#BOOLEAN}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#BOOLEAN}\macros\cleanmacros.bat; DestDir: {app}\modules\{#BOOLEAN}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#BOOLEAN}\demos\*.*; DestDir: {app}\modules\{#BOOLEAN}\demos; Components: {#COMPN_SCILAB}
;
Source: modules\{#BOOLEAN}\tests\*.*; DestDir: {app}\modules\{#BOOLEAN}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
