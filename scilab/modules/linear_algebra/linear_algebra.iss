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
; linear_algebra module
;--------------------------------------------------------------------------------------------------------------
;
#define LINEAR_ALGEBRA "linear_algebra"
;
Source: bin\{#LINEAR_ALGEBRA}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#LINEAR_ALGEBRA}_gw.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#LINEAR_ALGEBRA}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#LINEAR_ALGEBRA}\license.txt; DestDir: {app}\modules\{#LINEAR_ALGEBRA}; Components: {#COMPN_SCILAB}
;
Source: modules\{#LINEAR_ALGEBRA}\sci_gateway\{#LINEAR_ALGEBRA}_gateway.xml; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#LINEAR_ALGEBRA}\etc\{#LINEAR_ALGEBRA}.quit; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#LINEAR_ALGEBRA}\etc\{#LINEAR_ALGEBRA}.start; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#LINEAR_ALGEBRA}\macros\buildmacros.sce; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#LINEAR_ALGEBRA}\macros\buildmacros.bat; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#LINEAR_ALGEBRA}\macros\cleanmacros.bat; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#LINEAR_ALGEBRA}\macros\lib; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#LINEAR_ALGEBRA}\macros\*.sci; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#LINEAR_ALGEBRA}\macros\*.bin; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#LINEAR_ALGEBRA}\examples\*.*; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\examples; Flags: recursesubdirs;  Components: {#COMPN_SCILAB}
;
Source: modules\{#LINEAR_ALGEBRA}\tests\*.*; DestDir: {app}\modules\{#LINEAR_ALGEBRA}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;--------------------------------------------------------------------------------------------------------------
