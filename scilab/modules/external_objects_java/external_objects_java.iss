;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) SCilab Enterprises - 2012 - Antoine ELIAS
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
; EXTERNAL_OBJECTS_JAVA module
;--------------------------------------------------------------------------------------------------------------
;
#define EXTERNAL_OBJECTS_JAVA "external_objects_java"
;
Source: bin\{#EXTERNAL_OBJECTS_JAVA}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: bin\{#EXTERNAL_OBJECTS_JAVA}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: thirdparty\ecj.jar; DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\jar\org.scilab.modules.{#EXTERNAL_OBJECTS_JAVA}.jar;DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\jar; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\sci_gateway\{#EXTERNAL_OBJECTS_JAVA}_gateway.xml; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\sci_gateway; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\includes\*.h; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\includes; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\etc\{#EXTERNAL_OBJECTS_JAVA}.quit; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\etc\{#EXTERNAL_OBJECTS_JAVA}.start; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\macros\buildmacros.sce; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\macros\buildmacros.bat; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\macros\cleanmacros.bat; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\tests\*.*; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE} and {#COMPN_TESTS}
;
Source: modules\{#EXTERNAL_OBJECTS_JAVA}\examples\com\foo\*.java; DestDir: {app}\modules\{#EXTERNAL_OBJECTS_JAVA}\examples\com\foo; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE} and {#COMPN_TESTS}

;--------------------------------------------------------------------------------------------------------------
