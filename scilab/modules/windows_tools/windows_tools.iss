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
; windows_tools module
;--------------------------------------------------------------------------------------------------------------
;
#define WINDOWS_TOOLS "windows_tools"
;
Source: bin\windows_tools.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
;Source: java\jar\modules\org.scilab.modules.{#WINDOWS_TOOLS}.jar;DestDir: {app}\java\jar\modules; Components: {#COMPN_SCILAB}
;
;Source: bin\no{#WINDOWS_TOOLS}.dll; DestDir: {app}\bin; DestName: windows_tools.dll; Components: not ({#COMPN_SCILAB})
;
Source: modules\{#WINDOWS_TOOLS}\license.txt; DestDir: {app}\modules\{#WINDOWS_TOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#WINDOWS_TOOLS}\version.xml; DestDir: {app}\modules\{#WINDOWS_TOOLS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#WINDOWS_TOOLS}\sci_gateway\{#WINDOWS_TOOLS}_gateway.xml; DestDir: {app}\modules\{#WINDOWS_TOOLS}\sci_gateway; Components: {#COMPN_SCILAB}
;
;Source: modules\{#WINDOWS_TOOLS}\includes\*.h; DestDir: {app}\modules\{#WINDOWS_TOOLS}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#WINDOWS_TOOLS}\includes\strdup_windows.h; DestDir: {app}\modules\{#WINDOWS_TOOLS}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#WINDOWS_TOOLS}\etc\{#WINDOWS_TOOLS}.quit; DestDir: {app}\modules\{#WINDOWS_TOOLS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#WINDOWS_TOOLS}\etc\{#WINDOWS_TOOLS}.start; DestDir: {app}\modules\{#WINDOWS_TOOLS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#WINDOWS_TOOLS}\macros\buildmacros.sce; DestDir: {app}\modules\{#WINDOWS_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#WINDOWS_TOOLS}\macros\buildmacros.bat; DestDir: {app}\modules\{#WINDOWS_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#WINDOWS_TOOLS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#WINDOWS_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#WINDOWS_TOOLS}\macros\*.bin; DestDir: {app}\modules\{#WINDOWS_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#WINDOWS_TOOLS}\macros\*.sci; DestDir: {app}\modules\{#WINDOWS_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#WINDOWS_TOOLS}\macros\lib; DestDir: {app}\modules\{#WINDOWS_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#WINDOWS_TOOLS}\macros\names; DestDir: {app}\modules\{#WINDOWS_TOOLS}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#WINDOWS_TOOLS}\demos\*.*; DestDir: {app}\modules\{#WINDOWS_TOOLS}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#WINDOWS_TOOLS}\tests\*.*; DestDir: {app}\modules\{#WINDOWS_TOOLS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
