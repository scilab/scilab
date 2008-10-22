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
; scilab2fortran module
;--------------------------------------------------------------------------------------------------------------
;
#define SCI2FOR "scilab2fortran"
;
Source: modules\{#SCI2FOR}\VERSION.xml; DestDir: {app}\modules\{#SCI2FOR}; Components: {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\readme.txt; DestDir: {app}\modules\{#SCI2FOR}; Components: {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\license.txt; DestDir: {app}\modules\{#SCI2FOR}; Components: {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\changelog.txt; DestDir: {app}\modules\{#SCI2FOR}; Components: {#COMPN_SCI2FOR}
;
Source: modules\{#SCI2FOR}\etc\{#SCI2FOR}.quit; DestDir: {app}\modules\{#SCI2FOR}\etc; Components: {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\etc\{#SCI2FOR}.start; DestDir: {app}\modules\{#SCI2FOR}\etc; Components: {#COMPN_SCI2FOR}
;
Source: modules\{#SCI2FOR}\help\addchapter.sce; DestDir: {app}\modules\{#SCI2FOR}\help; Flags: recursesubdirs; Components:  {#COMPN_SCI2FOR}
;
;
Source: modules\{#SCI2FOR}\macros\lib; DestDir: {app}\modules\{#SCI2FOR}\macros; Components: {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\macros\names; DestDir: {app}\modules\{#SCI2FOR}\macros; Components: {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\macros\*.bin; DestDir: {app}\modules\{#SCI2FOR}\macros; Components: {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\macros\*.sci; DestDir: {app}\modules\{#SCI2FOR}\macros; Components: {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\macros\buildmacros.sce; DestDir: {app}\modules\{#SCI2FOR}\macros; Components: {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\macros\buildmacros.bat; DestDir: {app}\modules\{#SCI2FOR}\macros; Components: {#COMPN_SCI2FOR}
Source: modules\{#SCI2FOR}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SCI2FOR}\macros; Components: {#COMPN_SCI2FOR}
;
Source: modules\{#SCI2FOR}\demos\*.*; DestDir: {app}\modules\{#SCI2FOR}\demos; Flags: recursesubdirs; Components: {#COMPN_SCI2FOR}
;
;Source: modules\{#SCI2FOR}\tests\*.*; DestDir: {app}\modules\{#SCI2FOR}\tests; Flags: recursesubdirs; Components:{#COMPN_SCI2FOR} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
