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
; spreadsheet module
;--------------------------------------------------------------------------------------------------------------
;
#define SPREADSHEET "spreadsheet"
;
Source: bin\{#SPREADSHEET}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPREADSHEET}\license.txt; DestDir: {app}\modules\{#SPREADSHEET}; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPREADSHEET}\sci_gateway\{#SPREADSHEET}_gateway.xml; DestDir: {app}\modules\{#SPREADSHEET}\sci_gateway; Components: {#COMPN_SCILAB}
;
;Source: modules\{#SPREADSHEET}\includes\*.h; DestDir: {app}\modules\{#SPREADSHEET}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPREADSHEET}\etc\{#SPREADSHEET}.quit; DestDir: {app}\modules\{#SPREADSHEET}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#SPREADSHEET}\etc\{#SPREADSHEET}.start; DestDir: {app}\modules\{#SPREADSHEET}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPREADSHEET}\macros\buildmacros.sce; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SPREADSHEET}\macros\buildmacros.bat; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SPREADSHEET}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SPREADSHEET}\macros\*.sci; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SPREADSHEET}\macros\*.bin; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SPREADSHEET}\macros\lib; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPREADSHEET}\demos\*.*; DestDir: {app}\modules\{#SPREADSHEET}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#SPREADSHEET}\tests\*.*; DestDir: {app}\modules\{#SPREADSHEET}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
