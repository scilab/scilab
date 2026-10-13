;
; Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2026 - Dassault Systèmes - Clément DAVID
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
; fmu module
;--------------------------------------------------------------------------------------------------------------
;
#define FMU "fmu"
;
Source: bin\{#FMU}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#FMU}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#FMU}\license.txt; DestDir: {app}\modules\{#FMU}; Components: {#COMPN_SCILAB}
;
Source: modules\{#FMU}\includes\fmiModelFunctions.h; DestDir: {app}\modules\{#FMU}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#FMU}\includes\fmiModelTypes.h; DestDir: {app}\modules\{#FMU}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#FMU}\sci_gateway\{#FMU}_gateway.xml; DestDir: {app}\modules\{#FMU}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#FMU}\src\swig\fmuswig_gateway.xml; DestDir: {app}\modules\{#FMU}\src\swig; Components: {#COMPN_SCILAB}
;
Source: modules\{#FMU}\etc\{#FMU}.quit; DestDir: {app}\modules\{#FMU}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#FMU}\etc\{#FMU}.start; DestDir: {app}\modules\{#FMU}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#FMU}\macros\buildmacros.sce; DestDir: {app}\modules\{#FMU}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FMU}\macros\buildmacros.bat; DestDir: {app}\modules\{#FMU}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FMU}\macros\*.bin; DestDir: {app}\modules\{#FMU}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FMU}\macros\*.sci; DestDir: {app}\modules\{#FMU}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FMU}\macros\lib; DestDir: {app}\modules\{#FMU}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#FMU}\tests\*.*; DestDir: {app}\modules\{#FMU}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
Source: modules\{#FMU}\demos\*.*; DestDir: {app}\modules\{#FMU}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#FMU}\help\*.*; DestDir: {app}\modules\{#FMU}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;--------------------------------------------------------------------------------------------------------------
