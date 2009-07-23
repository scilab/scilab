;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2009 - DIGITEO - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;

;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; api_scilab module
;--------------------------------------------------------------------------------------------------------------
;
#define MODULE_API_SCILAB "api_scilab"
;
Source: bin\api_scilab.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\api_scilab.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#MODULE_API_SCILAB}\changelog.txt; DestDir: {app}\modules\{#MODULE_API_SCILAB}; Components: {#COMPN_SCILAB}
Source: modules\{#MODULE_API_SCILAB}\license.txt; DestDir: {app}\modules\{#MODULE_API_SCILAB}; Components: {#COMPN_SCILAB}
Source: modules\{#MODULE_API_SCILAB}\readme.txt; DestDir: {app}\modules\{#MODULE_API_SCILAB}; Components: {#COMPN_SCILAB}
Source: modules\{#MODULE_API_SCILAB}\version.xml; DestDir: {app}\modules\{#MODULE_API_SCILAB}; Components: {#COMPN_SCILAB}
;
Source: modules\{#MODULE_API_SCILAB}\includes\*.h; DestDir: {app}\modules\{#MODULE_API_SCILAB}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#MODULE_API_SCILAB}\etc\{#MODULE_API_SCILAB}.quit; DestDir: {app}\modules\{#MODULE_API_SCILAB}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#MODULE_API_SCILAB}\etc\{#MODULE_API_SCILAB}.start; DestDir: {app}\modules\{#MODULE_API_SCILAB}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#MODULE_API_SCILAB}\macros\buildmacros.sce; DestDir: {app}\modules\{#MODULE_API_SCILAB}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MODULE_API_SCILAB}\macros\buildmacros.bat; DestDir: {app}\modules\{#MODULE_API_SCILAB}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MODULE_API_SCILAB}\macros\cleanmacros.bat; DestDir: {app}\modules\{#MODULE_API_SCILAB}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#MODULE_API_SCILAB}\examples\*.*; DestDir: {app}\modules\{#MODULE_API_SCILAB}\examples; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
Source: modules\{#MODULE_API_SCILAB}\help\addchapter.sce; DestDir: {app}\modules\{#MODULE_API_SCILAB}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
;Source: modules\{#MODULE_API_SCILAB}\tests\*.*; DestDir: {app}\modules\{#MODULE_API_SCILAB}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
