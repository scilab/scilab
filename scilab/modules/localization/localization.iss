;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; localization module
;--------------------------------------------------------------------------------------------------------------
;
#define LOCALIZATION "localization"
;
Source: bin\scilocalization.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\scilocalization.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\jar\org.scilab.modules.localization.jar;DestDir: {app}\modules\{#LOCALIZATION}\jar; Components: {#COMPN_SCILAB}
;
Source: modules\{#LOCALIZATION}\VERSION.xml; DestDir: {app}\modules\{#LOCALIZATION}; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\readme.txt; DestDir: {app}\modules\{#LOCALIZATION}; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\license.txt; DestDir: {app}\modules\{#LOCALIZATION}; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\changelog.txt; DestDir: {app}\modules\{#LOCALIZATION}; Components: {#COMPN_SCILAB}
;
Source: modules\{#LOCALIZATION}\etc\{#LOCALIZATION}.quit; DestDir: {app}\modules\{#LOCALIZATION}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\etc\{#LOCALIZATION}.start; DestDir: {app}\modules\{#LOCALIZATION}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#LOCALIZATION}\sci_gateway\{#LOCALIZATION}_gateway.xml; DestDir: {app}\modules\{#LOCALIZATION}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#LOCALIZATION}\help\addchapter.sce; DestDir: {app}\modules\{#LOCALIZATION}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
;Source: modules\{#LOCALIZATION}\includes\*.h; DestDir: {app}\modules\{#LOCALIZATION}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\includes\localization.h; DestDir: {app}\modules\{#LOCALIZATION}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\includes\setgetlanguage.h; DestDir: {app}\modules\{#LOCALIZATION}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\includes\charEncoding.h; DestDir: {app}\modules\{#LOCALIZATION}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#LOCALIZATION}\includes\dynlib_localization.h; DestDir: {app}\modules\{#LOCALIZATION}\includes; Components: {#COMPN_SCILAB}
;
;Source: modules\{#LOCALIZATION}\macros\lib; DestDir: {app}\modules\{#LOCALIZATION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#LOCALIZATION}\macros\names; DestDir: {app}\modules\{#LOCALIZATION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#LOCALIZATION}\macros\*.bin; DestDir: {app}\modules\{#LOCALIZATION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#LOCALIZATION}\macros\*.sci; DestDir: {app}\modules\{#LOCALIZATION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#LOCALIZATION}\macros\buildmacros.sce; DestDir: {app}\modules\{#LOCALIZATION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#LOCALIZATION}\macros\buildmacros.bat; DestDir: {app}\modules\{#LOCALIZATION}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#LOCALIZATION}\macros\cleanmacros.bat; DestDir: {app}\modules\{#LOCALIZATION}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#LOCALIZATION}\xml\*.dtd; DestDir: {app}\modules\{#LOCALIZATION}\xml; Components: {#COMPN_SCILAB}
;
;Source: modules\{#LOCALIZATION}\demos\*.*; DestDir: {app}\modules\{#LOCALIZATION}\demos; Components: {#COMPN_SCILAB}
;
Source: modules\{#LOCALIZATION}\tests\*.*; DestDir: {app}\modules\{#LOCALIZATION}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
