;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; io module
;--------------------------------------------------------------------------------------------------------------
;
#define IO "io"
;
Source: bin\io.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\io_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#IO}\VERSION.xml; DestDir: {app}\modules\{#IO}; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\readme.txt; DestDir: {app}\modules\{#IO}; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\license.txt; DestDir: {app}\modules\{#IO}; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\changelog.txt; DestDir: {app}\modules\{#IO}; Components: {#COMPN_SCILAB}
;
Source: modules\{#IO}\sci_gateway\io_gateway.xml; DestDir: {app}\modules\{#IO}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#IO}\etc\{#IO}.quit; DestDir: {app}\modules\{#IO}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\etc\{#IO}.start; DestDir: {app}\modules\{#IO}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#IO}\help\addchapter.sce; DestDir: {app}\modules\{#IO}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
;Source: modules\{#IO}\includes\*.h; DestDir: {app}\modules\{#IO}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#IO}\macros\buildmacros.sce; DestDir: {app}\modules\{#IO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\macros\buildmacros.bat; DestDir: {app}\modules\{#IO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\macros\cleanmacros.bat; DestDir: {app}\modules\{#IO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\macros\lib; DestDir: {app}\modules\{#IO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\macros\names; DestDir: {app}\modules\{#IO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\macros\*.sci; DestDir: {app}\modules\{#IO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\macros\*.bin; DestDir: {app}\modules\{#IO}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#IO}\demos\*.*; DestDir: {app}\modules\{#IO}\demos; Flags: recursesubdirs;  Components: {#COMPN_SCILAB}
;
Source: modules\{#IO}\tests\*.*; DestDir: {app}\modules\{#IO}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
