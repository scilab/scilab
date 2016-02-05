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
; modules_manager module
;--------------------------------------------------------------------------------------------------------------
;
#define MODULES_MANAGER "modules_manager"
;
Source: modules\{#MODULES_MANAGER}\license.txt; DestDir: {app}\modules\{#MODULES_MANAGER}; Components: {#COMPN_MODULES_MANAGER}
;
Source: modules\{#MODULES_MANAGER}\etc\{#MODULES_MANAGER}.quit; DestDir: {app}\modules\{#MODULES_MANAGER}\etc; Components: {#COMPN_MODULES_MANAGER}
Source: modules\{#MODULES_MANAGER}\etc\{#MODULES_MANAGER}.start; DestDir: {app}\modules\{#MODULES_MANAGER}\etc; Components: {#COMPN_MODULES_MANAGER}
;
;Source: modules\{#MODULES_MANAGER}\includes\*.h; DestDir: {app}\modules\{#MODULES_MANAGER}\includes; Flags: recursesubdirs; Components: {#COMPN_MODULES_MANAGER}
;
Source: modules\{#MODULES_MANAGER}\macros\lib; DestDir: {app}\modules\{#MODULES_MANAGER}\macros; Components: {#COMPN_MODULES_MANAGER}
Source: modules\{#MODULES_MANAGER}\macros\*.sci; DestDir: {app}\modules\{#MODULES_MANAGER}\macros; Components: {#COMPN_MODULES_MANAGER}
Source: modules\{#MODULES_MANAGER}\macros\*.bin; DestDir: {app}\modules\{#MODULES_MANAGER}\macros; Components: {#COMPN_MODULES_MANAGER}
Source: modules\{#MODULES_MANAGER}\macros\buildmacros.sce; DestDir: {app}\modules\{#MODULES_MANAGER}\macros; Components: {#COMPN_MODULES_MANAGER}
Source: modules\{#MODULES_MANAGER}\macros\buildmacros.bat; DestDir: {app}\modules\{#MODULES_MANAGER}\macros; Components: {#COMPN_MODULES_MANAGER}
Source: modules\{#MODULES_MANAGER}\macros\cleanmacros.bat; DestDir: {app}\modules\{#MODULES_MANAGER}\macros; Components: {#COMPN_MODULES_MANAGER}
;
;Source: modules\{#MODULES_MANAGER}\demos\*.*; DestDir: {app}\modules\{#MODULES_MANAGER}\demos; Flags: recursesubdirs; Components: {#COMPN_MODULES_MANAGER}
;
;Source: modules\{#MODULES_MANAGER}\examples\*.*; DestDir: {app}\modules\{#MODULES_MANAGER}\examples; Flags: recursesubdirs; Components: {#COMPN_MODULES_MANAGER}
;
Source: modules\{#MODULES_MANAGER}\tests\*.*; DestDir: {app}\modules\{#MODULES_MANAGER}\tests; Flags: recursesubdirs; Components: {#COMPN_MODULES_MANAGER} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
