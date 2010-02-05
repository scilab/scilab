;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
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
#ifdef WITH_FFTW_LIBRARY
Source: bin\fftw\libfftw3-3.dll; DestDir: {app}\bin\fftw; Components: {#COMPN_FFTW}
Source: bin\fftw\COPYRIGHT; DestDir: {app}\bin\fftw; Components: {#COMPN_FFTW}
Source: bin\fftw\COPYING; DestDir: {app}\bin\fftw; Components: {#COMPN_FFTW}
#endif
Source: bin\fftw.dll; DestDir: {app}\bin; DestName: fftw.dll; Components: {#COMPN_FFTW}
Source: bin\nofftw.dll; DestDir: {app}\bin; DestName: fftw.dll; Components: not ({#COMPN_FFTW})
;

Source: modules\{#FFTW}\VERSION.xml; DestDir: {app}\modules\{#FFTW}; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\readme.txt; DestDir: {app}\modules\{#FFTW}; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\license.txt; DestDir: {app}\modules\{#FFTW}; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\changelog.txt; DestDir: {app}\modules\{#FFTW}; Components: {#COMPN_FFTW}
;
Source: modules\{#FFTW}\sci_gateway\fftw_gateway.xml; DestDir: {app}\modules\{#FFTW}\sci_gateway; Components: {#COMPN_FFTW}
;
Source: modules\{#FFTW}\etc\{#FFTW}.quit; DestDir: {app}\modules\{#FFTW}\etc; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\etc\{#FFTW}.start; DestDir: {app}\modules\{#FFTW}\etc; Components: {#COMPN_FFTW}
;
Source: modules\{#FFTW}\help\addchapter.sce; DestDir: {app}\modules\{#FFTW}\help; Flags: recursesubdirs; Components:  {#COMPN_FFTW}
;
;Source: modules\{#FFTW}\demos\*.*; DestDir: {app}\modules\{#FFTW}\demos; Flags: recursesubdirs; Components: {#COMPN_FFTW}
;
;Source: modules\{#FFTW}\includes\*.h; Flags: recursesubdirs; DestDir: {app}\modules\{#FFTW}\includes; Components: {#COMPN_FFTW}
;
;Source: modules\{#FFTW}\macros\lib; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
;Source: modules\{#FFTW}\macros\names; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
;Source: modules\{#FFTW}\macros\*.bin; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
;Source: modules\{#FFTW}\macros\*.sci; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\macros\buildmacros.sce; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\macros\buildmacros.bat; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
Source: modules\{#FFTW}\macros\cleanmacros.bat; DestDir: {app}\modules\{#FFTW}\macros; Components: {#COMPN_FFTW}
;
Source: modules\{#FFTW}\tests\*.*; DestDir: {app}\modules\{#FFTW}\tests; Flags: recursesubdirs; Components: {#COMPN_FFTW} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
;
