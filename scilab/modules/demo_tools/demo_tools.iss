;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; demo_tools module
;--------------------------------------------------------------------------------------------------------------
;
#define DEMO_TOOLS "demo_tools"
;
Source: modules\{#DEMO_TOOLS}\VERSION.xml; DestDir: {app}\modules\{#DEMO_TOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\readme.txt; DestDir: {app}\modules\{#DEMO_TOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\license.txt; DestDir: {app}\modules\{#DEMO_TOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\changelog.txt; DestDir: {app}\modules\{#DEMO_TOOLS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#DEMO_TOOLS}\etc\{#DEMO_TOOLS}.quit; DestDir: {app}\modules\{#DEMO_TOOLS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\etc\{#DEMO_TOOLS}.start; DestDir: {app}\modules\{#DEMO_TOOLS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#DEMO_TOOLS}\help\addchapter.sce; DestDir: {app}\modules\{#DEMO_TOOLS}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
Source: modules\{#DEMO_TOOLS}\help\*.htm; DestDir: {app}\modules\{#DEMO_TOOLS}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#DEMO_TOOLS}\macros\buildmacros.sce; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\macros\buildmacros.bat; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\macros\lib; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\macros\names; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\macros\*.bin; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#DEMO_TOOLS}\macros\*.sci; DestDir: {app}\modules\{#DEMO_TOOLS}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#DEMO_TOOLS}\examples\*.*; DestDir: {app}\modules\{#DEMO_TOOLS}\examples; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;Source: modules\{#DEMO_TOOLS}\tests\*.*; DestDir: {app}\modules\{#DEMO_TOOLS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
