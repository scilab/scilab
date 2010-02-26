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
; randlib module
;--------------------------------------------------------------------------------------------------------------
;
#define RANDLIB "randlib"
;
Source: bin\norandlib.dll; DestDir: {app}\bin; DestName: randlib.dll; Components: not ({#COMPN_RANDLIB})
;
Source: bin\randlib.dll; DestDir: {app}\bin; Components: {#COMPN_RANDLIB}
Source: bin\randlib_f.dll; DestDir: {app}\bin; Components: {#COMPN_RANDLIB}
;
Source: modules\{#RANDLIB}\VERSION.xml; DestDir: {app}\modules\{#RANDLIB}; Components: {#COMPN_RANDLIB}
Source: modules\{#RANDLIB}\readme.txt; DestDir: {app}\modules\{#RANDLIB}; Components: {#COMPN_RANDLIB}
Source: modules\{#RANDLIB}\license.txt; DestDir: {app}\modules\{#RANDLIB}; Components: {#COMPN_RANDLIB}
Source: modules\{#RANDLIB}\changelog.txt; DestDir: {app}\modules\{#RANDLIB}; Components: {#COMPN_RANDLIB}
;
Source: modules\{#RANDLIB}\sci_gateway\randlib_gateway.xml; DestDir: {app}\modules\{#RANDLIB}\sci_gateway; Components: {#COMPN_RANDLIB}
;
Source: modules\{#RANDLIB}\etc\{#RANDLIB}.quit; DestDir: {app}\modules\{#RANDLIB}\etc; Components: {#COMPN_RANDLIB}
Source: modules\{#RANDLIB}\etc\{#RANDLIB}.start; DestDir: {app}\modules\{#RANDLIB}\etc; Components: {#COMPN_RANDLIB}
;
Source: modules\{#RANDLIB}\help\addchapter.sce; DestDir: {app}\modules\{#RANDLIB}\help; Flags: recursesubdirs; Components:  {#COMPN_RANDLIB}
;
;Source: modules\{#RANDLIB}\includes\*.h; DestDir: {app}\modules\{#RANDLIB}\includes; Components: {#COMPN_RANDLIB}
;
;Source: modules\{#RANDLIB}\macros\lib; DestDir: {app}\modules\{#RANDLIB}\macros; Flags: recursesubdirs; Components: {#COMPN_RANDLIB}
;Source: modules\{#RANDLIB}\macros\names; DestDir: {app}\modules\{#RANDLIB}\macros; Flags: recursesubdirs; Components: {#COMPN_RANDLIB}
;Source: modules\{#RANDLIB}\macros\*.bin; DestDir: {app}\modules\{#RANDLIB}\macros; Flags: recursesubdirs; Components: {#COMPN_RANDLIB}
;Source: modules\{#RANDLIB}\macros\*.sci; DestDir: {app}\modules\{#RANDLIB}\macros; Flags: recursesubdirs; Components: {#COMPN_RANDLIB}
Source: modules\{#RANDLIB}\macros\buildmacros.sce; DestDir: {app}\modules\{#RANDLIB}\macros; Flags: recursesubdirs; Components: {#COMPN_RANDLIB}
Source: modules\{#RANDLIB}\macros\buildmacros.bat; DestDir: {app}\modules\{#RANDLIB}\macros; Flags: recursesubdirs; Components: {#COMPN_RANDLIB}
Source: modules\{#RANDLIB}\macros\cleanmacros.bat; DestDir: {app}\modules\{#RANDLIB}\macros; Flags: recursesubdirs; Components: {#COMPN_RANDLIB}
;
Source: modules\{#RANDLIB}\demos\*.*; DestDir: {app}\modules\{#RANDLIB}\demos; Flags: recursesubdirs; Components: {#COMPN_RANDLIB}
;
Source: modules\{#RANDLIB}\tests\*.*; DestDir: {app}\modules\{#RANDLIB}\tests; Flags: recursesubdirs; Components: {#COMPN_RANDLIB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
