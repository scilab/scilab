;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) DIGITEO - 2010-2012 - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
;
;--------------------------------------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;--------------------------------------------------------------------------------------------------------------
; development_tools module
;--------------------------------------------------------------------------------------------------------------
;
#define DEVTOOLS "development_tools"
;
Source: modules\{#DEVTOOLS}\license.txt; DestDir: {app}\modules\{#DEVTOOLS}; Components: {#COMPN_DEVTOOLS}
;
Source: modules\{#DEVTOOLS}\etc\{#DEVTOOLS}.quit; DestDir: {app}\modules\{#DEVTOOLS}\etc; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\etc\{#DEVTOOLS}.start; DestDir: {app}\modules\{#DEVTOOLS}\etc; Components: {#COMPN_DEVTOOLS}
;
;Source: modules\{#DEVTOOLS}\includes\*.h; DestDir: {app}\modules\{#DEVTOOLS}\includes; Flags: recursesubdirs; Components: {#COMPN_DEVTOOLS}
;
Source: modules\{#DEVTOOLS}\macros\lib; DestDir: {app}\modules\{#DEVTOOLS}\macros; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\names; DestDir: {app}\modules\{#DEVTOOLS}\macros; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\*.bin; DestDir: {app}\modules\{#DEVTOOLS}\macros; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\*.sci; DestDir: {app}\modules\{#DEVTOOLS}\macros; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\buildmacros.sce; DestDir: {app}\modules\{#DEVTOOLS}\macros; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\buildmacros.bat; DestDir: {app}\modules\{#DEVTOOLS}\macros; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#DEVTOOLS}\macros; Components: {#COMPN_DEVTOOLS}
;
Source: modules\{#DEVTOOLS}\macros\assert\lib; DestDir: {app}\modules\{#DEVTOOLS}\macros\assert; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\assert\names; DestDir: {app}\modules\{#DEVTOOLS}\macros\assert; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\assert\*.bin; DestDir: {app}\modules\{#DEVTOOLS}\macros\assert; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\assert\*.sci; DestDir: {app}\modules\{#DEVTOOLS}\macros\assert; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\assert\buildmacros.sce; DestDir: {app}\modules\{#DEVTOOLS}\macros\assert; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\assert\buildmacros.bat; DestDir: {app}\modules\{#DEVTOOLS}\macros\assert; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\assert\cleanmacros.bat; DestDir: {app}\modules\{#DEVTOOLS}\macros\assert; Components: {#COMPN_DEVTOOLS}
;
;Source: modules\{#DEVTOOLS}\demos\*.*; DestDir: {app}\modules\{#DEVTOOLS}\demos; Flags: recursesubdirs; Components: {#COMPN_DEVTOOLS}
;
;Source: modules\{#DEVTOOLS}\examples\*.*; DestDir: {app}\modules\{#DEVTOOLS}\examples; Flags: recursesubdirs; Components: {#COMPN_DEVTOOLS}
;
Source: modules\{#DEVTOOLS}\tests\*.*; DestDir: {app}\modules\{#DEVTOOLS}\tests; Flags: recursesubdirs; Components: {#COMPN_DEVTOOLS} and {#COMPN_TESTS}
Source: modules\{#DEVTOOLS}\data\test_run_level.xml; DestDir: {app}\modules\{#DEVTOOLS}\data; Flags: recursesubdirs; Components: {#COMPN_DEVTOOLS} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
