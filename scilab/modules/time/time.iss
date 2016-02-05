;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) DIGITEO - 2010 - Allan CORNET
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
; time module
;--------------------------------------------------------------------------------------------------------------
;
#define TIME "time"
;
Source: bin\{#TIME}_gw.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#TIME}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#TIME}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#TIME}\license.txt; DestDir: {app}\modules\{#TIME}; Components: {#COMPN_SCILAB}
;
;Source: modules\{#TIME}\includes\*.h; DestDir: {app}\modules\{#TIME}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\includes\realtime.h; DestDir: {app}\modules\{#TIME}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\includes\timer.h; DestDir: {app}\modules\{#TIME}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\includes\dynlib_time.h; DestDir: {app}\modules\{#TIME}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#TIME}\sci_gateway\{#TIME}_gateway.xml; DestDir: {app}\modules\{#TIME}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#TIME}\etc\{#TIME}.quit; DestDir: {app}\modules\{#TIME}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\etc\{#TIME}.start; DestDir: {app}\modules\{#TIME}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#TIME}\macros\buildmacros.sce; DestDir: {app}\modules\{#TIME}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\macros\buildmacros.bat; DestDir: {app}\modules\{#TIME}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\macros\cleanmacros.bat; DestDir: {app}\modules\{#TIME}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\macros\*.bin; DestDir: {app}\modules\{#TIME}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\macros\*.sci; DestDir: {app}\modules\{#TIME}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TIME}\macros\lib; DestDir: {app}\modules\{#TIME}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#TIME}\macros\names; DestDir: {app}\modules\{#TIME}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#TIME}\tests\*.*; DestDir: {app}\modules\{#TIME}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
