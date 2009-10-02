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
; linear_algebra module
;--------------------------------------------------------------------------------------------------------------
;
#define LINALG "linear_algebra"
;
Source: bin\linear_algebra.dll; DestDir: {app}\bin; Components: {#COMPN_LINALG}
Source: bin\linear_algebra_f.dll; DestDir: {app}\bin; Components: {#COMPN_LINALG}
;
Source: modules\{#LINALG}\VERSION.xml; DestDir: {app}\modules\{#LINALG}; Components: {#COMPN_LINALG}
Source: modules\{#LINALG}\readme.txt; DestDir: {app}\modules\{#LINALG}; Components: {#COMPN_LINALG}
Source: modules\{#LINALG}\license.txt; DestDir: {app}\modules\{#LINALG}; Components: {#COMPN_LINALG}
Source: modules\{#LINALG}\changelog.txt; DestDir: {app}\modules\{#LINALG}; Components: {#COMPN_LINALG}
;
Source: modules\{#LINALG}\sci_gateway\linear_algebra_gateway.xml; DestDir: {app}\modules\{#LINALG}\sci_gateway; Components: {#COMPN_LINALG}
;
Source: modules\{#LINALG}\etc\{#LINALG}.quit; DestDir: {app}\modules\{#LINALG}\etc; Components: {#COMPN_LINALG}
Source: modules\{#LINALG}\etc\{#LINALG}.start; DestDir: {app}\modules\{#LINALG}\etc; Components: {#COMPN_LINALG}
;
Source: modules\{#LINALG}\help\addchapter.sce; DestDir: {app}\modules\{#LINALG}\help; Flags: recursesubdirs; Components:  {#COMPN_LINALG}
;
;Source: modules\{#LINALG}\includes\*.h; DestDir: {app}\modules\{#LINALG}\includes; Components: {#COMPN_LINALG}
;
Source: modules\{#LINALG}\macros\buildmacros.sce; DestDir: {app}\modules\{#LINALG}\macros; Components: {#COMPN_LINALG}
Source: modules\{#LINALG}\macros\buildmacros.bat; DestDir: {app}\modules\{#LINALG}\macros; Components: {#COMPN_LINALG}
Source: modules\{#LINALG}\macros\cleanmacros.bat; DestDir: {app}\modules\{#LINALG}\macros; Components: {#COMPN_LINALG}
Source: modules\{#LINALG}\macros\lib; DestDir: {app}\modules\{#LINALG}\macros; Components: {#COMPN_LINALG}
Source: modules\{#LINALG}\macros\names; DestDir: {app}\modules\{#LINALG}\macros; Components: {#COMPN_LINALG}
Source: modules\{#LINALG}\macros\*.bin; DestDir: {app}\modules\{#LINALG}\macros; Components: {#COMPN_LINALG}
Source: modules\{#LINALG}\macros\*.sci; DestDir: {app}\modules\{#LINALG}\macros; Components: {#COMPN_LINALG}
;
;Source: modules\{#LINALG}\demos\*.*; DestDir: {app}\modules\{#LINALG}\demos; Flags: recursesubdirs;  Components: {#COMPN_LINALG}
;
Source: modules\{#LINALG}\examples\*.*; DestDir: {app}\modules\{#LINALG}\examples; Flags: recursesubdirs;  Components: {#COMPN_LINALG}
;
Source: modules\{#LINALG}\tests\*.*; DestDir: {app}\modules\{#LINALG}\tests; Flags: recursesubdirs; Components: {#COMPN_LINALG} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
