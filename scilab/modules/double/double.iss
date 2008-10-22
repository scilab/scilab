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
; double module
;--------------------------------------------------------------------------------------------------------------
;
#define DOUBLE "double"
;
Source: modules\{#DOUBLE}\VERSION.xml; DestDir: {app}\modules\{#DOUBLE}; Components: {#COMPN_SCILAB}
Source: modules\{#DOUBLE}\readme.txt; DestDir: {app}\modules\{#DOUBLE}; Components: {#COMPN_SCILAB}
Source: modules\{#DOUBLE}\license.txt; DestDir: {app}\modules\{#DOUBLE}; Components: {#COMPN_SCILAB}
Source: modules\{#DOUBLE}\changelog.txt; DestDir: {app}\modules\{#DOUBLE}; Components: {#COMPN_SCILAB}
;
Source: modules\{#DOUBLE}\etc\{#DOUBLE}.quit; DestDir: {app}\modules\{#DOUBLE}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#DOUBLE}\etc\{#DOUBLE}.start; DestDir: {app}\modules\{#DOUBLE}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#DOUBLE}\help\addchapter.sce; DestDir: {app}\modules\{#DOUBLE}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
;Source: modules\{#DOUBLE}\includes\*.h; DestDir: {app}\modules\{#DOUBLE}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#DOUBLE}\macros\buildmacros.sce; DestDir: {app}\modules\{#DOUBLE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DOUBLE}\macros\buildmacros.bat; DestDir: {app}\modules\{#DOUBLE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DOUBLE}\macros\cleanmacros.bat; DestDir: {app}\modules\{#DOUBLE}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#DOUBLE}\macros\lib; DestDir: {app}\modules\{#DOUBLE}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#DOUBLE}\macros\names; DestDir: {app}\modules\{#DOUBLE}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#DOUBLE}\macros\*.bin; DestDir: {app}\modules\{#DOUBLE}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#DOUBLE}\macros\*.sci; DestDir: {app}\modules\{#DOUBLE}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#DOUBLE}\demos\*.*; DestDir: {app}\modules\{#DOUBLE}\demos; Flags: recursesubdirs;  Components: {#COMPN_SCILAB}
;
Source: modules\{#DOUBLE}\tests\*.*; DestDir: {app}\modules\{#DOUBLE}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
