;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2015 - Antoine ELIAS
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
; optimization module
;--------------------------------------------------------------------------------------------------------------
;
#define SLINT "slint"
;
Source: bin\{#SLINT}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#SLINT}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#SLINT}\sci_gateway\{#SLINT}_gateway.xml; DestDir: {app}\modules\{#SLINT}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#SLINT}\etc\{#SLINT}.quit; DestDir: {app}\modules\{#SLINT}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#SLINT}\etc\{#SLINT}.start; DestDir: {app}\modules\{#SLINT}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#SLINT}\etc\{#SLINT}.xml; DestDir: {app}\modules\{#SLINT}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#SLINT}\macros\buildmacros.sce; DestDir: {app}\modules\{#SLINT}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SLINT}\macros\buildmacros.bat; DestDir: {app}\modules\{#SLINT}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SLINT}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SLINT}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#SLINT}\macros\lib; DestDir: {app}\modules\{#SLINT}\macros;Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;Source: modules\{#SLINT}\macros\*.sci; DestDir: {app}\modules\{#SLINT}\macros;Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;Source: modules\{#SLINT}\macros\*.bin; DestDir: {app}\modules\{#SLINT}\macros;Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;Source: modules\{#SLINT}\demos\*.*; DestDir: {app}\modules\{#SLINT}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#SLINT}\tests\*.*; DestDir: {app}\modules\{#SLINT}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB}  and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
