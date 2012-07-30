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
; integer module
;--------------------------------------------------------------------------------------------------------------
;
#define INTEGER "integer"
;
Source: bin\{#INTEGER}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#INTEGER}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#INTEGER}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTEGER}\license.txt; DestDir: {app}\modules\{#INTEGER}; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTEGER}\sci_gateway\{#INTEGER}_gateway.xml; DestDir: {app}\modules\{#INTEGER}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTEGER}\etc\{#INTEGER}.quit; DestDir: {app}\modules\{#INTEGER}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#INTEGER}\etc\{#INTEGER}.start; DestDir: {app}\modules\{#INTEGER}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#INTEGER}\includes\*.h; DestDir: {app}\modules\{#INTEGER}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTEGER}\macros\buildmacros.sce; DestDir: {app}\modules\{#INTEGER}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#INTEGER}\macros\buildmacros.bat; DestDir: {app}\modules\{#INTEGER}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#INTEGER}\macros\cleanmacros.bat; DestDir: {app}\modules\{#INTEGER}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#INTEGER}\macros\lib; DestDir: {app}\modules\{#INTEGER}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#INTEGER}\macros\names; DestDir: {app}\modules\{#INTEGER}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#INTEGER}\macros\*.bin; DestDir: {app}\modules\{#INTEGER}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#INTEGER}\macros\*.sci; DestDir: {app}\modules\{#INTEGER}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#INTEGER}\demos\*.*; DestDir: {app}\modules\{#INTEGER}\demos; Flags: recursesubdirs;  Components: {#COMPN_SCILAB}
;
Source: modules\{#INTEGER}\tests\*.*; DestDir: {app}\modules\{#INTEGER}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;
;--------------------------------------------------------------------------------------------------------------
