;
; Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
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
; gui module
;--------------------------------------------------------------------------------------------------------------
;
#define GUI "gui"
;
Source: bin\sci{#GUI}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: bin\no{#GUI}.dll; DestDir: {app}\bin; DestName: sci{#GUI}.dll; Components: not( {#COMPN_JVM_MODULE} )
Source: bin\no{#GUI}.dll; DestDir: {app}\bin; DestName: {#GUI}_gw.dll; Components: not( {#COMPN_JVM_MODULE} )
;

Source: modules\{#GUI}\jar\org.scilab.modules.{#GUI}.jar; DestDir: {app}\modules\{#GUI}\jar; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: thirdparty\jcef-146.0.10.jar; DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\gson-2.10.1.jar; DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\flexdock-1.2.5.jar; DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\commons-logging-1.1.1.jar; DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\jgoodies-looks-2.7.0.jar; DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\skinlf-1.2.3.jar; DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\javafx-base-21.0.11.jar; DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\javafx-graphics-21.0.11.jar; DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\javafx-swing-21.0.11.jar; DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\web\*.*; DestDir: {app}\thirdparty\web; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#GUI}\license.txt; DestDir: {app}\modules\{#GUI}; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#GUI}\sci_gateway\{#GUI}_gateway.xml; DestDir: {app}\modules\{#GUI}\sci_gateway; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#GUI}\etc\{#GUI}.quit; DestDir: {app}\modules\{#GUI}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#GUI}\etc\{#GUI}.start; DestDir: {app}\modules\{#GUI}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#GUI}\etc\*.xml; DestDir: {app}\modules\{#GUI}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#GUI}\etc\*.xsl; DestDir: {app}\modules\{#GUI}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#GUI}\etc\ScilabBrowser.js; DestDir: {app}\modules\{#GUI}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#GUI}\etc\error.html; DestDir: {app}\modules\{#GUI}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#GUI}\images\icons\*.png; DestDir: {app}\modules\{#GUI}\images\icons; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#GUI}\images\preferences\*.png; DestDir: {app}\modules\{#GUI}\images\preferences; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
;Source: modules\{#GUI}\includes\*.h; DestDir: {app}\modules\{#GUI}\includes; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#GUI}\macros\lib; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#GUI}\macros\*.sci; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#GUI}\macros\*.bin; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#GUI}\macros\buildmacros.sce; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#GUI}\macros\buildmacros.bat; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#GUI}\macros\cleanmacros.bat; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#GUI}\demos\*.*; DestDir: {app}\modules\{#GUI}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#GUI}\tests\*.*; DestDir: {app}\modules\{#GUI}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS} and {#COMPN_JVM_MODULE}
;
Source: modules\{#GUI}\help\*.*; DestDir: {app}\modules\{#GUI}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;--------------------------------------------------------------------------------------------------------------
