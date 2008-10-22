;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;
;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Version TRUNK
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; maple2scilab module
;--------------------------------------------------------------------------------------------------------------
;
#define MAP2SCI "maple2scilab"
;
Source: modules\{#MAP2SCI}\VERSION.xml; DestDir: {app}\modules\{#MAP2SCI}; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\readme.txt; DestDir: {app}\modules\{#MAP2SCI}; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\license.txt; DestDir: {app}\modules\{#MAP2SCI}; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\changelog.txt; DestDir: {app}\modules\{#MAP2SCI}; Components: {#COMPN_MAP2SCI}
;
Source: modules\{#MAP2SCI}\etc\{#MAP2SCI}.quit; DestDir: {app}\modules\{#MAP2SCI}\etc; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\etc\{#MAP2SCI}.start; DestDir: {app}\modules\{#MAP2SCI}\etc; Components: {#COMPN_MAP2SCI}
;
Source: modules\{#MAP2SCI}\help\addchapter.sce; DestDir: {app}\modules\{#MAP2SCI}\help; Flags: recursesubdirs; Components:  {#COMPN_MAP2SCI}
;
Source: modules\{#MAP2SCI}\src\maple\*.mpl; DestDir: {app}\modules\{#MAP2SCI}\src\maple; Components: {#COMPN_MAP2SCI}
;
Source: modules\{#MAP2SCI}\macros\lib; DestDir: {app}\modules\{#MAP2SCI}\macros; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\macros\names; DestDir: {app}\modules\{#MAP2SCI}\macros; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\macros\*.bin; DestDir: {app}\modules\{#MAP2SCI}\macros; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\macros\*.sci; DestDir: {app}\modules\{#MAP2SCI}\macros; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\macros\buildmacros.sce; DestDir: {app}\modules\{#MAP2SCI}\macros; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\macros\buildmacros.bat; DestDir: {app}\modules\{#MAP2SCI}\macros; Components: {#COMPN_MAP2SCI}
Source: modules\{#MAP2SCI}\macros\cleanmacros.bat; DestDir: {app}\modules\{#MAP2SCI}\macros; Components: {#COMPN_MAP2SCI}
;
;Source: modules\{#MAP2SCI}\demos\*.*; DestDir: {app}\modules\{#MAP2SCI}\demos; Flags: recursesubdirs; Components: {#COMPN_MAP2SCI}
;
Source: modules\{#MAP2SCI}\tests\*.*; DestDir: {app}\modules\{#MAP2SCI}\tests; Flags: recursesubdirs; Components: {#COMPN_MAP2SCI} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
