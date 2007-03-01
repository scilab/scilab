;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
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
Source: modules\{#SIGPROC}\VERSION; DestDir: {app}\modules\{#SIGPROC}; Components: {#COMPN_SIGPROC}; Check: set_with_signal_processing
Source: modules\{#SIGPROC}\readme.txt; DestDir: {app}\modules\signal_processing; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\licence.txt; DestDir: {app}\modules\signal_processing; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\changelog.txt; DestDir: {app}\modules\signal_processing; Components: {#COMPN_SIGPROC}
;
Source: modules\{#SIGPROC}\sci_gateway\signal_processing; DestDir: {app}\modules\signal_processing\sci_gateway; Components: {#COMPN_SIGPROC}
;
Source: modules\{#SIGPROC}\etc\{#SIGPROC}.quit; DestDir: {app}\modules\{#SIGPROC}\etc; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\etc\{#SIGPROC}.start; DestDir: {app}\modules\{#SIGPROC}\etc; Components: {#COMPN_SIGPROC}
;
Source: modules\{#SIGPROC}\help\en_US\addchapter.sce; DestDir: {app}\modules\{#SIGPROC}\help\en_US; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\help\en_US\removechapter.sce; DestDir: {app}\modules\{#SIGPROC}\help\en_US; Components: {#COMPN_SIGPROC}
;
Source: modules\{#SIGPROC}\help\fr_FR\addchapter.sce; DestDir: {app}\modules\{#SIGPROC}\help\fr_FR; Components:  {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\help\fr_FR\removechapter.sce; DestDir: {app}\modules\{#SIGPROC}\help\fr_FR; Components: {#COMPN_SIGPROC}
;
Source: modules\{#SIGPROC}\help\en_US\*.htm; DestDir: {app}\modules\{#SIGPROC}\help\en_US; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\help\fr_FR\*.htm; DestDir: {app}\modules\{#SIGPROC}\help\fr_FR; Components: {#COMPN_SIGPROC}
;
Source: modules\{#SIGPROC}\languages\en_US\*.xml; DestDir: {app}\modules\{#SIGPROC}\languages\en_US; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\languages\fr_FR\*.xml; DestDir: {app}\modules\{#SIGPROC}\languages\fr_FR; Components: {#COMPN_SIGPROC}
;
Source: modules\{#SIGPROC}\macros\lib; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\macros\names; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\macros\*.bin; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\macros\*.sci; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\macros\buildmacros.sce; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SIGPROC}
;
Source: modules\{#SIGPROC}\demos\*.*; DestDir: {app}\modules\{#SIGPROC}\demos; Flags: recursesubdirs; Components: {#COMPN_SIGPROC}
;
;--------------------------------------------------------------------------------------------------------------
