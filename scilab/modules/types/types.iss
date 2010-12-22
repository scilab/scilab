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
; types module
;--------------------------------------------------------------------------------------------------------------
;
#define TYPES "types"
;
Source: modules\{#TYPES}\jar\org.scilab.modules.{#TYPES}.jar;DestDir: {app}\modules\{#TYPES}\jar; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#TYPES}\VERSION.xml; DestDir: {app}\modules\{#TYPES}; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#TYPES}\readme.txt; DestDir: {app}\modules\{#TYPES}; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#TYPES}\license.txt; DestDir: {app}\modules\{#TYPES}; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#TYPES}\changelog.txt; DestDir: {app}\modules\{#TYPES}; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#TYPES}\etc\{#TYPES}.quit; DestDir: {app}\modules\{#TYPES}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#TYPES}\etc\{#TYPES}.start; DestDir: {app}\modules\{#TYPES}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#TYPES}\macros\buildmacros.sce; DestDir: {app}\modules\{#TYPES}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#TYPES}\macros\buildmacros.bat; DestDir: {app}\modules\{#TYPES}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#TYPES}\macros\cleanmacros.bat; DestDir: {app}\modules\{#TYPES}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
;Source: modules\{#TYPES}\tests\*.*; DestDir: {app}\modules\{#TYPES}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS} and {#COMPN_JVM_MODULE}
;
;
;--------------------------------------------------------------------------------------------------------------
