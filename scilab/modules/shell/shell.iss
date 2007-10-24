;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; SHELL module
;--------------------------------------------------------------------------------------------------------------
;
#define SHELL "shell"
;
Source: bin\libshell.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#SHELL}\jar\org.scilab.modules.shell.jar;DestDir: {app}\modules\{#SHELL}\jar; Components: {#COMPN_SCILAB}
;
;
Source: modules\{#SHELL}\changelog.txt; DestDir: {app}\modules\{#SHELL}; Components: {#COMPN_SCILAB};
Source: modules\{#SHELL}\licence.txt; DestDir: {app}\modules\{#SHELL}; Components: {#COMPN_SCILAB}
Source: modules\{#SHELL}\readme.txt; DestDir: {app}\modules\{#SHELL}; Components: {#COMPN_SCILAB}
Source: modules\{#SHELL}\VERSION.xml; DestDir: {app}\modules\{#SHELL}; Components: {#COMPN_SCILAB}
;
Source: modules\{#SHELL}\sci_gateway\shell_gateway.xml; DestDir: {app}\modules\{#SHELL}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#SHELL}\includes\*.h; DestDir: {app}\modules\{#SHELL}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#SHELL}\help\addchapter.sce; DestDir: {app}\modules\{#SHELL}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
Source: modules\{#SHELL}\help\removechapter.sce; DestDir: {app}\modules\{#SHELL}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#SHELL}\etc\{#SHELL}.quit; DestDir: {app}\modules\{#SHELL}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#SHELL}\etc\{#SHELL}.start; DestDir: {app}\modules\{#SHELL}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#SHELL}\help\*.htm; DestDir: {app}\modules\{#SHELL}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;
Source: modules\{#SHELL}\macros\buildmacros.sce; DestDir: {app}\modules\{#SHELL}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SHELL}\macros\buildmacros.bat; DestDir: {app}\modules\{#SHELL}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#SHELL}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SHELL}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#SHELL}\macros\*.bin; DestDir: {app}\modules\{#SHELL}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#SHELL}\macros\*.sci; DestDir: {app}\modules\{#SHELL}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#SHELL}\macros\lib; DestDir: {app}\modules\{#SHELL}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#SHELL}\macros\names; DestDir: {app}\modules\{#SHELL}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#SHELL}\demos\*.*; DestDir: {app}\modules\{#SHELL}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;--------------------------------------------------------------------------------------------------------------
