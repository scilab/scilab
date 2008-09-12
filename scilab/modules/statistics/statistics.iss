;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2006-2008 - INRIA - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; statistics module
;--------------------------------------------------------------------------------------------------------------
;
#define STATS "statistics"

Source: bin\statistics.dll; DestDir: {app}\bin; Components: {#COMPN_STATS}
Source: bin\dcd.dll; DestDir: {app}\bin; Components: {#COMPN_STATS}
;
Source: modules\{#STATS}\VERSION.xml; DestDir: {app}\modules\{#STATS}; Components: {#COMPN_STATS}
Source: modules\{#STATS}\readme.txt; DestDir: {app}\modules\{#STATS}; Components: {#COMPN_STATS}
Source: modules\{#STATS}\license.txt; DestDir: {app}\modules\{#STATS}; Components: {#COMPN_STATS}
Source: modules\{#STATS}\changelog.txt; DestDir: {app}\modules\{#STATS}; Components: {#COMPN_STATS}
;
Source: modules\{#STATS}\sci_gateway\statistics_gateway.xml; DestDir: {app}\modules\{#STATS}\sci_gateway; Components: {#COMPN_STATS}
;
Source: modules\{#STATS}\etc\{#STATS}.quit; DestDir: {app}\modules\{#STATS}\etc; Components: {#COMPN_STATS}
Source: modules\{#STATS}\etc\{#STATS}.start; DestDir: {app}\modules\{#STATS}\etc; Components: {#COMPN_STATS}
;
Source: modules\{#STATS}\help\addchapter.sce; DestDir: {app}\modules\{#STATS}\help; Flags: recursesubdirs; Components:  {#COMPN_STATS}
;
Source: modules\{#STATS}\macros\lib; DestDir: {app}\modules\{#STATS}\macros; Components: {#COMPN_STATS}
Source: modules\{#STATS}\macros\names; DestDir: {app}\modules\{#STATS}\macros; Components: {#COMPN_STATS}
Source: modules\{#STATS}\macros\*.bin; DestDir: {app}\modules\{#STATS}\macros; Components: {#COMPN_STATS}
Source: modules\{#STATS}\macros\*.sci; DestDir: {app}\modules\{#STATS}\macros; Components: {#COMPN_STATS}
Source: modules\{#STATS}\macros\buildmacros.sce; DestDir: {app}\modules\{#STATS}\macros; Components: {#COMPN_STATS}
Source: modules\{#STATS}\macros\buildmacros.bat; DestDir: {app}\modules\{#STATS}\macros; Components: {#COMPN_STATS}
Source: modules\{#STATS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#STATS}\macros; Components: {#COMPN_STATS}
;
;Source: modules\{#STATS}\demos\*.*; DestDir: {app}\modules\{#STATS}\demos; Flags: recursesubdirs; Components: {#COMPN_STATS}
;
Source: modules\{#STATS}\tests\*.*; DestDir: {app}\modules\{#STATS}\tests; Flags: recursesubdirs; Components: {#COMPN_STATS} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
