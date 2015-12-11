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
; history_browser module
;--------------------------------------------------------------------------------------------------------------
;
#define HISTORY_BROWSER "history_browser"
;
Source: bin\{#HISTORY_BROWSER}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: bin\no{#HISTORY_BROWSER}.dll; DestDir: {app}\bin; DestName: {#HISTORY_BROWSER}.dll; Components: not ({#COMPN_JVM_MODULE})
;
Source: modules\{#HISTORY_BROWSER}\jar\org.scilab.modules.{#HISTORY_BROWSER}.jar;DestDir: {app}\modules\{#HISTORY_BROWSER}\jar; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#HISTORY_BROWSER}\license.txt; DestDir: {app}\modules\{#HISTORY_BROWSER}; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#HISTORY_BROWSER}\etc\{#HISTORY_BROWSER}.quit; DestDir: {app}\modules\{#HISTORY_BROWSER}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#HISTORY_BROWSER}\etc\{#HISTORY_BROWSER}.start; DestDir: {app}\modules\{#HISTORY_BROWSER}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#HISTORY_BROWSER}\macros\buildmacros.sce; DestDir: {app}\modules\{#HISTORY_BROWSER}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#HISTORY_BROWSER}\macros\buildmacros.bat; DestDir: {app}\modules\{#HISTORY_BROWSER}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#HISTORY_BROWSER}\macros\cleanmacros.bat; DestDir: {app}\modules\{#HISTORY_BROWSER}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#HISTORY_BROWSER}\sci_gateway\{#HISTORY_BROWSER}_gateway.xml; DestDir: {app}\modules\{#HISTORY_BROWSER}\sci_gateway; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#HISTORY_BROWSER}\tests\*.*; DestDir: {app}\modules\{#HISTORY_BROWSER}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS} and {#COMPN_JVM_MODULE}
;
;
;--------------------------------------------------------------------------------------------------------------
