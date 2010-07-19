;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
; api_scilab module
;--------------------------------------------------------------------------------------------------------------
;
#define API_SCILAB "api_scilab"
;
Source: bin\{#API_SCILAB}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#API_SCILAB}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#API_SCILAB}\changelog.txt; DestDir: {app}\modules\{#API_SCILAB}; Components: {#COMPN_SCILAB}
Source: modules\{#API_SCILAB}\license.txt; DestDir: {app}\modules\{#API_SCILAB}; Components: {#COMPN_SCILAB}
Source: modules\{#API_SCILAB}\readme.txt; DestDir: {app}\modules\{#API_SCILAB}; Components: {#COMPN_SCILAB}
Source: modules\{#API_SCILAB}\version.xml; DestDir: {app}\modules\{#API_SCILAB}; Components: {#COMPN_SCILAB}
;
Source: modules\{#API_SCILAB}\includes\*.h; DestDir: {app}\modules\{#API_SCILAB}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#API_SCILAB}\etc\{#API_SCILAB}.quit; DestDir: {app}\modules\{#API_SCILAB}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#API_SCILAB}\etc\{#API_SCILAB}.start; DestDir: {app}\modules\{#API_SCILAB}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#API_SCILAB}\macros\buildmacros.sce; DestDir: {app}\modules\{#API_SCILAB}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#API_SCILAB}\macros\buildmacros.bat; DestDir: {app}\modules\{#API_SCILAB}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#API_SCILAB}\macros\cleanmacros.bat; DestDir: {app}\modules\{#API_SCILAB}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#API_SCILAB}\examples\*.*; DestDir: {app}\modules\{#API_SCILAB}\examples; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
Source: modules\{#API_SCILAB}\tests\*.*; DestDir: {app}\modules\{#API_SCILAB}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
