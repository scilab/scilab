;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2015 - Antoine ELIAS
;
; Copyright (C) 2012 - 2016 - Scilab Enterprises
;
; This file is hereby licensed under the terms of the GNU GPL v2.0,
; pursuant to article 5.3.4 of the CeCILL v.2.1.
; This file was originally licensed under the terms of the CeCILL v2.1,
; and continues to be available under such terms.
; For more information, see the COPYING file which you should have received
; along with this program.
;
;--------------------------------------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;--------------------------------------------------------------------------------------------------------------
; coverage module
;--------------------------------------------------------------------------------------------------------------
;
#define COVERAGE "coverage"
;
Source: bin\{#COVERAGE}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#COVERAGE}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#COVERAGE}\data\*.*; DestDir: {app}\modules\{#COVERAGE}\data; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#COVERAGE}\etc\{#COVERAGE}.quit; DestDir: {app}\modules\{#COVERAGE}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#COVERAGE}\etc\{#COVERAGE}.start; DestDir: {app}\modules\{#COVERAGE}\etc; Components: {#COMPN_SCILAB}
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
;Source: modules\{#COVERAGE}\tests\*.*; DestDir: {app}\modules\{#COVERAGE}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB}  and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
