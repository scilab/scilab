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
; localization module
;--------------------------------------------------------------------------------------------------------------
;
#define LOCALIZATION "localization"
;
Source: modules\{#LOCALIZATION}\jar\org.scilab.modules.{#LOCALIZATION}.jar;DestDir: {app}\modules\{#LOCALIZATION}\jar; Components: {#COMPN_SCILAB}
;
Source: bin\sci{#LOCALIZATION}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\sci{#LOCALIZATION}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#LOCALIZATION}\VERSION.xml; DestDir: {app}\modules\{#LOCALIZATION}; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\license.txt; DestDir: {app}\modules\{#LOCALIZATION}; Components: {#COMPN_SCILAB}
;
Source: modules\{#LOCALIZATION}\etc\{#LOCALIZATION}.quit; DestDir: {app}\modules\{#LOCALIZATION}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\etc\{#LOCALIZATION}.start; DestDir: {app}\modules\{#LOCALIZATION}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#LOCALIZATION}\sci_gateway\{#LOCALIZATION}_gateway.xml; DestDir: {app}\modules\{#LOCALIZATION}\sci_gateway; Components: {#COMPN_SCILAB}
;
;Source: modules\{#LOCALIZATION}\includes\*.h; DestDir: {app}\modules\{#LOCALIZATION}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\includes\localization.h; DestDir: {app}\modules\{#LOCALIZATION}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\includes\setgetlanguage.h; DestDir: {app}\modules\{#LOCALIZATION}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\includes\charEncoding.h; DestDir: {app}\modules\{#LOCALIZATION}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\includes\dynlib_localization.h; DestDir: {app}\modules\{#LOCALIZATION}\includes; Components: {#COMPN_SCILAB}
;
;Source: modules\{#LOCALIZATION}\macros\lib; DestDir: {app}\modules\{#LOCALIZATION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#LOCALIZATION}\macros\names; DestDir: {app}\modules\{#LOCALIZATION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#LOCALIZATION}\macros\*.bin; DestDir: {app}\modules\{#LOCALIZATION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#LOCALIZATION}\macros\*.sci; DestDir: {app}\modules\{#LOCALIZATION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#LOCALIZATION}\macros\buildmacros.sce; DestDir: {app}\modules\{#LOCALIZATION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#LOCALIZATION}\macros\buildmacros.bat; DestDir: {app}\modules\{#LOCALIZATION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#LOCALIZATION}\macros\cleanmacros.bat; DestDir: {app}\modules\{#LOCALIZATION}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#LOCALIZATION}\xml\*.dtd; DestDir: {app}\modules\{#LOCALIZATION}\xml; Components: {#COMPN_SCILAB}
;
;Source: modules\{#LOCALIZATION}\demos\*.*; DestDir: {app}\modules\{#LOCALIZATION}\demos; Components: {#COMPN_SCILAB}
;
Source: modules\{#LOCALIZATION}\tests\*.*; DestDir: {app}\modules\{#LOCALIZATION}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
