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
; matio module
;--------------------------------------------------------------------------------------------------------------
;
#define MATIO "matio"
;
Source: bin\{#MATIO}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\lib{#MATIO}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#MATIO}_gw.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#MATIO}\license.txt; DestDir: {app}\modules\{#MATIO}; Components: {#COMPN_SCILAB}
;
Source: modules\{#MATIO}\etc\{#MATIO}.quit; DestDir: {app}\modules\{#MATIO}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#MATIO}\etc\{#MATIO}.start; DestDir: {app}\modules\{#MATIO}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#MATIO}\macros\lib; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MATIO}\macros\*.sci; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MATIO}\macros\*.bin; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MATIO}\macros\buildmacros.sce; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MATIO}\macros\buildmacros.bat; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MATIO}\macros\cleanmacros.bat; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#MATIO}\sci_gateway\{#MATIO}_gateway.xml; DestDir: {app}\modules\{#MATIO}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#MATIO}\tests\*.*; DestDir: {app}\modules\{#MATIO}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;--------------------------------------------------------------------------------------------------------------
