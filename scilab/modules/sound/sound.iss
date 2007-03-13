;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; sound module
;--------------------------------------------------------------------------------------------------------------
;
#define SOUND "sound"
;
Source: bin\sound.dll; DestDir: {app}\bin; Components: {#COMPN_SOUND}
;
Source: bin\nosound.dll; DestDir: {app}\bin; DestName: sound.dll; Components: not ({#COMPN_SOUND})
;
Source: modules\{#SOUND}\changelog.txt; DestDir: {app}\modules\{#SOUND}; Components: {#COMPN_SOUND}; Check: set_with_sound
Source: modules\{#SOUND}\licence.txt; DestDir: {app}\modules\{#SOUND}; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\readme.txt; DestDir: {app}\modules\{#SOUND}; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\VERSION; DestDir: {app}\modules\{#SOUND}; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\sci_gateway\sound; DestDir: {app}\modules\{#SOUND}\sci_gateway; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\includes\*.h; DestDir: {app}\modules\{#SOUND}\includes; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\help\en_US\addchapter.sce; DestDir: {app}\modules\{#SOUND}\help\en_US; Components:  {#COMPN_SOUND}
Source: modules\{#SOUND}\help\en_US\removechapter.sce; DestDir: {app}\modules\{#SOUND}\help\en_US; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\help\fr_FR\addchapter.sce; DestDir: {app}\modules\{#SOUND}\help\fr_FR; Components:  {#COMPN_SOUND}
Source: modules\{#SOUND}\help\fr_FR\removechapter.sce; DestDir: {app}\modules\{#SOUND}\help\fr_FR; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\etc\{#SOUND}.quit; DestDir: {app}\modules\{#SOUND}\etc; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\etc\{#SOUND}.start; DestDir: {app}\modules\{#SOUND}\etc; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\help\en_US\*.htm; DestDir: {app}\modules\{#SOUND}\help\en_US; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\help\fr_FR\*.htm; DestDir: {app}\modules\{#SOUND}\help\fr_FR; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\languages\en_US\*.xml; DestDir: {app}\modules\{#SOUND}\languages\en_US; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\languages\fr_FR\*.xml; DestDir: {app}\modules\{#SOUND}\languages\fr_FR; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\macros\buildmacros.sce; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\macros\*.bin; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\macros\*.sci; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\macros\lib; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SOUND}
Source: modules\{#SOUND}\macros\names; DestDir: {app}\modules\{#SOUND}\macros; Components: {#COMPN_SOUND}
;
Source: modules\{#SOUND}\demos\*.*; DestDir: {app}\modules\{#SOUND}\demos; Flags: recursesubdirs; Components: {#COMPN_SOUND}
;
;--------------------------------------------------------------------------------------------------------------
