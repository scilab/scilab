;##############################################################################################################
; Scilab ( http://mwww.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2010 - DIGITEO - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; ui_data module
;--------------------------------------------------------------------------------------------------------------
;
#define UI_DATA "ui_data"
;
Source: bin\sci{#UI_DATA}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: bin\no{#UI_DATA}.dll; DestDir: {app}\bin; DestName: sci{#UI_DATA}.dll; Components: {#COMPN_SCILAB} and not ({#COMPN_JVM_MODULE})
Source: modules\{#UI_DATA}\jar\org.scilab.modules.{#UI_DATA}.jar;DestDir: {app}\modules\{#UI_DATA}\jar; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#UI_DATA}\etc\{#UI_DATA}.start;DestDir: {app}\modules\{#UI_DATA}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#UI_DATA}\etc\{#UI_DATA}.quit;DestDir: {app}\modules\{#UI_DATA}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#UI_DATA}\etc\*.xml;DestDir: {app}\modules\{#UI_DATA}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#UI_DATA}\etc\*.xsl;DestDir: {app}\modules\{#UI_DATA}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#UI_DATA}\sci_gateway\{#UI_DATA}_gateway.xml; DestDir: {app}\modules\{#UI_DATA}\sci_gateway; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;

Source: modules\{#UI_DATA}\license.txt; DestDir: {app}\modules\{#UI_DATA}; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#UI_DATA}\images\icons\*.png; DestDir: {app}\modules\{#UI_DATA}\images\icons; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
;Source: modules\{#UI_DATA}\demos\*.*; DestDir: {app}\modules\{#UI_DATA}\demos; Flags: recursesubdirs;  Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#UI_DATA}\tests\*.*; DestDir: {app}\modules\{#UI_DATA}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS} and {#COMPN_JVM_MODULE}
;
;--------------------------------------------------------------------------------------------------------------
