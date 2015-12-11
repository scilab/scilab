;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2011-2012 - Allan CORNET
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
; xml module
;--------------------------------------------------------------------------------------------------------------
;
#define XML "xml"
;
Source: bin\{#XML}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#XML}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#XML}\license.txt; DestDir: {app}\modules\{#XML}; Components: {#COMPN_SCILAB}
;
Source: modules\{#XML}\sci_gateway\{#XML}_gateway.xml; DestDir: {app}\modules\{#XML}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#XML}\etc\{#XML}.quit; DestDir: {app}\modules\{#XML}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#XML}\etc\{#XML}.start; DestDir: {app}\modules\{#XML}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#XML}\macros\buildmacros.sce; DestDir: {app}\modules\{#xml}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#XML}\macros\buildmacros.bat; DestDir: {app}\modules\{#xml}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#XML}\macros\cleanmacros.bat; DestDir: {app}\modules\{#xml}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#XML}\macros\*.bin; DestDir: {app}\modules\{#XML}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#XML}\macros\*.sci; DestDir: {app}\modules\{#XML}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#XML}\macros\lib; DestDir: {app}\modules\{#XML}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#XML}\macros\names; DestDir: {app}\modules\{#XML}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#XML}\tests\*.*; DestDir: {app}\modules\{#XML}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
Source: modules\{#XML}\demos\*.*; DestDir: {app}\modules\{#XML}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;--------------------------------------------------------------------------------------------------------------
