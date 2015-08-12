;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2010 - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
;
;--------------------------------------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;--------------------------------------------------------------------------------------------------------------
; commons module
;--------------------------------------------------------------------------------------------------------------
;
#define COMMONS "commons"
;
Source: modules\{#COMMONS}\jar\org.scilab.modules.{#COMMONS}.jar;DestDir: {app}\modules\{#COMMONS}\jar; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: bin\sci{#COMMONS}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMMONS}\license.txt; DestDir: {app}\modules\{#COMMONS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMMONS}\etc\{#COMMONS}.quit; DestDir: {app}\modules\{#COMMONS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#COMMONS}\etc\{#COMMONS}.start; DestDir: {app}\modules\{#COMMONS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMMONS}\macros\buildmacros.sce; DestDir: {app}\modules\{#COMMONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#COMMONS}\macros\buildmacros.bat; DestDir: {app}\modules\{#COMMONS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#COMMONS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#COMMONS}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#COMMONS}\tests\*.*; DestDir: {app}\modules\{#COMMONS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;
;--------------------------------------------------------------------------------------------------------------
