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
; polynomials module
;--------------------------------------------------------------------------------------------------------------
;
#define POLYNOMIALS "polynomials"
;
Source: bin\{#POLYNOMIALS}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#POLYNOMIALS}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#POLYNOMIALS}\license.txt; DestDir: {app}\modules\{#POLYNOMIALS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#POLYNOMIALS}\sci_gateway\{#POLYNOMIALS}_gateway.xml; DestDir: {app}\modules\{#POLYNOMIALS}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#POLYNOMIALS}\etc\{#POLYNOMIALS}.quit; DestDir: {app}\modules\{#POLYNOMIALS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#POLYNOMIALS}\etc\{#POLYNOMIALS}.start; DestDir: {app}\modules\{#POLYNOMIALS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#POLYNOMIALS}\macros\buildmacros.sce; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#POLYNOMIALS}\macros\buildmacros.bat; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#POLYNOMIALS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#POLYNOMIALS}\macros\lib; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#POLYNOMIALS}\macros\*.sci; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#POLYNOMIALS}\macros\*.bin; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#POLYNOMIALS}\demos\*.*; DestDir: {app}\modules\{#POLYNOMIALS}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#POLYNOMIALS}\tests\*.*; DestDir: {app}\modules\{#POLYNOMIALS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
