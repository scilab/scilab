;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; jvm module
;--------------------------------------------------------------------------------------------------------------
;
#DEFINE JVM "jvm"
;
Source: bin\libjvm.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#JVM}\licence.txt; DestDir: {app}\modules\{#JVM}; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\readme.txt; DestDir: {app}\modules\{#JVM}; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\VERSION.xml; DestDir: {app}\modules\{#JVM}; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\changelog.txt; DestDir: {app}\modules\{#JVM}; Components: {#COMPN_SCILAB}
;
Source: modules\{#JVM}\bin\jre\*.*; DestDir: {app}\modules\{#JVM}\bin\jre; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\bin\SWT\*.*; DestDir: {app}\modules\{#JVM}\bin\SWT; Components: {#COMPN_SCILAB}
;
Source: modules\{#JVM}\classes\*.class; DestDir: {app}\modules\{#JVM}\classes; Components: {#COMPN_SCILAB}
;
Source: modules\{#JVM}\etc\{#JVM}.quit; DestDir: {app}\modules\{#JVM}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#JVM}\etc\{#JVM}.start; DestDir: {app}\modules\{#JVM}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#JVM}\help\addchapter.sce; DestDir: {app}\modules\{#JVM}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
Source: modules\{#JVM}\help\removechapter.sce; DestDir: {app}\modules\{#JVM}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#JVM}\languages\*.xml; DestDir: {app}\modules\{#JVM}\languages; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;--------------------------------------------------------------------------------------------------------------
