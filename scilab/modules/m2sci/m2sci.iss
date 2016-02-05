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
; m2sci module
;--------------------------------------------------------------------------------------------------------------
;
#define M2SCI "m2sci"
;
Source: modules\{#M2SCI}\license.txt; DestDir: {app}\modules\{#M2SCI}; Components: {#COMPN_SCILAB}
;
Source: modules\{#M2SCI}\etc\{#M2SCI}.quit; DestDir: {app}\modules\{#M2SCI}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#M2SCI}\etc\{#M2SCI}.start; DestDir: {app}\modules\{#M2SCI}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#M2SCI}\macros\lib; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#M2SCI}\macros\*.sci; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#M2SCI}\macros\*.bin; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#M2SCI}\macros\buildmacros.sce; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#M2SCI}\macros\buildmacros.bat; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#M2SCI}\macros\cleanmacros.bat; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#M2SCI}\tests\*.*; DestDir: {app}\modules\{#M2SCI}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
