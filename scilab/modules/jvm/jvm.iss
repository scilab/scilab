;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
; jvm module
;--------------------------------------------------------------------------------------------------------------
;
#DEFINE JVM "jvm"
;
;
Source: bin\lib{#JVM}.dll; DestDir: {app}\bin; Components: {#COMPN_JVM_MODULE}
Source: bin\lib{#JVM}.lib; DestDir: {app}\bin; Components: {#COMPN_JVM_MODULE}
Source: bin\lib{#JVM}_gw.dll; DestDir: {app}\bin; Components: {#COMPN_JVM_MODULE}
Source: bin\nolib{#JVM}.dll; DestDir: {app}\bin; DestName: lib{#JVM}.dll; Components: not ({#COMPN_JVM_MODULE})
Source: bin\nolib{#JVM}.lib; DestDir: {app}\bin; DestName: lib{#JVM}.lib; Components: not ({#COMPN_JVM_MODULE})
Source: bin\nolib{#JVM}.dll; DestDir: {app}\bin; DestName: lib{#JVM}_gw.dll; Components: not ({#COMPN_JVM_MODULE})
;
Source: etc\classpath.xml; DestDir: {app}\etc; Components: {#COMPN_JVM_MODULE}
Source: etc\librarypath.xml; DestDir: {app}\etc; Components: {#COMPN_JVM_MODULE}
Source: etc\{#JVM}_options.xml; DestDir: {app}\etc; Components: {#COMPN_JVM_MODULE}
;
Source: modules\{#JVM}\xml\*.dtd; DestDir: {app}\modules\{#JVM}\xml; Components: {#COMPN_JVM_MODULE}
;
Source: modules\{#JVM}\jar\org.scilab.modules.{#JVM}.jar;DestDir: {app}\modules\{#JVM}\jar; Components: {#COMPN_JVM_MODULE}
;
Source: modules\{#JVM}\license.txt; DestDir: {app}\modules\{#JVM}; Components: {#COMPN_JVM_MODULE}
;
Source: modules\{#JVM}\sci_gateway\{#JVM}_gateway.xml; DestDir: {app}\modules\{#JVM}\sci_gateway; Components: {#COMPN_JVM_MODULE}
;
;Source: modules\{#JVM}\includes\*.h; DestDir: {app}\modules\{#JVM}\includes; Components: {#COMPN_JVM_MODULE}
Source: modules\{#JVM}\includes\getScilabJavaVM.h; DestDir: {app}\modules\{#JVM}\includes; Components: {#COMPN_JVM_MODULE}
Source: modules\{#JVM}\includes\getScilabJNIEnv.h; DestDir: {app}\modules\{#JVM}\includes; Components: {#COMPN_JVM_MODULE}
Source: modules\{#JVM}\includes\addToClasspath.h; DestDir: {app}\modules\{#JVM}\includes; Components: {#COMPN_JVM_MODULE}
Source: modules\{#JVM}\includes\addToLibrarypath.h; DestDir: {app}\modules\{#JVM}\includes; Components: {#COMPN_JVM_MODULE}
Source: modules\{#JVM}\includes\getScilabObject.h; DestDir: {app}\modules\{#JVM}\includes; Components: {#COMPN_JVM_MODULE}
;
Source: modules\{#JVM}\etc\{#JVM}.quit; DestDir: {app}\modules\{#JVM}\etc; Components: {#COMPN_JVM_MODULE}
Source: modules\{#JVM}\etc\{#JVM}.start; DestDir: {app}\modules\{#JVM}\etc; Components: {#COMPN_JVM_MODULE}
;
Source: modules\{#JVM}\macros\buildmacros.sce; DestDir: {app}\modules\{#JVM}\macros; Components: {#COMPN_JVM_MODULE}
Source: modules\{#JVM}\macros\buildmacros.bat; DestDir: {app}\modules\{#JVM}\macros; Components: {#COMPN_JVM_MODULE}
Source: modules\{#JVM}\macros\cleanmacros.bat; DestDir: {app}\modules\{#JVM}\macros; Components: {#COMPN_JVM_MODULE}
Source: modules\{#JVM}\macros\lib; DestDir: {app}\modules\{#JVM}\macros; Components: {#COMPN_JVM_MODULE}
Source: modules\{#JVM}\macros\*.sci; DestDir: {app}\modules\{#JVM}\macros; Components: {#COMPN_JVM_MODULE}
Source: modules\{#JVM}\macros\*.bin; DestDir: {app}\modules\{#JVM}\macros; Components: {#COMPN_JVM_MODULE}
;
Source: modules\{#JVM}\tests\*.*; DestDir: {app}\modules\{#JVM}\tests; Flags: recursesubdirs; Components: {#COMPN_JVM_MODULE} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
