;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2010 - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;
;--------------------------------------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;--------------------------------------------------------------------------------------------------------------
; graph module
;--------------------------------------------------------------------------------------------------------------
;
#define GRAPH "graph"
;
Source: thirdparty\jgraphx.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#GRAPH}\jar\org.scilab.modules.{#GRAPH}.jar;DestDir: {app}\modules\{#GRAPH}\jar; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#GRAPH}\VERSION.xml; DestDir: {app}\modules\{#GRAPH}; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#GRAPH}\license.txt; DestDir: {app}\modules\{#GRAPH}; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#GRAPH}\etc\{#GRAPH}.quit; DestDir: {app}\modules\{#GRAPH}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#GRAPH}\etc\{#GRAPH}.start; DestDir: {app}\modules\{#GRAPH}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#GRAPH}\macros\buildmacros.sce; DestDir: {app}\modules\{#GRAPH}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#GRAPH}\macros\buildmacros.bat; DestDir: {app}\modules\{#GRAPH}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#GRAPH}\macros\cleanmacros.bat; DestDir: {app}\modules\{#GRAPH}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
;Source: modules\{#GRAPH}\tests\*.*; DestDir: {app}\modules\{#GRAPH}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS} and {#COMPN_JVM_MODULE}
;
;
;--------------------------------------------------------------------------------------------------------------
