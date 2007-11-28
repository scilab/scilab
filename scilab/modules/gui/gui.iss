;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; gui module
;--------------------------------------------------------------------------------------------------------------
;
#define GUI "gui"
;
Source: bin\scigui.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#GUI}\jar\org.scilab.modules.gui.jar; DestDir: {app}\modules\{#GUI}\jar; Components: {#COMPN_SCILAB}
Source: thirdparty\flexdock-0.5.1.jar; DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\commons-logging-1.1.jar; DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\looks-2.1.1.jar; DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
Source: thirdparty\skinlf.jar; DestDir: {app}\thirdparty; Components: {#COMPN_SCILAB}
;
Source: modules\{#GUI}\VERSION.xml; DestDir: {app}\modules\{#GUI}; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\readme.txt; DestDir: {app}\modules\{#GUI}; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\licence.txt; DestDir: {app}\modules\{#GUI}; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\changelog.txt; DestDir: {app}\modules\{#GUI}; Components: {#COMPN_SCILAB}
;
Source: modules\{#GUI}\sci_gateway\gui_gateway.xml; DestDir: {app}\modules\{#GUI}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#GUI}\etc\{#GUI}.quit; DestDir: {app}\modules\{#GUI}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\etc\{#GUI}.start; DestDir: {app}\modules\{#GUI}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#GUI}\help\addchapter.sce; DestDir: {app}\modules\{#GUI}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
Source: modules\{#GUI}\help\*.htm; DestDir: {app}\modules\{#GUI}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;
Source: modules\{#GUI}\includes\*.h; DestDir: {app}\modules\{#GUI}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#GUI}\macros\lib; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\macros\names; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\macros\*.bin; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\macros\*.sci; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\macros\buildmacros.sce; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\macros\buildmacros.bat; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GUI}\macros\cleanmacros.bat; DestDir: {app}\modules\{#GUI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#GUI}\demos\*.*; DestDir: {app}\modules\{#GUI}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;Source: modules\{#GUI}\unit_tests\*.ref; DestDir: {app}\modules\{#GUI}\unit_tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_UNIT_TESTS}
;Source: modules\{#GUI}\unit_tests\*.tst; DestDir: {app}\modules\{#GUI}\unit_tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_UNIT_TESTS}
;
Source: modules\{#GUI}\nonreg_tests\*.*; DestDir: {app}\modules\{#GUI}\nonreg_tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_NO_REGRESSION_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
