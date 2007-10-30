;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; output_stream module
;--------------------------------------------------------------------------------------------------------------
;
#define OUTPUT_STREAM "output_stream"
;
Source: bin\scioutput_stream.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#OUTPUT_STREAM}\changelog.txt; DestDir: {app}\modules\{#OUTPUT_STREAM}; Components: {#COMPN_SCILAB};
Source: modules\{#OUTPUT_STREAM}\licence.txt; DestDir: {app}\modules\{#OUTPUT_STREAM}; Components: {#COMPN_SCILAB}
Source: modules\{#OUTPUT_STREAM}\readme.txt; DestDir: {app}\modules\{#OUTPUT_STREAM}; Components: {#COMPN_SCILAB}
Source: modules\{#OUTPUT_STREAM}\version.xml; DestDir: {app}\modules\{#OUTPUT_STREAM}; Components: {#COMPN_SCILAB}
;
Source: modules\{#OUTPUT_STREAM}\includes\*.h; DestDir: {app}\modules\{#OUTPUT_STREAM}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#OUTPUT_STREAM}\help\addchapter.sce; DestDir: {app}\modules\{#OUTPUT_STREAM}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
Source: modules\{#OUTPUT_STREAM}\help\removechapter.sce; DestDir: {app}\modules\{#OUTPUT_STREAM}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#OUTPUT_STREAM}\etc\{#OUTPUT_STREAM}.quit; DestDir: {app}\modules\{#OUTPUT_STREAM}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#OUTPUT_STREAM}\etc\{#OUTPUT_STREAM}.start; DestDir: {app}\modules\{#OUTPUT_STREAM}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#OUTPUT_STREAM}\help\*.htm; DestDir: {app}\modules\{#OUTPUT_STREAM}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;
Source: modules\{#OUTPUT_STREAM}\macros\buildmacros.sce; DestDir: {app}\modules\{#OUTPUT_STREAM}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OUTPUT_STREAM}\macros\buildmacros.bat; DestDir: {app}\modules\{#OUTPUT_STREAM}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#OUTPUT_STREAM}\macros\cleanmacros.bat; DestDir: {app}\modules\{#OUTPUT_STREAM}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#OUTPUT_STREAM}\macros\*.bin; DestDir: {app}\modules\{#OUTPUT_STREAM}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#OUTPUT_STREAM}\macros\*.sci; DestDir: {app}\modules\{#OUTPUT_STREAM}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#OUTPUT_STREAM}\macros\lib; DestDir: {app}\modules\{#OUTPUT_STREAM}\macros; Components: {#COMPN_SCILAB}
;Source: modules\{#OUTPUT_STREAM}\macros\names; DestDir: {app}\modules\{#OUTPUT_STREAM}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#OUTPUT_STREAM}\demos\*.*; DestDir: {app}\modules\{#OUTPUT_STREAM}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;--------------------------------------------------------------------------------------------------------------
