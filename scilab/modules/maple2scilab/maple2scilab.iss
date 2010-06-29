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
