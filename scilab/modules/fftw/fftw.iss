;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; FFTW module
;--------------------------------------------------------------------------------------------------------------
;
#define FFTW "fftw"
;

Source: modules\{#FFTW}\VERSION; DestDir: {app}\modules\{#FFTW}; Components: {#COMPN_FFTW}; Check: set_with_fftw
Source: modules\{#FFTW}\readme.txt; DestDir: {app}\modules\{#FFTW}; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\licence.txt; DestDir: {app}\modules\{#FFTW}; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\changelog.txt; DestDir: {app}\modules\{#FFTW}; Components: {#COMPN_FFTW}
;
Source: modules\{#FFTW}\sci_gateway\fftw; DestDir: {app}\modules\{#FFTW}\sci_gateway; Components: {#COMPN_FFTW}
;
Source: modules\{#FFTW}\etc\{#FFTW}.quit; DestDir: {app}\modules\{#FFTW}\etc; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\etc\{#FFTW}.start; DestDir: {app}\modules\{#FFTW}\etc; Components: {#COMPN_FFTW}
;
Source: modules\{#FFTW}\help\en_US\addchapter.sce; DestDir: {app}\modules\{#FFTW}\help\en_US; Components:  {#COMPN_FFTW}
Source: modules\{#FFTW}\help\en_US\removechapter.sce; DestDir: {app}\modules\{#FFTW}\help\en_US; Components: {#COMPN_FFTW}
;
Source: modules\{#FFTW}\help\fr_FR\addchapter.sce; DestDir: {app}\modules\{#FFTW}\help\fr_FR; Components:  {#COMPN_FFTW}
Source: modules\{#FFTW}\help\fr_FR\removechapter.sce; DestDir: {app}\modules\{#FFTW}\help\fr_FR; Components: {#COMPN_FFTW}
;
;Source: modules\{#FFTW}\demos\*.*; DestDir: {app}\modules\{#FFTW}\demos; Flags: recursesubdirs; Components: {#COMPN_FFTW}
;
;Source: modules\{#FFTW}\help\en_US\*.htm; DestDir: {app}\modules\{#FFTW}\help\en_US; Components: {#COMPN_FFTW}
;Source: modules\{#FFTW}\help\fr_FR\*.htm; DestDir: {app}\modules\{#FFTW}\help\fr_FR; Components: {#COMPN_FFTW}
;
Source: modules\{#FFTW}\languages\en_US\*.xml; DestDir: {app}\modules\{#FFTW}\languages\en_US; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\languages\fr_FR\*.xml; DestDir: {app}\modules\{#FFTW}\languages\fr_FR; Components: {#COMPN_FFTW}
;
Source: modules\{#FFTW}\includes\*.h; Flags: recursesubdirs; DestDir: {app}\modules\{#FFTW}\includes; Components: {#COMPN_FFTW}
;
;Source: modules\{#FFTW}\macros\lib; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
;Source: modules\{#FFTW}\macros\names; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
;Source: modules\{#FFTW}\macros\*.bin; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
;Source: modules\{#FFTW}\macros\*.sci; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\macros\buildmacros.sce; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
;
Source: bin\fftw.dll; DestDir: {app}\bin; DestName: fftw.dll; Components: {#COMPN_FFTW}
Source: bin\nofftw.dll; DestDir: {app}\bin; DestName: fftw.dll; Components: not ({#COMPN_FFTW})
;
;
;--------------------------------------------------------------------------------------------------------------
