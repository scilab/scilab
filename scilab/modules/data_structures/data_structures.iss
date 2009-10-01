; Scilab ( http://mwww.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2007-2008 - INRIA - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Version TRUNK
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; data_structures module
;--------------------------------------------------------------------------------------------------------------
;
#define DATASTRUCT "data_structures"
;
Source: modules\{#DATASTRUCT}\VERSION.xml; DestDir: {app}\modules\{#DATASTRUCT}; Components: {#COMPN_SCILAB}
Source: modules\{#DATASTRUCT}\readme.txt; DestDir: {app}\modules\{#DATASTRUCT}; Components: {#COMPN_SCILAB}
Source: modules\{#DATASTRUCT}\license.txt; DestDir: {app}\modules\{#DATASTRUCT}; Components: {#COMPN_SCILAB}
Source: modules\{#DATASTRUCT}\changelog.txt; DestDir: {app}\modules\{#DATASTRUCT}; Components: {#COMPN_SCILAB}
;
Source: modules\{#DATASTRUCT}\sci_gateway\data_structures_gateway.xml; DestDir: {app}\modules\{#DATASTRUCT}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#DATASTRUCT}\etc\{#DATASTRUCT}.quit; DestDir: {app}\modules\{#DATASTRUCT}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#DATASTRUCT}\etc\{#DATASTRUCT}.start; DestDir: {app}\modules\{#DATASTRUCT}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#DATASTRUCT}\help\addchapter.sce; DestDir: {app}\modules\{#DATASTRUCT}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
;Source: modules\{#DATASTRUCT}\includes\*.h; DestDir: {app}\modules\{#DATASTRUCT}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#DATASTRUCT}\macros\lib; DestDir: {app}\modules\{#DATASTRUCT}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DATASTRUCT}\macros\names; DestDir: {app}\modules\{#DATASTRUCT}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DATASTRUCT}\macros\*.bin; DestDir: {app}\modules\{#DATASTRUCT}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DATASTRUCT}\macros\*.sci; DestDir: {app}\modules\{#DATASTRUCT}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DATASTRUCT}\macros\buildmacros.sce; DestDir: {app}\modules\{#DATASTRUCT}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DATASTRUCT}\macros\buildmacros.bat; DestDir: {app}\modules\{#DATASTRUCT}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DATASTRUCT}\macros\cleanmacros.bat; DestDir: {app}\modules\{#DATASTRUCT}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#DATASTRUCT}\demos\*.*; DestDir: {app}\modules\{#DATASTRUCT}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#DATASTRUCT}\tests\*.*; DestDir: {app}\modules\{#DATASTRUCT}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
