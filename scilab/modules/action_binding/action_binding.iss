;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - 2007 -  Allan CORNET
; Copyright (C) INRIA - 2008 - Vincent COUVERT <vincent.couvert@inria.fr>
; Copyright (C) DIGITEO - 2010 - Allan CORNET
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
; action_binding module
;--------------------------------------------------------------------------------------------------------------
;
#define ACTION_BINDING "action_binding"
;
Source: bin\sciaction_binding.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#ACTION_BINDING}\jar\org.scilab.modules.{#ACTION_BINDING}.jar;DestDir: {app}\modules\{#ACTION_BINDING}\jar; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
;Source: bin\no{#ACTION_BINDING}.dll; DestDir: {app}\bin; DestName: sci{#ACTION_BINDING}.dll; Components: not ({#COMPN_ACTION_BINDING})
;
Source: modules\{#ACTION_BINDING}\license.txt; DestDir: {app}\modules\{#ACTION_BINDING}; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#ACTION_BINDING}\version.xml; DestDir: {app}\modules\{#ACTION_BINDING}; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#ACTION_BINDING}\sci_gateway\{#ACTION_BINDING}_gateway.xml; DestDir: {app}\modules\{#ACTION_BINDING}\sci_gateway; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
;Source: modules\{#ACTION_BINDING}\includes\*.h; DestDir: {app}\modules\{#ACTION_BINDING}\includes; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#ACTION_BINDING}\etc\{#ACTION_BINDING}.quit; DestDir: {app}\modules\{#ACTION_BINDING}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#ACTION_BINDING}\etc\{#ACTION_BINDING}.start; DestDir: {app}\modules\{#ACTION_BINDING}\etc; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#ACTION_BINDING}\macros\buildmacros.sce; DestDir: {app}\modules\{#ACTION_BINDING}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#ACTION_BINDING}\macros\buildmacros.bat; DestDir: {app}\modules\{#ACTION_BINDING}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#ACTION_BINDING}\macros\cleanmacros.bat; DestDir: {app}\modules\{#ACTION_BINDING}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;Source: modules\{#ACTION_BINDING}\macros\*.bin; DestDir: {app}\modules\{#ACTION_BINDING}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;Source: modules\{#ACTION_BINDING}\macros\*.sci; DestDir: {app}\modules\{#ACTION_BINDING}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;Source: modules\{#ACTION_BINDING}\macros\lib; DestDir: {app}\modules\{#ACTION_BINDING}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;Source: modules\{#ACTION_BINDING}\macros\names; DestDir: {app}\modules\{#ACTION_BINDING}\macros; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
;Source: modules\{#ACTION_BINDING}\demos\*.*; DestDir: {app}\modules\{#ACTION_BINDING}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#ACTION_BINDING}\tests\*.*; DestDir: {app}\modules\{#ACTION_BINDING}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS} and {#COMPN_JVM_MODULE}
;--------------------------------------------------------------------------------------------------------------
