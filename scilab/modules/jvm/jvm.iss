;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; jvm module
;--------------------------------------------------------------------------------------------------------------
;
#DEFINE JVM "jvm"
;
Source: bin\libjvm.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\libjvm.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: etc\classpath.xml; DestDir: {app}\etc; Components: {#COMPN_SCILAB}
Source: etc\librarypath.xml; DestDir: {app}\etc; Components: {#COMPN_SCILAB}
Source: etc\jvm_options.xml; DestDir: {app}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#JVM}\jar\org.scilab.modules.jvm.jar;DestDir: {app}\modules\{#JVM}\jar; Components: {#COMPN_SCILAB}
;
Source: modules\{#JVM}\license.txt; DestDir: {app}\modules\{#JVM}; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\readme.txt; DestDir: {app}\modules\{#JVM}; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\VERSION.xml; DestDir: {app}\modules\{#JVM}; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\changelog.txt; DestDir: {app}\modules\{#JVM}; Components: {#COMPN_SCILAB}
;
Source: modules\{#JVM}\sci_gateway\jvm_gateway.xml; DestDir: {app}\modules\{#JVM}\sci_gateway; Components: {#COMPN_SCILAB}
;
;Source: modules\{#JVM}\includes\*.h; DestDir: {app}\modules\{#JVM}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\includes\getScilabJavaVM.h; DestDir: {app}\modules\{#JVM}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\includes\getScilabJNIEnv.h; DestDir: {app}\modules\{#JVM}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\includes\addToClasspath.h; DestDir: {app}\modules\{#JVM}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\includes\addToLibrarypath.h; DestDir: {app}\modules\{#JVM}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\includes\getScilabObject.h; DestDir: {app}\modules\{#JVM}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\includes\dynlib_jvm.h; DestDir: {app}\modules\{#JVM}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#JVM}\etc\{#JVM}.quit; DestDir: {app}\modules\{#JVM}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\etc\{#JVM}.start; DestDir: {app}\modules\{#JVM}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#JVM}\macros\buildmacros.sce; DestDir: {app}\modules\{#JVM}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\macros\buildmacros.bat; DestDir: {app}\modules\{#JVM}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\macros\cleanmacros.bat; DestDir: {app}\modules\{#JVM}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\macros\lib; DestDir: {app}\modules\{#JVM}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\macros\names; DestDir: {app}\modules\{#JVM}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\macros\*.bin; DestDir: {app}\modules\{#JVM}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\macros\*.sci; DestDir: {app}\modules\{#JVM}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#JVM}\help\addchapter.sce; DestDir: {app}\modules\{#JVM}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
Source: modules\{#JVM}\tests\*.*; DestDir: {app}\modules\{#JVM}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
