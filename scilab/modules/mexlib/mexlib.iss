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
; mexlib module
;--------------------------------------------------------------------------------------------------------------
;
#define MEXLIB "mexlib"
;
Source: bin\libmx.dll; DestDir: {app}\bin; Components: {#COMPN_MEXLIB}
Source: bin\libmex.dll; DestDir: {app}\bin; Components: {#COMPN_MEXLIB}
Source: bin\libmat.dll; DestDir: {app}\bin; Components: {#COMPN_MEXLIB}
;
Source: modules\{#MEXLIB}\VERSION.xml; DestDir: {app}\modules\{#MEXLIB}; Components: {#COMPN_MEXLIB}
Source: modules\{#MEXLIB}\readme.txt; DestDir: {app}\modules\{#MEXLIB}; Components: {#COMPN_MEXLIB}
Source: modules\{#MEXLIB}\license.txt; DestDir: {app}\modules\{#MEXLIB}; Components: {#COMPN_MEXLIB}
Source: modules\{#MEXLIB}\changelog.txt; DestDir: {app}\modules\{#MEXLIB}; Components: {#COMPN_MEXLIB}
;
Source: modules\{#MEXLIB}\etc\{#MEXLIB}.quit; DestDir: {app}\modules\{#MEXLIB}\etc; Components: {#COMPN_MEXLIB}
Source: modules\{#MEXLIB}\etc\{#MEXLIB}.start; DestDir: {app}\modules\{#MEXLIB}\etc; Components: {#COMPN_MEXLIB}
;
Source: modules\{#MEXLIB}\help\addchapter.sce; DestDir: {app}\modules\{#MEXLIB}\help; Flags: recursesubdirs; Components:  {#COMPN_MEXLIB}
;
Source: modules\{#MEXLIB}\includes\*.h; DestDir: {app}\modules\{#MEXLIB}\includes; Components: {#COMPN_MEXLIB}
;
;Source: modules\{#MEXLIB}\macros\lib; DestDir: {app}\modules\{#MEXLIB}\macros; Components: {#COMPN_MEXLIB}
;Source: modules\{#MEXLIB}\macros\names; DestDir: {app}\modules\{#MEXLIB}\macros; Components: {#COMPN_MEXLIB}
;Source: modules\{#MEXLIB}\macros\*.bin; DestDir: {app}\modules\{#MEXLIB}\macros; Components: {#COMPN_MEXLIB}
;Source: modules\{#MEXLIB}\macros\*.sci; DestDir: {app}\modules\{#MEXLIB}\macros; Components: {#COMPN_MEXLIB}
Source: modules\{#MEXLIB}\macros\buildmacros.sce; DestDir: {app}\modules\{#MEXLIB}\macros; Components: {#COMPN_MEXLIB}
Source: modules\{#MEXLIB}\macros\buildmacros.bat; DestDir: {app}\modules\{#MEXLIB}\macros; Components: {#COMPN_MEXLIB}
Source: modules\{#MEXLIB}\macros\cleanmacros.bat; DestDir: {app}\modules\{#MEXLIB}\macros; Components: {#COMPN_MEXLIB}
;
;Source: modules\{#MEXLIB}\demos\*.*; DestDir: {app}\modules\{#MEXLIB}\demos; Flags: recursesubdirs; Components: {#COMPN_MEXLIB}
;
Source: modules\{#MEXLIB}\examples\*.*; DestDir: {app}\modules\{#MEXLIB}\examples; Flags: recursesubdirs; Components: {#COMPN_MEXLIB}
;
Source: modules\{#MEXLIB}\tests\*.*; DestDir: {app}\modules\{#MEXLIB}\tests; Flags: recursesubdirs; Components: {#COMPN_MEXLIB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
