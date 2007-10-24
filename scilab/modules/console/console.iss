;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Vincent COUVERT
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; console module
;--------------------------------------------------------------------------------------------------------------
;
#define CONSOLE "console"
;
Source: bin\sciconsole.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#CONSOLE}\jar\org.scilab.modules.console.jar;DestDir: {app}\modules\{#CONSOLE}\jar; Components: {#COMPN_SCILAB}
Source: thirdparty\JRosetta-API-1.0.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\JRosetta-1.0.jar;DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: modules\{#CONSOLE}\etc\configuration.xml; DestDir: {app}\modules\{#CONSOLE}\etc; Components: {#COMPN_SCILAB};
;
;Source: bin\noconsole.dll; DestDir: {app}\bin; DestName: sciconsole.dll; Components: not ({#COMPN_CONSOLE})
;
Source: modules\{#CONSOLE}\changelog.txt; DestDir: {app}\modules\{#CONSOLE}; Components: {#COMPN_SCILAB};
Source: modules\{#CONSOLE}\licence.txt; DestDir: {app}\modules\{#CONSOLE}; Components: {#COMPN_SCILAB}
Source: modules\{#CONSOLE}\readme.txt; DestDir: {app}\modules\{#CONSOLE}; Components: {#COMPN_SCILAB}
Source: modules\{#CONSOLE}\version.xml; DestDir: {app}\modules\{#CONSOLE}; Components: {#COMPN_SCILAB}
;
;Source: modules\{#CONSOLE}\sci_gateway\console.xml; DestDir: {app}\modules\{#CONSOLE}\sci_gateway; Components: {#COMPN_SCILAB}
;
;Source: modules\{#CONSOLE}\includes\*.h; DestDir: {app}\modules\{#CONSOLE}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#CONSOLE}\help\addchapter.sce; DestDir: {app}\modules\{#CONSOLE}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
Source: modules\{#CONSOLE}\help\removechapter.sce; DestDir: {app}\modules\{#CONSOLE}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#CONSOLE}\etc\{#CONSOLE}.quit; DestDir: {app}\modules\{#CONSOLE}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#CONSOLE}\etc\{#CONSOLE}.start; DestDir: {app}\modules\{#CONSOLE}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#CONSOLE}\etc\configuration.xml; DestDir: {app}\modules\{#CONSOLE}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#CONSOLE}\help\*.htm; DestDir: {app}\modules\{#CONSOLE}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;
Source: modules\{#CONSOLE}\macros\buildmacros.sce; DestDir: {app}\modules\{#CONSOLE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CONSOLE}\macros\buildmacros.bat; DestDir: {app}\modules\{#CONSOLE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CONSOLE}\macros\cleanmacros.bat; DestDir: {app}\modules\{#CONSOLE}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#CONSOLE}\macros\*.bin; DestDir: {app}\modules\{#CONSOLE}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#CONSOLE}\macros\*.sci; DestDir: {app}\modules\{#CONSOLE}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#CONSOLE}\macros\lib; DestDir: {app}\modules\{#CONSOLE}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#CONSOLE}\macros\names; DestDir: {app}\modules\{#CONSOLE}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#CONSOLE}\demos\*.*; DestDir: {app}\modules\{#CONSOLE}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;--------------------------------------------------------------------------------------------------------------
