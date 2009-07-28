; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2009 - DIGITEO - Allan CORNET
;
; This file is distributed under the same license as the Scilab package.
;
;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Version TRUNK
;##############################################################################################################
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
;
Source: modules\{#ATOMS}\help\addchapter.sce; DestDir: {app}\modules\{#ATOMS}\help; Flags: recursesubdirs; Components:  {#COMPN_ATOMS}
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

