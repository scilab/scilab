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
; polynomials module
;--------------------------------------------------------------------------------------------------------------
;
#define POLYNOMIALS "polynomials"
;
Source: bin\polynomials.dll; DestDir: {app}\bin; Components: {#COMPN_POLY}
Source: bin\polynomials_f.dll; DestDir: {app}\bin; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\VERSION.xml; DestDir: {app}\modules\{#POLYNOMIALS}; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\readme.txt; DestDir: {app}\modules\{#POLYNOMIALS}; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\license.txt; DestDir: {app}\modules\{#POLYNOMIALS}; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\changelog.txt; DestDir: {app}\modules\{#POLYNOMIALS}; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\sci_gateway\polynomials_gateway.xml; DestDir: {app}\modules\{#POLYNOMIALS}\sci_gateway; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\etc\{#POLYNOMIALS}.quit; DestDir: {app}\modules\{#POLYNOMIALS}\etc; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\etc\{#POLYNOMIALS}.start; DestDir: {app}\modules\{#POLYNOMIALS}\etc; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\help\addchapter.sce; DestDir: {app}\modules\{#POLYNOMIALS}\help; Flags: recursesubdirs; Components:  {#COMPN_POLY}
;
;Source: modules\{#POLYNOMIALS}\includes\*.h; DestDir: {app}\modules\{#POLYNOMIALS}\includes; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\macros\buildmacros.sce; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\macros\buildmacros.bat; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\macros\lib; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\macros\names; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\macros\*.bin; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\macros\*.sci; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\demos\*.*; DestDir: {app}\modules\{#POLYNOMIALS}\demos; Flags: recursesubdirs; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\tests\*.*; DestDir: {app}\modules\{#POLYNOMIALS}\tests; Flags: recursesubdirs; Components: {#COMPN_POLY} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
