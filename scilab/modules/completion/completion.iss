;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; completion module
;--------------------------------------------------------------------------------------------------------------
;
#define COMPLETION "completion"
;
Source: bin\scicompletion.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\scicompletion.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMPLETION}\jar\org.scilab.modules.completion.jar;DestDir: {app}\modules\{#COMPLETION}\jar; Components: {#COMPN_SCILAB}
;
;Source: bin\noscicompletion.dll; DestDir: {app}\bin; DestName: scicompletion.dll; Components: not ({#COMPN_SCILAB})
;
Source: modules\{#COMPLETION}\changelog.txt; DestDir: {app}\modules\{#COMPLETION}; Components: {#COMPN_SCILAB};
Source: modules\{#COMPLETION}\license.txt; DestDir: {app}\modules\{#COMPLETION}; Components: {#COMPN_SCILAB}
Source: modules\{#COMPLETION}\readme.txt; DestDir: {app}\modules\{#COMPLETION}; Components: {#COMPN_SCILAB}
Source: modules\{#COMPLETION}\version.xml; DestDir: {app}\modules\{#COMPLETION}; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMPLETION}\sci_gateway\completion_gateway.xml; DestDir: {app}\modules\{#COMPLETION}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMPLETION}\includes\*.h; DestDir: {app}\modules\{#COMPLETION}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMPLETION}\help\addchapter.sce; DestDir: {app}\modules\{#COMPLETION}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
Source: modules\{#COMPLETION}\etc\{#COMPLETION}.quit; DestDir: {app}\modules\{#COMPLETION}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#COMPLETION}\etc\{#COMPLETION}.start; DestDir: {app}\modules\{#COMPLETION}\etc; Components: {#COMPN_SCILAB}
;
;
;Source: modules\{#COMPLETION}\macros\buildmacros.sce; DestDir: {app}\modules\{#COMPLETION}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#COMPLETION}\macros\buildmacros.bat; DestDir: {app}\modules\{#COMPLETION}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#COMPLETION}\macros\cleanmacros.bat; DestDir: {app}\modules\{#COMPLETION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#COMPLETION}\macros\*.bin; DestDir: {app}\modules\{#COMPLETION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#COMPLETION}\macros\*.sci; DestDir: {app}\modules\{#COMPLETION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#COMPLETION}\macros\lib; DestDir: {app}\modules\{#COMPLETION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#COMPLETION}\macros\names; DestDir: {app}\modules\{#COMPLETION}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#COMPLETION}\demos\*.*; DestDir: {app}\modules\{#COMPLETION}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#COMPLETION}\tests\*.*; DestDir: {app}\modules\{#COMPLETION}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
