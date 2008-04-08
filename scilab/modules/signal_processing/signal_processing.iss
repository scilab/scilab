;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; signal_processing module
;--------------------------------------------------------------------------------------------------------------
;
#define SIGPROC "signal_processing"
;
Source: modules\{#SIGPROC}\VERSION.xml; DestDir: {app}\modules\{#SIGPROC}; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\readme.txt; DestDir: {app}\modules\signal_processing; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\license.txt; DestDir: {app}\modules\signal_processing; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\changelog.txt; DestDir: {app}\modules\signal_processing; Components: {#COMPN_SIGPROC}
;
Source: modules\{#SIGPROC}\sci_gateway\signal_processing_gateway.xml; DestDir: {app}\modules\signal_processing\sci_gateway; Components: {#COMPN_SIGPROC}
;
Source: modules\{#SIGPROC}\etc\{#SIGPROC}.quit; DestDir: {app}\modules\{#SIGPROC}\etc; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\etc\{#SIGPROC}.start; DestDir: {app}\modules\{#SIGPROC}\etc; Components: {#COMPN_SIGPROC}
;
Source: modules\{#SIGPROC}\help\addchapter.sce; DestDir: {app}\modules\{#SIGPROC}\help; Flags: recursesubdirs; Components: {#COMPN_SIGPROC}
;
Source: modules\{#SIGPROC}\macros\lib; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\macros\names; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\macros\*.bin; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\macros\*.sci; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\macros\buildmacros.sce; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\macros\buildmacros.bat; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SIGPROC}
;
Source: modules\{#SIGPROC}\demos\*.*; DestDir: {app}\modules\{#SIGPROC}\demos; Flags: recursesubdirs; Components: {#COMPN_SIGPROC}
;
Source: modules\{#SIGPROC}\tests\*.*; DestDir: {app}\modules\{#SIGPROC}\tests; Flags: recursesubdirs; Components: {#COMPN_SIGPROC} and {#COMPN_TESTS}
;--------------------------------------------------------------------------------------------------------------
