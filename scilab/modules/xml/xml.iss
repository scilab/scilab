;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2011 - Allan CORNET
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
; xml module
;--------------------------------------------------------------------------------------------------------------
;
#define XML "xml"
;
Source: bin\{#xml}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#xml}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#xml}\license.txt; DestDir: {app}\modules\{#xml}; Components: {#COMPN_SCILAB}
;
Source: modules\{#xml}\sci_gateway\{#xml}_gateway.xml; DestDir: {app}\modules\{#xml}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#xml}\etc\{#xml}.quit; DestDir: {app}\modules\{#xml}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#xml}\etc\{#xml}.start; DestDir: {app}\modules\{#xml}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#xml}\macros\buildmacros.sce; DestDir: {app}\modules\{#xml}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#xml}\macros\buildmacros.bat; DestDir: {app}\modules\{#xml}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#xml}\macros\cleanmacros.bat; DestDir: {app}\modules\{#xml}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#xml}\macros\*.bin; DestDir: {app}\modules\{#xml}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#xml}\macros\*.sci; DestDir: {app}\modules\{#xml}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#xml}\macros\lib; DestDir: {app}\modules\{#xml}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#xml}\macros\names; DestDir: {app}\modules\{#xml}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#xml}\tests\*.*; DestDir: {app}\modules\{#xml}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
