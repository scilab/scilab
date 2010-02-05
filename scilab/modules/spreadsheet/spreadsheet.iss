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
; spreadsheet module
;--------------------------------------------------------------------------------------------------------------
;
#define SPREADSHEET "spreadsheet"
;
Source: bin\spreadsheet.dll; DestDir: {app}\bin; Components: {#COMPN_SPREADSHEET}
;
Source: bin\nospreadsheet.dll; DestDir: {app}\bin; DestName: spreadsheet.dll; Components: not ({#COMPN_SPREADSHEET})
;
Source: modules\{#SPREADSHEET}\changelog.txt; DestDir: {app}\modules\{#SPREADSHEET}; Components: {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\license.txt; DestDir: {app}\modules\{#SPREADSHEET}; Components: {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\readme.txt; DestDir: {app}\modules\{#SPREADSHEET}; Components: {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\VERSION.xml; DestDir: {app}\modules\{#SPREADSHEET}; Components: {#COMPN_SPREADSHEET}
;
Source: modules\{#SPREADSHEET}\sci_gateway\spreadsheet_gateway.xml; DestDir: {app}\modules\{#SPREADSHEET}\sci_gateway; Components: {#COMPN_SPREADSHEET}
;
;Source: modules\{#SPREADSHEET}\includes\*.h; DestDir: {app}\modules\{#SPREADSHEET}\includes; Components: {#COMPN_SPREADSHEET}
;
Source: modules\{#SPREADSHEET}\etc\{#SPREADSHEET}.quit; DestDir: {app}\modules\{#SPREADSHEET}\etc; Components: {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\etc\{#SPREADSHEET}.start; DestDir: {app}\modules\{#SPREADSHEET}\etc; Components: {#COMPN_SPREADSHEET}
;
Source: modules\{#SPREADSHEET}\help\addchapter.sce; DestDir: {app}\modules\{#SPREADSHEET}\help; Flags: recursesubdirs; Components:  {#COMPN_SPREADSHEET}
;
Source: modules\{#SPREADSHEET}\macros\buildmacros.sce; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\macros\buildmacros.bat; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\macros\*.bin; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\macros\*.sci; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\macros\lib; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SPREADSHEET}
Source: modules\{#SPREADSHEET}\macros\names; DestDir: {app}\modules\{#SPREADSHEET}\macros; Components: {#COMPN_SPREADSHEET}
;
Source: modules\{#SPREADSHEET}\demos\*.*; DestDir: {app}\modules\{#SPREADSHEET}\demos; Flags: recursesubdirs; Components: {#COMPN_SPREADSHEET}
;
Source: modules\{#SPREADSHEET}\tests\*.*; DestDir: {app}\modules\{#SPREADSHEET}\tests; Flags: recursesubdirs; Components: {#COMPN_SPREADSHEET} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
