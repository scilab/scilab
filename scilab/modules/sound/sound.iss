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
; sound module
;--------------------------------------------------------------------------------------------------------------
;
#define SOUND "sound"
;
Source: bin\{#SOUND}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#SOUND}\license.txt; DestDir: {app}\modules\{#SOUND}; Components: {#COMPN_SCILAB}
;
Source: modules\{#SOUND}\sci_gateway\{#SOUND}_gateway.xml; DestDir: {app}\modules\{#SOUND}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#SOUND}\etc\{#SOUND}.quit; DestDir: {app}\modules\{#SOUND}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#SOUND}\etc\{#SOUND}.start; DestDir: {app}\modules\{#SOUND}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#SOUND}\macros\buildmacros.sce; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SOUND}\macros\buildmacros.bat; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SOUND}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SOUND}\macros\*.sci; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SOUND}\macros\*.bin; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SOUND}\macros\lib; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#SOUND}\demos\*.*; DestDir: {app}\modules\{#SOUND}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#SOUND}\tests\*.*; DestDir: {app}\modules\{#SOUND}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
