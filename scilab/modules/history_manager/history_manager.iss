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
; history_manager module
;--------------------------------------------------------------------------------------------------------------
;
#define HISTORY_MANAGER "history_manager"
;
Source: bin\scihistory_manager.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#HISTORY_MANAGER}\jar\org.scilab.modules.history_manager.jar;DestDir: {app}\modules\{#HISTORY_MANAGER}\jar; Components: {#COMPN_SCILAB}
;
;Source: bin\noscihistory_manger.dll; DestDir: {app}\bin; DestName: scihistory_manager.dll; Components: not ({#COMPN_SCILAB})
;
Source: modules\{#HISTORY_MANAGER}\changelog.txt; DestDir: {app}\modules\{#HISTORY_MANAGER}; Components: {#COMPN_SCILAB};
Source: modules\{#HISTORY_MANAGER}\license.txt; DestDir: {app}\modules\{#HISTORY_MANAGER}; Components: {#COMPN_SCILAB}
Source: modules\{#HISTORY_MANAGER}\readme.txt; DestDir: {app}\modules\{#HISTORY_MANAGER}; Components: {#COMPN_SCILAB}
Source: modules\{#HISTORY_MANAGER}\version.xml; DestDir: {app}\modules\{#HISTORY_MANAGER}; Components: {#COMPN_SCILAB}
;
Source: modules\{#HISTORY_MANAGER}\sci_gateway\history_manager_gateway.xml; DestDir: {app}\modules\{#HISTORY_MANAGER}\sci_gateway; Components: {#COMPN_SCILAB}
;
;Source: modules\{#HISTORY_MANAGER}\includes\*.h; DestDir: {app}\modules\{#HISTORY_MANAGER}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#HISTORY_MANAGER}\help\addchapter.sce; DestDir: {app}\modules\{#HISTORY_MANAGER}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
Source: modules\{#HISTORY_MANAGER}\etc\{#HISTORY_MANAGER}.quit; DestDir: {app}\modules\{#HISTORY_MANAGER}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#HISTORY_MANAGER}\etc\{#HISTORY_MANAGER}.start; DestDir: {app}\modules\{#HISTORY_MANAGER}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#HISTORY_MANAGER}\macros\buildmacros.sce; DestDir: {app}\modules\{#HISTORY_MANAGER}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#HISTORY_MANAGER}\macros\buildmacros.bat; DestDir: {app}\modules\{#HISTORY_MANAGER}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#HISTORY_MANAGER}\macros\cleanmacros.bat; DestDir: {app}\modules\{#HISTORY_MANAGER}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#HISTORY_MANAGER}\macros\*.bin; DestDir: {app}\modules\{#HISTORY_MANAGER}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#HISTORY_MANAGER}\macros\*.sci; DestDir: {app}\modules\{#HISTORY_MANAGER}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#HISTORY_MANAGER}\macros\lib; DestDir: {app}\modules\{#HISTORY_MANAGER}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#HISTORY_MANAGER}\macros\names; DestDir: {app}\modules\{#HISTORY_MANAGER}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#HISTORY_MANAGER}\demos\*.*; DestDir: {app}\modules\{#HISTORY_MANAGER}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#HISTORY_MANAGER}\tests\*.*; DestDir: {app}\modules\{#HISTORY_MANAGER}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
