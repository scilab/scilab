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
; SHELL module
;--------------------------------------------------------------------------------------------------------------
;
#define SHELL "shell"
;
Source: bin\lib{#SHELL}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#SHELL}\jar\org.scilab.modules.{#SHELL}.jar;DestDir: {app}\modules\{#SHELL}\jar; Components: {#COMPN_SCILAB}
;
;
Source: modules\{#SHELL}\changelog.txt; DestDir: {app}\modules\{#SHELL}; Components: {#COMPN_SCILAB};
Source: modules\{#SHELL}\license.txt; DestDir: {app}\modules\{#SHELL}; Components: {#COMPN_SCILAB}
Source: modules\{#SHELL}\readme.txt; DestDir: {app}\modules\{#SHELL}; Components: {#COMPN_SCILAB}
Source: modules\{#SHELL}\VERSION.xml; DestDir: {app}\modules\{#SHELL}; Components: {#COMPN_SCILAB}
;
Source: modules\{#SHELL}\sci_gateway\{#SHELL}_gateway.xml; DestDir: {app}\modules\{#SHELL}\sci_gateway; Components: {#COMPN_SCILAB}
;
;Source: modules\{#SHELL}\includes\*.h; DestDir: {app}\modules\{#SHELL}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#SHELL}\includes\prompt.h; DestDir: {app}\modules\{#SHELL}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#SHELL}\includes\scilines.h; DestDir: {app}\modules\{#SHELL}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#SHELL}\includes\dynlib_shell.h; DestDir: {app}\modules\{#SHELL}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#SHELL}\etc\{#SHELL}.quit; DestDir: {app}\modules\{#SHELL}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#SHELL}\etc\{#SHELL}.start; DestDir: {app}\modules\{#SHELL}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#SHELL}\macros\buildmacros.sce; DestDir: {app}\modules\{#SHELL}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SHELL}\macros\buildmacros.bat; DestDir: {app}\modules\{#SHELL}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SHELL}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SHELL}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#SHELL}\macros\*.bin; DestDir: {app}\modules\{#SHELL}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#SHELL}\macros\*.sci; DestDir: {app}\modules\{#SHELL}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#SHELL}\macros\lib; DestDir: {app}\modules\{#SHELL}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#SHELL}\macros\names; DestDir: {app}\modules\{#SHELL}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#SHELL}\demos\*.*; DestDir: {app}\modules\{#SHELL}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#SHELL}\tests\*.*; DestDir: {app}\modules\{#SHELL}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
