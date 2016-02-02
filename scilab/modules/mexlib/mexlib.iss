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
; mexlib module
;--------------------------------------------------------------------------------------------------------------
;
#define MEXLIB "mexlib"
;
Source: bin\libmx.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libmx.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libmex.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libmex.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libmat.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libmat.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#MEXLIB}\license.txt; DestDir: {app}\modules\{#MEXLIB}; Components: {#COMPN_SCILAB}
;
Source: modules\{#MEXLIB}\etc\{#MEXLIB}.quit; DestDir: {app}\modules\{#MEXLIB}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#MEXLIB}\etc\{#MEXLIB}.start; DestDir: {app}\modules\{#MEXLIB}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#MEXLIB}\includes\*.h; DestDir: {app}\modules\{#MEXLIB}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#MEXLIB}\macros\buildmacros.sce; DestDir: {app}\modules\{#MEXLIB}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MEXLIB}\macros\buildmacros.bat; DestDir: {app}\modules\{#MEXLIB}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MEXLIB}\macros\cleanmacros.bat; DestDir: {app}\modules\{#MEXLIB}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#MEXLIB}\examples\*.*; DestDir: {app}\modules\{#MEXLIB}\examples; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#MEXLIB}\tests\*.*; DestDir: {app}\modules\{#MEXLIB}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
