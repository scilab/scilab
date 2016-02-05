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
; tclsci module
;--------------------------------------------------------------------------------------------------------------
;
#define TCLSCI "tclsci"
;
Source: modules\{#TCLSCI}\license.txt; DestDir: {app}\modules\{#TCLSCI}; Components: {#COMPN_SCILAB}
;
Source: modules\{#TCLSCI}\sci_gateway\{#TCLSCI}_gateway.xml; DestDir: {app}\modules\{#TCLSCI}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#TCLSCI}\etc\{#TCLSCI}.quit; DestDir: {app}\modules\{#TCLSCI}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#TCLSCI}\etc\{#TCLSCI}.start; DestDir: {app}\modules\{#TCLSCI}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#TCLSCI}\demos\*.*; DestDir: {app}\modules\{#TCLSCI}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;Source: modules\{#TCLSCI}\includes\*.h; Flags: recursesubdirs; DestDir: {app}\modules\{#TCLSCI}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#TCLSCI}\macros\lib; DestDir: {app}\modules\{#TCLSCI}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TCLSCI}\macros\*.sci; DestDir: {app}\modules\{#TCLSCI}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TCLSCI}\macros\*.bin; DestDir: {app}\modules\{#TCLSCI}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TCLSCI}\macros\buildmacros.sce; DestDir: {app}\modules\{#TCLSCI}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TCLSCI}\macros\buildmacros.bat; DestDir: {app}\modules\{#TCLSCI}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#TCLSCI}\macros\cleanmacros.bat; DestDir: {app}\modules\{#TCLSCI}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#TCLSCI}\tcl\*.*; DestDir: {app}\modules\{#TCLSCI}\tcl; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: bin\tclsci.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\tcl85.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\tk85.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#TCLSCI}\tests\*.*; DestDir: {app}\modules\{#TCLSCI}\tests; Flags: recursesubdirs; Components:  {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
