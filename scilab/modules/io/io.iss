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
; io module
;--------------------------------------------------------------------------------------------------------------
;
#define IO "io"
;
Source: bin\{#IO}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#IO}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#IO}\license.txt; DestDir: {app}\modules\{#IO}; Components: {#COMPN_SCILAB}
;
Source: modules\{#IO}\sci_gateway\{#IO}_gateway.xml; DestDir: {app}\modules\{#IO}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#IO}\etc\{#IO}.quit; DestDir: {app}\modules\{#IO}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\etc\{#IO}.start; DestDir: {app}\modules\{#IO}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#IO}\includes\*.h; DestDir: {app}\modules\{#IO}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#IO}\macros\buildmacros.sce; DestDir: {app}\modules\{#IO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\macros\buildmacros.bat; DestDir: {app}\modules\{#IO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\macros\cleanmacros.bat; DestDir: {app}\modules\{#IO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\macros\lib; DestDir: {app}\modules\{#IO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\macros\names; DestDir: {app}\modules\{#IO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\macros\*.sci; DestDir: {app}\modules\{#IO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\macros\*.bin; DestDir: {app}\modules\{#IO}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#IO}\demos\*.*; DestDir: {app}\modules\{#IO}\demos; Flags: recursesubdirs;  Components: {#COMPN_SCILAB}
;
Source: modules\{#IO}\tests\*.*; DestDir: {app}\modules\{#IO}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
