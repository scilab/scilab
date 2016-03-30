;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
; api_scilab module
;--------------------------------------------------------------------------------------------------------------
;
#define API_SCILAB "api_scilab"
;
Source: bin\{#API_SCILAB}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#API_SCILAB}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#API_SCILAB}\license.txt; DestDir: {app}\modules\{#API_SCILAB}; Components: {#COMPN_SCILAB}
;
Source: modules\{#API_SCILAB}\includes\*.h*; DestDir: {app}\modules\{#API_SCILAB}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#API_SCILAB}\etc\{#API_SCILAB}.quit; DestDir: {app}\modules\{#API_SCILAB}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#API_SCILAB}\etc\{#API_SCILAB}.start; DestDir: {app}\modules\{#API_SCILAB}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#API_SCILAB}\macros\buildmacros.sce; DestDir: {app}\modules\{#API_SCILAB}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#API_SCILAB}\macros\buildmacros.bat; DestDir: {app}\modules\{#API_SCILAB}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#API_SCILAB}\macros\cleanmacros.bat; DestDir: {app}\modules\{#API_SCILAB}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#API_SCILAB}\examples\*.*; DestDir: {app}\modules\{#API_SCILAB}\examples; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
Source: modules\{#API_SCILAB}\tests\*.*; DestDir: {app}\modules\{#API_SCILAB}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
