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
#define COVERAGE "coverage"
;
Source: bin\{#COVERAGE}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#COVERAGE}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#COVERAGE}\data\*.*; DestDir: {app}\modules\{#COVERAGE}\data; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#COVERAGE}\sci_gateway\{#COVERAGE}_gateway.xml; DestDir: {app}\modules\{#COVERAGE}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#COVERAGE}\etc\{#COVERAGE}.quit; DestDir: {app}\modules\{#COVERAGE}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#COVERAGE}\etc\{#COVERAGE}.start; DestDir: {app}\modules\{#COVERAGE}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#COVERAGE}\etc\{#COVERAGE}.xml; DestDir: {app}\modules\{#COVERAGE}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#COVERAGE}\macros\buildmacros.sce; DestDir: {app}\modules\{#COVERAGE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#COVERAGE}\macros\buildmacros.bat; DestDir: {app}\modules\{#COVERAGE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#COVERAGE}\macros\cleanmacros.bat; DestDir: {app}\modules\{#COVERAGE}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#COVERAGE}\macros\lib; DestDir: {app}\modules\{#COVERAGE}\macros;Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;Source: modules\{#COVERAGE}\macros\*.sci; DestDir: {app}\modules\{#COVERAGE}\macros;Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;Source: modules\{#COVERAGE}\macros\*.bin; DestDir: {app}\modules\{#COVERAGE}\macros;Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;Source: modules\{#COVERAGE}\demos\*.*; DestDir: {app}\modules\{#COVERAGE}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#COVERAGE}\tests\*.*; DestDir: {app}\modules\{#COVERAGE}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB}  and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
