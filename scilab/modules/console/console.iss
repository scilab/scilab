;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) INRIA - 2007 - Vincent COUVERT
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
; console module
;--------------------------------------------------------------------------------------------------------------
;
#define CONSOLE "console"
;
Source: bin\sci{#CONSOLE}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: bin\sci{#CONSOLE}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: bin\{#CONSOLE}_gw.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\no{#CONSOLE}.dll; DestDir: {app}\bin; DestName: sci{#CONSOLE}.dll; Components: {#COMPN_SCILAB} and not ({#COMPN_JVM_MODULE})
Source: bin\no{#CONSOLE}.lib; DestDir: {app}\bin; DestName: sci{#CONSOLE}.lib; Components: {#COMPN_SCILAB} and not ({#COMPN_JVM_MODULE})
;
Source: modules\{#CONSOLE}\jar\org.scilab.modules.{#CONSOLE}.jar;DestDir: {app}\modules\{#CONSOLE}\jar; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\jrosetta-engine.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: thirdparty\jrosetta-API.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#CONSOLE}\etc\configuration.xml; DestDir: {app}\modules\{#CONSOLE}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;

Source: modules\{#CONSOLE}\license.txt; DestDir: {app}\modules\{#CONSOLE}; Components: {#COMPN_SCILAB}
;
Source: modules\{#CONSOLE}\sci_gateway\{#CONSOLE}_gateway.xml; DestDir: {app}\modules\{#CONSOLE}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#CONSOLE}\includes\*.h; DestDir: {app}\modules\{#CONSOLE}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#CONSOLE}\etc\{#CONSOLE}.quit; DestDir: {app}\modules\{#CONSOLE}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#CONSOLE}\etc\{#CONSOLE}.start; DestDir: {app}\modules\{#CONSOLE}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#CONSOLE}\etc\*.xml; DestDir: {app}\modules\{#CONSOLE}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#CONSOLE}\etc\*.xsl; DestDir: {app}\modules\{#CONSOLE}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#CONSOLE}\macros\buildmacros.sce; DestDir: {app}\modules\{#CONSOLE}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#CONSOLE}\macros\buildmacros.bat; DestDir: {app}\modules\{#CONSOLE}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#CONSOLE}\macros\cleanmacros.bat; DestDir: {app}\modules\{#CONSOLE}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
;Source: modules\{#CONSOLE}\demos\*.*; DestDir: {app}\modules\{#CONSOLE}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#CONSOLE}\tests\*.*; DestDir: {app}\modules\{#CONSOLE}\tests; Flags: recursesubdirs; Components:  {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
