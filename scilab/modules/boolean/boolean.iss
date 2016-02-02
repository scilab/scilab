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
; boolean module
;--------------------------------------------------------------------------------------------------------------
;
#define BOOLEAN "boolean"
;
Source: bin\{#BOOLEAN}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#BOOLEAN}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#BOOLEAN}_gw.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#BOOLEAN}\license.txt; DestDir: {app}\modules\{#BOOLEAN}; Components: {#COMPN_SCILAB}
;
Source: modules\{#BOOLEAN}\sci_gateway\{#BOOLEAN}_gateway.xml; DestDir: {app}\modules\{#BOOLEAN}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#BOOLEAN}\etc\{#BOOLEAN}.quit; DestDir: {app}\modules\{#BOOLEAN}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#BOOLEAN}\etc\{#BOOLEAN}.start; DestDir: {app}\modules\{#BOOLEAN}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#BOOLEAN}\includes\*.h; DestDir: {app}\modules\{#BOOLEAN}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#BOOLEAN}\macros\buildmacros.sce; DestDir: {app}\modules\{#BOOLEAN}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#BOOLEAN}\macros\buildmacros.bat; DestDir: {app}\modules\{#BOOLEAN}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#BOOLEAN}\macros\cleanmacros.bat; DestDir: {app}\modules\{#BOOLEAN}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#BOOLEAN}\demos\*.*; DestDir: {app}\modules\{#BOOLEAN}\demos; Components: {#COMPN_SCILAB}
;
Source: modules\{#BOOLEAN}\tests\*.*; DestDir: {app}\modules\{#BOOLEAN}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
