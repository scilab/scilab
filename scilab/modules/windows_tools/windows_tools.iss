;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; windows_tools module
;--------------------------------------------------------------------------------------------------------------
;
#define WINDOWS_TOOLS "windows_tools"
;
Source: bin\windows_tools.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
;Source: java\jar\modules\org.scilab.modules.module_skeleton.jar;DestDir: {app}\java\jar\modules; Components: {#COMPN_SCILAB}
;
;Source: bin\nowindows_tools.dll; DestDir: {app}\bin; DestName: windows_tools.dll; Components: not ({#COMPN_SCILAB})
;
Source: modules\{#WINDOWS_TOOLS}\changelog.txt; DestDir: {app}\modules\{#WINDOWS_TOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#WINDOWS_TOOLS}\licence.txt; DestDir: {app}\modules\{#WINDOWS_TOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#WINDOWS_TOOLS}\readme.txt; DestDir: {app}\modules\{#WINDOWS_TOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#WINDOWS_TOOLS}\version.xml; DestDir: {app}\modules\{#WINDOWS_TOOLS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#WINDOWS_TOOLS}\sci_gateway\windows_tools_gateway.xml; DestDir: {app}\modules\{#WINDOWS_TOOLS}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#WINDOWS_TOOLS}\includes\*.h; DestDir: {app}\modules\{#WINDOWS_TOOLS}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#WINDOWS_TOOLS}\help\addchapter.sce; DestDir: {app}\modules\{#WINDOWS_TOOLS}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
Source: modules\{#WINDOWS_TOOLS}\etc\{#WINDOWS_TOOLS}.quit; DestDir: {app}\modules\{#WINDOWS_TOOLS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#WINDOWS_TOOLS}\etc\{#WINDOWS_TOOLS}.start; DestDir: {app}\modules\{#WINDOWS_TOOLS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#WINDOWS_TOOLS}\help\*.htm; DestDir: {app}\modules\{#WINDOWS_TOOLS}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;
Source: modules\{#WINDOWS_TOOLS}\macros\buildmacros.sce; DestDir: {app}\modules\{#WINDOWS_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#WINDOWS_TOOLS}\macros\buildmacros.bat; DestDir: {app}\modules\{#WINDOWS_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#WINDOWS_TOOLS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#WINDOWS_TOOLS}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#WINDOWS_TOOLS}\macros\*.bin; DestDir: {app}\modules\{#WINDOWS_TOOLS}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#WINDOWS_TOOLS}\macros\*.sci; DestDir: {app}\modules\{#WINDOWS_TOOLS}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#WINDOWS_TOOLS}\macros\lib; DestDir: {app}\modules\{#WINDOWS_TOOLS}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#WINDOWS_TOOLS}\macros\names; DestDir: {app}\modules\{#WINDOWS_TOOLS}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#WINDOWS_TOOLS}\demos\*.*; DestDir: {app}\modules\{#WINDOWS_TOOLS}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;--------------------------------------------------------------------------------------------------------------
