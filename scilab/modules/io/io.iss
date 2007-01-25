;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
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
Source: modules\{#IO}\VERSION; DestDir: {app}\modules\{#IO}; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\readme.txt; DestDir: {app}\modules\{#IO}; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\licence.txt; DestDir: {app}\modules\{#IO}; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\changelog.txt; DestDir: {app}\modules\{#IO}; Components: {#COMPN_SCILAB}
;
Source: modules\{#IO}\sci_gateway\io; DestDir: {app}\modules\{#IO}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#IO}\etc\{#IO}.quit; DestDir: {app}\modules\{#IO}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\etc\{#IO}.start; DestDir: {app}\modules\{#IO}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#IO}\help\eng\*.htm; DestDir: {app}\modules\{#IO}\help\eng; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\help\fr\*.htm; DestDir: {app}\modules\{#IO}\help\fr; Components: {#COMPN_SCILAB}
;
Source: modules\{#IO}\includes\*.h; DestDir: {app}\modules\{#IO}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#IO}\macros\buildmacros.sce; DestDir: {app}\modules\{#IO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\macros\lib; DestDir: {app}\modules\{#IO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\macros\names; DestDir: {app}\modules\{#IO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\macros\*.bin; DestDir: {app}\modules\{#IO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#IO}\macros\*.sci; DestDir: {app}\modules\{#IO}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#IO}\demos\*.*; DestDir: {app}\modules\{#IO}\demos; Flags: recursesubdirs;  Components: {#COMPN_SCILAB}
;
;--------------------------------------------------------------------------------------------------------------
