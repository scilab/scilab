;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; CONSOLE module
;--------------------------------------------------------------------------------------------------------------
;
#define CONSOLE "console"
;
Source: bin\CONSOLE.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
;Source: java\jar\modules\console.jar;DestDir: {app}\java\jar\modules; Components: {#COMPN_SCILAB}
;
;
Source: modules\{#CONSOLE}\changelog.txt; DestDir: {app}\modules\{#CONSOLE}; Components: {#COMPN_SCILAB}; Check: set_with_console
Source: modules\{#CONSOLE}\licence.txt; DestDir: {app}\modules\{#CONSOLE}; Components: {#COMPN_SCILAB}
Source: modules\{#CONSOLE}\readme.txt; DestDir: {app}\modules\{#CONSOLE}; Components: {#COMPN_SCILAB}
Source: modules\{#CONSOLE}\VERSION.xml; DestDir: {app}\modules\{#CONSOLE}; Components: {#COMPN_SCILAB}
;
Source: modules\{#CONSOLE}\sci_gateway\console_gateway.xml; DestDir: {app}\modules\{#CONSOLE}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#CONSOLE}\includes\*.h; DestDir: {app}\modules\{#CONSOLE}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#CONSOLE}\help\addchapter.sce; DestDir: {app}\modules\{#CONSOLE}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
Source: modules\{#CONSOLE}\help\removechapter.sce; DestDir: {app}\modules\{#CONSOLE}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#CONSOLE}\etc\{#CONSOLE}.quit; DestDir: {app}\modules\{#CONSOLE}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#CONSOLE}\etc\{#CONSOLE}.start; DestDir: {app}\modules\{#CONSOLE}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#CONSOLE}\help\*.htm; DestDir: {app}\modules\{#CONSOLE}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#CONSOLE}\languages\*.xml; DestDir: {app}\modules\{#CONSOLE}\languages; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#CONSOLE}\macros\buildmacros.sce; DestDir: {app}\modules\{#CONSOLE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CONSOLE}\macros\*.bin; DestDir: {app}\modules\{#CONSOLE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CONSOLE}\macros\*.sci; DestDir: {app}\modules\{#CONSOLE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CONSOLE}\macros\lib; DestDir: {app}\modules\{#CONSOLE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CONSOLE}\macros\names; DestDir: {app}\modules\{#CONSOLE}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#CONSOLE}\demos\*.*; DestDir: {app}\modules\{#CONSOLE}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;--------------------------------------------------------------------------------------------------------------
