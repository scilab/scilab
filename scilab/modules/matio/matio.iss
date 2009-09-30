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
; matio module
;--------------------------------------------------------------------------------------------------------------
;
#define MATIO "matio"
;
Source: bin\matio.dll; DestDir: {app}\bin; Components: {#COMPN_MATIO}
Source: bin\libmatio.dll; DestDir: {app}\bin; Components: {#COMPN_MATIO}
;
Source: modules\{#MATIO}\VERSION.xml; DestDir: {app}\modules\{#MATIO}; Components: {#COMPN_MATIO}
Source: modules\{#MATIO}\readme.txt; DestDir: {app}\modules\{#MATIO}; Components: {#COMPN_MATIO}
Source: modules\{#MATIO}\license.txt; DestDir: {app}\modules\{#MATIO}; Components: {#COMPN_MATIO}
Source: modules\{#MATIO}\changelog.txt; DestDir: {app}\modules\{#MATIO}; Components: {#COMPN_MATIO}
;
Source: modules\{#MATIO}\etc\{#MATIO}.quit; DestDir: {app}\modules\{#MATIO}\etc; Components: {#COMPN_MATIO}
Source: modules\{#MATIO}\etc\{#MATIO}.start; DestDir: {app}\modules\{#MATIO}\etc; Components: {#COMPN_MATIO}
;
Source: modules\{#MATIO}\help\addchapter.sce; DestDir: {app}\modules\{#MATIO}\help; Flags: recursesubdirs; Components:  {#COMPN_MATIO}
;
;Source: modules\{#MATIO}\includes\*.h; DestDir: {app}\modules\{#MATIO}\includes; Components: {#COMPN_MATIO}
;
Source: modules\{#MATIO}\macros\lib; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_MATIO}
Source: modules\{#MATIO}\macros\names; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_MATIO}
Source: modules\{#MATIO}\macros\*.bin; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_MATIO}
Source: modules\{#MATIO}\macros\*.sci; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_MATIO}
Source: modules\{#MATIO}\macros\buildmacros.sce; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_MATIO}
Source: modules\{#MATIO}\macros\buildmacros.bat; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_MATIO}
Source: modules\{#MATIO}\macros\cleanmacros.bat; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_MATIO}
;
Source: modules\{#MATIO}\sci_gateway\matio_gateway.xml; DestDir: {app}\modules\{#MATIO}\sci_gateway; Components: {#COMPN_MATIO}
;
;Source: modules\{#MATIO}\demos\*.*; DestDir: {app}\modules\{#MATIO}\demos; Flags: recursesubdirs; Components: {#COMPN_MATIO}
;
Source: modules\{#MATIO}\tests\*.*; DestDir: {app}\modules\{#MATIO}\tests; Flags: recursesubdirs; Components: {#COMPN_MATIO} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
