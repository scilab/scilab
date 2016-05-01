;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2010 - Allan CORNET
;
; Copyright (C) 2012 - 2016 - Scilab Enterprises
;
; This file is hereby licensed under the terms of the GNU GPL v2.0,
; pursuant to article 5.3.4 of the CeCILL v.2.1.
; This file was originally licensed under the terms of the CeCILL v2.1,
; and continues to be available under such terms.
; For more information, see the COPYING file which you should have received
; along with this program.
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
