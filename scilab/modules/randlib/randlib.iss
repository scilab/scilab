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
; randlib module
;--------------------------------------------------------------------------------------------------------------
;
#define RANDLIB "randlib"
;
Source: bin\{#RANDLIB}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#RANDLIB}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#RANDLIB}\license.txt; DestDir: {app}\modules\{#RANDLIB}; Components: {#COMPN_SCILAB}
;
Source: modules\{#RANDLIB}\sci_gateway\{#RANDLIB}_gateway.xml; DestDir: {app}\modules\{#RANDLIB}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#RANDLIB}\etc\{#RANDLIB}.quit; DestDir: {app}\modules\{#RANDLIB}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#RANDLIB}\etc\{#RANDLIB}.start; DestDir: {app}\modules\{#RANDLIB}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#RANDLIB}\includes\*.h; DestDir: {app}\modules\{#RANDLIB}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#RANDLIB}\macros\lib; DestDir: {app}\modules\{#RANDLIB}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#RANDLIB}\macros\*.sci; DestDir: {app}\modules\{#RANDLIB}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#RANDLIB}\macros\*.bin; DestDir: {app}\modules\{#RANDLIB}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#RANDLIB}\macros\buildmacros.sce; DestDir: {app}\modules\{#RANDLIB}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#RANDLIB}\macros\buildmacros.bat; DestDir: {app}\modules\{#RANDLIB}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#RANDLIB}\macros\cleanmacros.bat; DestDir: {app}\modules\{#RANDLIB}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#RANDLIB}\demos\*.*; DestDir: {app}\modules\{#RANDLIB}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#RANDLIB}\tests\*.*; DestDir: {app}\modules\{#RANDLIB}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
