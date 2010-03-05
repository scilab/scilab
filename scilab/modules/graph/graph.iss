;##############################################################################################################
; Scilab ( http://mwww.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2010 - DIGITEO - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; graph module
;--------------------------------------------------------------------------------------------------------------
;
#define GRAPH "graph"
;
Source: modules\{#GRAPH}\jar\org.scilab.modules.graph.jar;DestDir: {app}\modules\{#GRAPH}\jar; Components: {#COMPN_SCILAB}
;
Source: modules\{#GRAPH}\VERSION.xml; DestDir: {app}\modules\{#GRAPH}; Components: {#COMPN_SCILAB}
Source: modules\{#GRAPH}\readme.txt; DestDir: {app}\modules\{#GRAPH}; Components: {#COMPN_SCILAB}
Source: modules\{#GRAPH}\license.txt; DestDir: {app}\modules\{#GRAPH}; Components: {#COMPN_SCILAB}
Source: modules\{#GRAPH}\changelog.txt; DestDir: {app}\modules\{#GRAPH}; Components: {#COMPN_SCILAB}
;
Source: modules\{#GRAPH}\etc\{#GRAPH}.quit; DestDir: {app}\modules\{#GRAPH}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#GRAPH}\etc\{#GRAPH}.start; DestDir: {app}\modules\{#GRAPH}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#GRAPH}\macros\buildmacros.sce; DestDir: {app}\modules\{#GRAPH}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#GRAPH}\macros\buildmacros.bat; DestDir: {app}\modules\{#GRAPH}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#GRAPH}\macros\cleanmacros.bat; DestDir: {app}\modules\{#GRAPH}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#GRAPH}\tests\*.*; DestDir: {app}\modules\{#GRAPH}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;
;--------------------------------------------------------------------------------------------------------------
