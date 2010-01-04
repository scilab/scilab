;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; boolean module
;--------------------------------------------------------------------------------------------------------------
;
#define BOOLEAN "boolean"
;
Source: modules\{#BOOLEAN}\VERSION.xml; DestDir: {app}\modules\{#BOOLEAN}; Components: {#COMPN_SCILAB}
Source: modules\{#BOOLEAN}\readme.txt; DestDir: {app}\modules\{#BOOLEAN}; Components: {#COMPN_SCILAB}
Source: modules\{#BOOLEAN}\license.txt; DestDir: {app}\modules\{#BOOLEAN}; Components: {#COMPN_SCILAB}
Source: modules\{#BOOLEAN}\changelog.txt; DestDir: {app}\modules\{#BOOLEAN}; Components: {#COMPN_SCILAB}
;
Source: modules\{#BOOLEAN}\sci_gateway\boolean_gateway.xml; DestDir: {app}\modules\{#BOOLEAN}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#BOOLEAN}\etc\{#BOOLEAN}.quit; DestDir: {app}\modules\{#BOOLEAN}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#BOOLEAN}\etc\{#BOOLEAN}.start; DestDir: {app}\modules\{#BOOLEAN}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#BOOLEAN}\help\addchapter.sce; DestDir: {app}\modules\{#BOOLEAN}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;
;Source: modules\{#BOOLEAN}\includes\*.h; DestDir: {app}\modules\{#BOOLEAN}\includes; Components: {#COMPN_SCILAB}
;
;Source: modules\{#BOOLEAN}\macros\lib; DestDir: {app}\modules\{#BOOLEAN}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#BOOLEAN}\macros\names; DestDir: {app}\modules\{#BOOLEAN}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#BOOLEAN}\macros\*.bin; DestDir: {app}\modules\{#BOOLEAN}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#BOOLEAN}\macros\*.sci; DestDir: {app}\modules\{#BOOLEAN}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#BOOLEAN}\macros\buildmacros.sce; DestDir: {app}\modules\{#BOOLEAN}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#BOOLEAN}\macros\buildmacros.bat; DestDir: {app}\modules\{#BOOLEAN}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#BOOLEAN}\macros\cleanmacros.bat; DestDir: {app}\modules\{#BOOLEAN}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#BOOLEAN}\demos\*.*; DestDir: {app}\modules\{#BOOLEAN}\demos; Components: {#COMPN_SCILAB}
;
Source: modules\{#BOOLEAN}\tests\*.*; DestDir: {app}\modules\{#BOOLEAN}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
