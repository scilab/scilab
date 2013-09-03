;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) SCilab Enterprises - 2012 - Antoine ELIAS
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
; EXTERNAL_OBJECTS_JAVA module
;--------------------------------------------------------------------------------------------------------------
;
#define EXTERNAL_OBJECTS_JAVA_JAVA "EXTERNAL_OBJECTS_JAVA_java"
;
Source: bin\EXTERNAL_OBJECTS_JAVA.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: bin\EXTERNAL_OBJECTS_JAVA.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\sci_gateway\{#EXTERNAL_OBJECTS_JAVA}_gateway.xml; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\sci_gateway; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\includes\*.h; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\includes; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\includes\*.hxx; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\includes; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\etc\{#EXTERNAL_OBJECTS_JAVA}.quit; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\etc\{#EXTERNAL_OBJECTS_JAVA}.start; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\macros\buildmacros.sce; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\macros\buildmacros.bat; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\macros\cleanmacros.bat; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\macros\*.bin; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\macros\*.sci; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\macros\lib; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\macros\names; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\macros\lib; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\macros\names; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\macros\*.sci; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\macros\*.bin; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\tests\*.*; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE} and {#COMPN_TESTS}

;--------------------------------------------------------------------------------------------------------------
