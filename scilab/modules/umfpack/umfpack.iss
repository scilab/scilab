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
; UMFPACK module
;--------------------------------------------------------------------------------------------------------------
;
#define UMFPACK "umfpack"
;
Source: bin\lib{#UMFPACK}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#UMFPACK}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#UMFPACK}\license.txt; DestDir: {app}\modules\{#UMFPACK}; Components: {#COMPN_SCILAB}
;
Source: modules\{#UMFPACK}\sci_gateway\{#UMFPACK}_gateway.xml; DestDir: {app}\modules\{#UMFPACK}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#UMFPACK}\etc\{#UMFPACK}.quit; DestDir: {app}\modules\{#UMFPACK}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#UMFPACK}\etc\{#UMFPACK}.start; DestDir: {app}\modules\{#UMFPACK}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#UMFPACK}\includes\*.h; DestDir: {app}\modules\{#UMFPACK}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#UMFPACK}\macros\buildmacros.sce; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#UMFPACK}\macros\buildmacros.bat; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#UMFPACK}\macros\cleanmacros.bat; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#UMFPACK}\macros\lib; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#UMFPACK}\macros\*.sci; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#UMFPACK}\macros\*.bin; DestDir: {app}\modules\{#UMFPACK}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#UMFPACK}\demos\*.*; DestDir: {app}\modules\{#UMFPACK}\demos; Flags: recursesubdirs;  Components: {#COMPN_SCILAB}
;
Source: modules\{#UMFPACK}\tests\*.*; DestDir: {app}\modules\{#UMFPACK}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
