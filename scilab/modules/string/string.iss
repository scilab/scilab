;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; string module
;--------------------------------------------------------------------------------------------------------------
;
#define STRING "string"
;
Source: modules\{#STRING}\VERSION; DestDir: {app}\modules\{#STRING}; Components: {#COMPN_SCILAB}
Source: modules\{#STRING}\readme.txt; DestDir: {app}\modules\{#STRING}; Components: {#COMPN_SCILAB}
Source: modules\{#STRING}\licence.txt; DestDir: {app}\modules\{#STRING}; Components: {#COMPN_SCILAB}
Source: modules\{#STRING}\changelog.txt; DestDir: {app}\modules\{#STRING}; Components: {#COMPN_SCILAB}
;
Source: modules\{#STRING}\sci_gateway\string; DestDir: {app}\modules\{#STRING}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#STRING}\etc\{#STRING}.quit; DestDir: {app}\modules\{#STRING}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#STRING}\etc\{#STRING}.start; DestDir: {app}\modules\{#STRING}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#STRING}\help\en_US\addchapter.sce; DestDir: {app}\modules\{#STRING}\help\en_US; Components:  {#COMPN_SCILAB}
Source: modules\{#STRING}\help\en_US\removechapter.sce; DestDir: {app}\modules\{#STRING}\help\en_US; Components: {#COMPN_SCILAB}
;
Source: modules\{#STRING}\help\fr_FR\addchapter.sce; DestDir: {app}\modules\{#STRING}\help\fr_FR; Components:  {#COMPN_SCILAB}
Source: modules\{#STRING}\help\fr_FR\removechapter.sce; DestDir: {app}\modules\{#STRING}\help\fr_FR; Components: {#COMPN_SCILAB}
;
Source: modules\{#STRING}\help\en_US\*.htm; DestDir: {app}\modules\{#STRING}\help\en_US; Components: {#COMPN_SCILAB}
Source: modules\{#STRING}\help\fr_FR\*.htm; DestDir: {app}\modules\{#STRING}\help\fr_FR; Components: {#COMPN_SCILAB}
;
Source: modules\{#STRING}\includes\*.h; DestDir: {app}\modules\{#STRING}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#STRING}\macros\buildmacros.sce; DestDir: {app}\modules\{#STRING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#STRING}\macros\lib; DestDir: {app}\modules\{#STRING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#STRING}\macros\names; DestDir: {app}\modules\{#STRING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#STRING}\macros\*.bin; DestDir: {app}\modules\{#STRING}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#STRING}\macros\*.sci; DestDir: {app}\modules\{#STRING}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#STRING}\demos\*.*; DestDir: {app}\modules\{#STRING}\demos; Flags: recursesubdirs;  Components: {#COMPN_SCILAB}
;
;--------------------------------------------------------------------------------------------------------------
