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
Source: modules\{#SLINT}\etc\cnes_tool_conf.xml; DestDir: {app}\modules\{#SLINT}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#SLINT}\etc\cnes_analysis_conf.xml; DestDir: {app}\modules\{#SLINT}\etc; Components: {#COMPN_SCILAB}
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
