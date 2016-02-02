;##############################################################################################################
; Scilab ( http://mwww.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2010 - DIGITEO - Allan CORNET
;
; Copyright (C) 2012 - 2016 - Scilab Enterprises
;
; This file is hereby licensed under the terms of the GNU GPL v2.0,
; pursuant to article 5.3.4 of the CeCILL v.2.1.
; This file was originally licensed under the terms of the CeCILL v2.1,
; and continues to be available under such terms.
; For more information, see the COPYING file which you should have received
; along with this program.
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
Source: modules\{#UI_DATA}\etc\*.css;DestDir: {app}\modules\{#UI_DATA}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}

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
