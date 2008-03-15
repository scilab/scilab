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
; helptools module
;--------------------------------------------------------------------------------------------------------------
;
#define HELPTOOLS "helptools"
;
Source: modules\{#HELPTOOLS}\VERSION.xml; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\readme.txt; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\license.txt; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\changelog.txt; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
;
;Source: modules\{#HELPTOOLS}\*.htm; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
;Source: modules\{#HELPTOOLS}\*.xsl; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
;Source: modules\{#HELPTOOLS}\help.dtd; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\css\*.*; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\doc\*.*; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\docbook_xsl\*.*; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\schema\*.*; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\xsl\*.*; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\etc\{#HELPTOOLS}.quit; DestDir: {app}\modules\{#HELPTOOLS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\etc\{#HELPTOOLS}.start; DestDir: {app}\modules\{#HELPTOOLS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\help\addchapter.sce; DestDir: {app}\modules\{#HELPTOOLS}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
;
;Source: modules\{#HELPTOOLS}\help\*.htm; DestDir: {app}\modules\{#HELPTOOLS}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\macros\buildmacros.sce; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\macros\buildmacros.bat; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\macros\lib; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\macros\names; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\macros\*.bin; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\macros\*.sci; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\bin\*.*; DestDir: {app}\modules\{#HELPTOOLS}\bin\sablotron; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\examples\*.*; DestDir: {app}\modules\{#HELPTOOLS}\examples; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\tests\*.*; DestDir: {app}\modules\{#HELPTOOLS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
