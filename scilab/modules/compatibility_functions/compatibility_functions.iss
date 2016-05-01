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
; compatibility_functions module
;--------------------------------------------------------------------------------------------------------------
;
#define COMPATFUNCTIONS "compatibility_functions"
;
Source: modules\{#COMPATFUNCTIONS}\license.txt; DestDir: {app}\modules\{#COMPATFUNCTIONS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMPATFUNCTIONS}\etc\{#COMPATFUNCTIONS}.quit; DestDir: {app}\modules\{#COMPATFUNCTIONS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#COMPATFUNCTIONS}\etc\{#COMPATFUNCTIONS}.start; DestDir: {app}\modules\{#COMPATFUNCTIONS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMPATFUNCTIONS}\macros\lib; DestDir: {app}\modules\{#COMPATFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#COMPATFUNCTIONS}\macros\*.sci; DestDir: {app}\modules\{#COMPATFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#COMPATFUNCTIONS}\macros\*.bin; DestDir: {app}\modules\{#COMPATFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#COMPATFUNCTIONS}\macros\buildmacros.sce; DestDir: {app}\modules\{#COMPATFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#COMPATFUNCTIONS}\macros\buildmacros.bat; DestDir: {app}\modules\{#COMPATFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#COMPATFUNCTIONS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#COMPATFUNCTIONS}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMPATFUNCTIONS}\tests\*.*; DestDir: {app}\modules\{#COMPATFUNCTIONS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;--------------------------------------------------------------------------------------------------------------
