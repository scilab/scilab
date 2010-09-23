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
; texmacs module
;--------------------------------------------------------------------------------------------------------------
;
#define TEXMACS "texmacs"
;
Source: modules\{#TEXMACS}\VERSION.xml; DestDir: {app}\modules\{#TEXMACS}; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\readme.txt; DestDir: {app}\modules\{#TEXMACS}; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\license.txt; DestDir: {app}\modules\{#TEXMACS}; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\changelog.txt; DestDir: {app}\modules\{#TEXMACS}; Components: {#COMPN_TEXMACS}
;
Source: modules\{#TEXMACS}\etc\{#TEXMACS}.quit; DestDir: {app}\modules\{#TEXMACS}\etc; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\etc\{#TEXMACS}.start; DestDir: {app}\modules\{#TEXMACS}\etc; Components: {#COMPN_TEXMACS}
;
Source: modules\{#TEXMACS}\macros\lib; DestDir: {app}\modules\{#TEXMACS}\macros; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\macros\names; DestDir: {app}\modules\{#TEXMACS}\macros; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\macros\*.bin; DestDir: {app}\modules\{#TEXMACS}\macros; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\macros\*.sci; DestDir: {app}\modules\{#TEXMACS}\macros; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\macros\buildmacros.sce; DestDir: {app}\modules\{#TEXMACS}\macros; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\macros\buildmacros.bat; DestDir: {app}\modules\{#TEXMACS}\macros; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#TEXMACS}\macros; Components: {#COMPN_TEXMACS}
;
Source: modules\{#TEXMACS}\macros\overloading\lib; DestDir: {app}\modules\{#TEXMACS}\macros\overloading; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\macros\overloading\names; DestDir: {app}\modules\{#TEXMACS}\macros\overloading; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\macros\overloading\*.bin; DestDir: {app}\modules\{#TEXMACS}\macros\overloading; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\macros\overloading\*.sci; DestDir: {app}\modules\{#TEXMACS}\macros\overloading; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\macros\overloading\buildmacros.sce; DestDir: {app}\modules\{#TEXMACS}\macros\overloading; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\macros\overloading\buildmacros.bat; DestDir: {app}\modules\{#TEXMACS}\macros\overloading; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\macros\overloading\cleanmacros.bat; DestDir: {app}\modules\{#TEXMACS}\macros\overloading; Components: {#COMPN_TEXMACS}
;
;Source: modules\{#TEXMACS}\demos\*.*; DestDir: {app}\modules\{#TEXMACS}\demos; Flags: recursesubdirs; Components: {#COMPN_TEXMACS}
;
;Source: modules\{#TEXMACS}\tests\*.*; DestDir: {app}\modules\{#TEXMACS}\tests; Flags: recursesubdirs; Components: {#COMPN_TEXMACS} and {#COMPN_TESTS}
;
;
;--------------------------------------------------------------------------------------------------------------
