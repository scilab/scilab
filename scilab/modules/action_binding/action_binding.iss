;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Vincent COUVERT
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; action_binding module
;--------------------------------------------------------------------------------------------------------------
;
#define ACTION_BINDING "action_binding"
;
Source: bin\sciaction_binding.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#ACTION_BINDING}\jar\org.scilab.modules.action_binding.jar;DestDir: {app}\modules\{#ACTION_BINDING}\jar; Components: {#COMPN_SCILAB}
;
;Source: bin\noaction_binding.dll; DestDir: {app}\bin; DestName: sciaction_binding.dll; Components: not ({#COMPN_ACTION_BINDING})
;
Source: modules\{#ACTION_BINDING}\changelog.txt; DestDir: {app}\modules\{#ACTION_BINDING}; Components: {#COMPN_SCILAB};
Source: modules\{#ACTION_BINDING}\licence.txt; DestDir: {app}\modules\{#ACTION_BINDING}; Components: {#COMPN_SCILAB}
Source: modules\{#ACTION_BINDING}\readme.txt; DestDir: {app}\modules\{#ACTION_BINDING}; Components: {#COMPN_SCILAB}
Source: modules\{#ACTION_BINDING}\version.xml; DestDir: {app}\modules\{#ACTION_BINDING}; Components: {#COMPN_SCILAB}
;
;Source: modules\{#ACTION_BINDING}\sci_gateway\action_binding.xml; DestDir: {app}\modules\{#ACTION_BINDING}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#ACTION_BINDING}\includes\*.h; DestDir: {app}\modules\{#ACTION_BINDING}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#ACTION_BINDING}\help\addchapter.sce; DestDir: {app}\modules\{#ACTION_BINDING}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
Source: modules\{#ACTION_BINDING}\help\removechapter.sce; DestDir: {app}\modules\{#ACTION_BINDING}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#ACTION_BINDING}\etc\{#ACTION_BINDING}.quit; DestDir: {app}\modules\{#ACTION_BINDING}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#ACTION_BINDING}\etc\{#ACTION_BINDING}.start; DestDir: {app}\modules\{#ACTION_BINDING}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#ACTION_BINDING}\help\*.htm; DestDir: {app}\modules\{#ACTION_BINDING}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;
Source: modules\{#ACTION_BINDING}\macros\buildmacros.sce; DestDir: {app}\modules\{#ACTION_BINDING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ACTION_BINDING}\macros\buildmacros.bat; DestDir: {app}\modules\{#ACTION_BINDING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#ACTION_BINDING}\macros\cleanmacros.bat; DestDir: {app}\modules\{#ACTION_BINDING}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#ACTION_BINDING}\macros\*.bin; DestDir: {app}\modules\{#ACTION_BINDING}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#ACTION_BINDING}\macros\*.sci; DestDir: {app}\modules\{#ACTION_BINDING}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#ACTION_BINDING}\macros\lib; DestDir: {app}\modules\{#ACTION_BINDING}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#ACTION_BINDING}\macros\names; DestDir: {app}\modules\{#ACTION_BINDING}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#ACTION_BINDING}\demos\*.*; DestDir: {app}\modules\{#ACTION_BINDING}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;--------------------------------------------------------------------------------------------------------------
