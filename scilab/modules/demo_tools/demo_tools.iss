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
; demo_tools module
;--------------------------------------------------------------------------------------------------------------
;
#define DEMO_TOOLS "demo_tools"
;
Source: modules\{#DEMO_TOOLS}\VERSION.xml; DestDir: {app}\modules\{#DEMO_TOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\readme.txt; DestDir: {app}\modules\{#DEMO_TOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\license.txt; DestDir: {app}\modules\{#DEMO_TOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\changelog.txt; DestDir: {app}\modules\{#DEMO_TOOLS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#DEMO_TOOLS}\etc\{#DEMO_TOOLS}.quit; DestDir: {app}\modules\{#DEMO_TOOLS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\etc\{#DEMO_TOOLS}.start; DestDir: {app}\modules\{#DEMO_TOOLS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#DEMO_TOOLS}\images\*.*; DestDir: {app}\modules\{#DEMO_TOOLS}\images; Components: {#COMPN_SCILAB}
;
Source: modules\{#DEMO_TOOLS}\help\addchapter.sce; DestDir: {app}\modules\{#DEMO_TOOLS}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
Source: modules\{#DEMO_TOOLS}\macros\buildmacros.sce; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\macros\buildmacros.bat; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\macros\lib; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\macros\names; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\macros\*.bin; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\macros\*.sci; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#DEMO_TOOLS}\examples\*.*; DestDir: {app}\modules\{#DEMO_TOOLS}\examples; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#DEMO_TOOLS}\tests\*.*; DestDir: {app}\modules\{#DEMO_TOOLS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
