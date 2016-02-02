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
; sparse module
;--------------------------------------------------------------------------------------------------------------
;
#define SPARSE "sparse"
;
Source: bin\{#SPARSE}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#SPARSE}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#SPARSE}_gw.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPARSE}\license.txt; DestDir: {app}\modules\{#SPARSE}; Components: {#COMPN_SCILAB}
;
;Source: modules\{#SPARSE}\sci_gateway\{#SPARSE}_gateway.xml; DestDir: {app}\modules\{#SPARSE}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPARSE}\etc\{#SPARSE}.quit; DestDir: {app}\modules\{#SPARSE}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#SPARSE}\etc\{#SPARSE}.start; DestDir: {app}\modules\{#SPARSE}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPARSE}\macros\buildmacros.sce; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SPARSE}\macros\buildmacros.bat; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SPARSE}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SPARSE}\macros\lib; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SPARSE}\macros\*.sci; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SPARSE}\macros\*.bin; DestDir: {app}\modules\{#SPARSE}\macros; Components: {#COMPN_SCILAB};

Source: modules\{#SPARSE}\tests\*.*; DestDir: {app}\modules\{#SPARSE}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
