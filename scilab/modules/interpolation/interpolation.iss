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
; interpolation module
;--------------------------------------------------------------------------------------------------------------
;
#define INTERPOLATION "interpolation"
;
Source: bin\{#INTERPOLATION}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#INTERPOLATION}_f.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#INTERPOLATION}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTERPOLATION}\license.txt; DestDir: {app}\modules\{#INTERPOLATION}; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTERPOLATION}\sci_gateway\{#INTERPOLATION}_gateway.xml; DestDir: {app}\modules\{#INTERPOLATION}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTERPOLATION}\etc\{#INTERPOLATION}.quit; DestDir: {app}\modules\{#INTERPOLATION}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#INTERPOLATION}\etc\{#INTERPOLATION}.start; DestDir: {app}\modules\{#INTERPOLATION}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTERPOLATION}\macros\lib; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#INTERPOLATION}\macros\*.sci; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#INTERPOLATION}\macros\*.bin; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#INTERPOLATION}\macros\buildmacros.sce; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#INTERPOLATION}\macros\buildmacros.bat; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#INTERPOLATION}\macros\cleanmacros.bat; DestDir: {app}\modules\{#INTERPOLATION}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTERPOLATION}\demos\*.*; DestDir: {app}\modules\{#INTERPOLATION}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#INTERPOLATION}\tests\*.*; DestDir: {app}\modules\{#INTERPOLATION}\tests; Flags: recursesubdirs; Components:{#COMPN_SCILAB} and {#COMPN_TESTS}
;--------------------------------------------------------------------------------------------------------------
