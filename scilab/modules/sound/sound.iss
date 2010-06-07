;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) DIGITEO - 2010 - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
Source: bin\sound.dll; DestDir: {app}\bin; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\changelog.txt; DestDir: {app}\modules\{#SOUND}; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\license.txt; DestDir: {app}\modules\{#SOUND}; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\readme.txt; DestDir: {app}\modules\{#SOUND}; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\VERSION.xml; DestDir: {app}\modules\{#SOUND}; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\sci_gateway\{#SOUND}_gateway.xml; DestDir: {app}\modules\{#SOUND}\sci_gateway; Components: {#COMPN_SOUND}
;
;Source: modules\{#SOUND}\includes\*.h; DestDir: {app}\modules\{#SOUND}\includes; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\etc\{#SOUND}.quit; DestDir: {app}\modules\{#SOUND}\etc; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\etc\{#SOUND}.start; DestDir: {app}\modules\{#SOUND}\etc; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\macros\buildmacros.sce; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\macros\buildmacros.bat; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\macros\*.bin; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\macros\*.sci; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\macros\lib; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\macros\names; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\demos\*.*; DestDir: {app}\modules\{#SOUND}\demos; Flags: recursesubdirs; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\tests\*.*; DestDir: {app}\modules\{#SOUND}\tests; Flags: recursesubdirs; Components: {#COMPN_SOUND} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
