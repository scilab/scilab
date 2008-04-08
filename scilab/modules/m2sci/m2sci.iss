; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2007 - INRIA - Allan CORNET
; 
; This file is distributed under the same license as the Scilab package.
;
;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Version TRUNK
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; m2sci module
;--------------------------------------------------------------------------------------------------------------
;
#define M2SCI "m2sci"
;
Source: modules\{#M2SCI}\VERSION.xml; DestDir: {app}\modules\{#M2SCI}; Components: {#COMPN_M2SCI}
Source: modules\{#M2SCI}\readme.txt; DestDir: {app}\modules\{#M2SCI}; Components: {#COMPN_M2SCI}
Source: modules\{#M2SCI}\license.txt; DestDir: {app}\modules\{#M2SCI}; Components: {#COMPN_M2SCI}
Source: modules\{#M2SCI}\changelog.txt; DestDir: {app}\modules\{#M2SCI}; Components: {#COMPN_M2SCI}
;
Source: modules\{#M2SCI}\etc\{#M2SCI}.quit; DestDir: {app}\modules\{#M2SCI}\etc; Components: {#COMPN_M2SCI}
Source: modules\{#M2SCI}\etc\{#M2SCI}.start; DestDir: {app}\modules\{#M2SCI}\etc; Components: {#COMPN_M2SCI}
;
Source: modules\{#M2SCI}\help\addchapter.sce; DestDir: {app}\modules\{#M2SCI}\help; Flags: recursesubdirs; Components:  {#COMPN_M2SCI}
;
Source: modules\{#M2SCI}\macros\lib; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_M2SCI}
Source: modules\{#M2SCI}\macros\names; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_M2SCI}
Source: modules\{#M2SCI}\macros\*.bin; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_M2SCI}
Source: modules\{#M2SCI}\macros\*.sci; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_M2SCI}
Source: modules\{#M2SCI}\macros\buildmacros.sce; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_M2SCI}
Source: modules\{#M2SCI}\macros\buildmacros.bat; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_M2SCI}
Source: modules\{#M2SCI}\macros\cleanmacros.bat; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_M2SCI}
;
;Source: modules\{#M2SCI}\demos\*.*; DestDir: {app}\modules\{#M2SCI}\demos; Flags: recursesubdirs; Components: {#COMPN_M2SCI}
;
Source: modules\{#M2SCI}\tests\*.*; DestDir: {app}\modules\{#M2SCI}\tests; Flags: recursesubdirs; Components: {#COMPN_M2SCI} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
