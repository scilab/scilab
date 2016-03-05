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
