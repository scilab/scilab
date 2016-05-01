;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) DIGITEO - 2010-2012 - Allan CORNET
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
Source: modules\{#DEVTOOLS}\macros\*.bin; DestDir: {app}\modules\{#DEVTOOLS}\macros; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\*.sci; DestDir: {app}\modules\{#DEVTOOLS}\macros; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\buildmacros.sce; DestDir: {app}\modules\{#DEVTOOLS}\macros; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\buildmacros.bat; DestDir: {app}\modules\{#DEVTOOLS}\macros; Components: {#COMPN_DEVTOOLS}
Source: modules\{#DEVTOOLS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#DEVTOOLS}\macros; Components: {#COMPN_DEVTOOLS}
;
Source: modules\{#DEVTOOLS}\macros\assert\lib; DestDir: {app}\modules\{#DEVTOOLS}\macros\assert; Components: {#COMPN_DEVTOOLS}
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
