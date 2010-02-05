;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; fileio module
;--------------------------------------------------------------------------------------------------------------
;
#define FILEIO "fileio"
;
Source: modules\{#FILEIO}\VERSION.xml; DestDir: {app}\modules\{#FILEIO}; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\readme.txt; DestDir: {app}\modules\{#FILEIO}; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\license.txt; DestDir: {app}\modules\{#FILEIO}; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\changelog.txt; DestDir: {app}\modules\{#FILEIO}; Components: {#COMPN_SCILAB}
;
Source: modules\{#FILEIO}\sci_gateway\fileio_gateway.xml; DestDir: {app}\modules\{#FILEIO}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#FILEIO}\etc\{#FILEIO}.quit; DestDir: {app}\modules\{#FILEIO}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\etc\fileio.start; DestDir: {app}\modules\{#FILEIO}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#FILEIO}\help\addchapter.sce; DestDir: {app}\modules\{#FILEIO}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
;Source: modules\{#FILEIO}\includes\*.h; DestDir: {app}\modules\{#FILEIO}\includes; Components: {#COMPN_SCILAB}
;
Source: bin\fileio.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#FILEIO}\macros\buildmacros.sce; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\macros\buildmacros.bat; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\macros\cleanmacros.bat; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\macros\lib; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\macros\names; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\macros\*.bin; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#FILEIO}\macros\*.sci; DestDir: {app}\modules\{#FILEIO}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#FILEIO}\demos\*.*; DestDir: {app}\modules\{#FILEIO}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#FILEIO}\tests\*.*; DestDir: {app}\modules\{#FILEIO}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
