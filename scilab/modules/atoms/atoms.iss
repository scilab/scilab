;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2009 - Allan CORNET
; Copyright (C) DIGITEO - 2010 - Pierre MARECHAL
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
; atoms module
;--------------------------------------------------------------------------------------------------------------
;
#define ATOMS "atoms"
;
Source: modules\{#ATOMS}\VERSION.xml; DestDir: {app}\modules\{#ATOMS}; Components: {#COMPN_ATOMS}
Source: modules\{#ATOMS}\readme.txt; DestDir: {app}\modules\{#ATOMS}; Components: {#COMPN_ATOMS}
Source: modules\{#ATOMS}\license.txt; DestDir: {app}\modules\{#ATOMS}; Components: {#COMPN_ATOMS}
Source: modules\{#ATOMS}\changelog.txt; DestDir: {app}\modules\{#ATOMS}; Components: {#COMPN_ATOMS}
;
Source: modules\{#ATOMS}\etc\{#ATOMS}.quit; DestDir: {app}\modules\{#ATOMS}\etc; Components: {#COMPN_ATOMS}
Source: modules\{#ATOMS}\etc\{#ATOMS}.start; DestDir: {app}\modules\{#ATOMS}\etc; Components: {#COMPN_ATOMS}
Source: modules\{#ATOMS}\etc\repositories; DestDir: {app}\modules\{#ATOMS}\etc; Components: {#COMPN_ATOMS}
Source: modules\{#ATOMS}\images\icons\*.png; DestDir: {app}\modules\{#ATOMS}\images\icons; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#ATOMS}\macros\lib; DestDir: {app}\modules\{#ATOMS}\macros; Flags: recursesubdirs; Components: {#COMPN_ATOMS}
Source: modules\{#ATOMS}\macros\names; DestDir: {app}\modules\{#ATOMS}\macros; Flags: recursesubdirs; Components: {#COMPN_ATOMS}
Source: modules\{#ATOMS}\macros\*.bin; DestDir: {app}\modules\{#ATOMS}\macros; Flags: recursesubdirs; Components: {#COMPN_ATOMS}
Source: modules\{#ATOMS}\macros\*.sci; DestDir: {app}\modules\{#ATOMS}\macros; Flags: recursesubdirs; Components: {#COMPN_ATOMS}
Source: modules\{#ATOMS}\macros\buildmacros.sce; DestDir: {app}\modules\{#ATOMS}\macros; Flags: recursesubdirs; Components: {#COMPN_ATOMS}
Source: modules\{#ATOMS}\macros\buildmacros.bat; DestDir: {app}\modules\{#ATOMS}\macros; Flags: recursesubdirs; Components: {#COMPN_ATOMS}
Source: modules\{#ATOMS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#ATOMS}\macros; Flags: recursesubdirs; Components: {#COMPN_ATOMS}
;
;Source: modules\{#ATOMS}\demos\*.*; DestDir: {app}\modules\{#ATOMS}\demos; Flags: recursesubdirs; Components: {#COMPN_ATOMS}
;
Source: modules\{#ATOMS}\tests\*.*; DestDir: {app}\modules\{#ATOMS}\tests; Flags: recursesubdirs; Components: {#COMPN_ATOMS} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
