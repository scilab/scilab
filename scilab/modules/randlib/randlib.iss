;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; randlib module
;--------------------------------------------------------------------------------------------------------------
;
#define RANDLIB "randlib"
;
Source: bin\norandlib.dll; DestDir: {app}\bin; DestName: randlib.dll; Components: not ({#COMPN_RANDLIB})
;
Source: bin\randlib.dll; DestDir: {app}\bin; Components: {#COMPN_RANDLIB}
Source: bin\randlib_f.dll; DestDir: {app}\bin; Components: {#COMPN_RANDLIB}
;
Source: modules\{#RANDLIB}\VERSION; DestDir: {app}\modules\{#RANDLIB}; Components: {#COMPN_RANDLIB}; Check: set_with_randlib
Source: modules\{#RANDLIB}\readme.txt; DestDir: {app}\modules\{#RANDLIB}; Components: {#COMPN_RANDLIB}
Source: modules\{#RANDLIB}\licence.txt; DestDir: {app}\modules\{#RANDLIB}; Components: {#COMPN_RANDLIB}
Source: modules\{#RANDLIB}\changelog.txt; DestDir: {app}\modules\{#RANDLIB}; Components: {#COMPN_RANDLIB}
;
Source: modules\{#RANDLIB}\sci_gateway\randlib; DestDir: {app}\modules\{#RANDLIB}\sci_gateway; Components: {#COMPN_RANDLIB}
;
Source: modules\{#RANDLIB}\etc\{#RANDLIB}.quit; DestDir: {app}\modules\{#RANDLIB}\etc; Components: {#COMPN_RANDLIB}
Source: modules\{#RANDLIB}\etc\{#RANDLIB}.start; DestDir: {app}\modules\{#RANDLIB}\etc; Components: {#COMPN_RANDLIB}
;
Source: modules\{#RANDLIB}\help\en_US\addchapter.sce; DestDir: {app}\modules\{#RANDLIB}\help\en_US; Components:  {#COMPN_RANDLIB}
Source: modules\{#RANDLIB}\help\en_US\removechapter.sce; DestDir: {app}\modules\{#RANDLIB}\help\en_US; Components: {#COMPN_RANDLIB}
;
Source: modules\{#RANDLIB}\help\fr_FR\addchapter.sce; DestDir: {app}\modules\{#RANDLIB}\help\fr_FR; Components:  {#COMPN_RANDLIB}
Source: modules\{#RANDLIB}\help\fr_FR\removechapter.sce; DestDir: {app}\modules\{#RANDLIB}\help\fr_FR; Components: {#COMPN_RANDLIB}
;
Source: modules\{#RANDLIB}\help\en_US\*.htm; DestDir: {app}\modules\{#RANDLIB}\help\en_US; Components: {#COMPN_RANDLIB}
Source: modules\{#RANDLIB}\help\fr_FR\*.htm; DestDir: {app}\modules\{#RANDLIB}\help\fr_FR; Components: {#COMPN_RANDLIB}
;
Source: modules\{#RANDLIB}\languages\en_US\*.xml; DestDir: {app}\modules\{#RANDLIB}\languages\en_US; Components: {#COMPN_RANDLIB}
Source: modules\{#RANDLIB}\languages\fr_FR\*.xml; DestDir: {app}\modules\{#RANDLIB}\languages\fr_FR; Components: {#COMPN_RANDLIB}
;
Source: modules\{#RANDLIB}\includes\*.h; DestDir: {app}\modules\{#RANDLIB}\includes; Components: {#COMPN_RANDLIB}
;
;Source: modules\{#RANDLIB}\macros\lib; DestDir: {app}\modules\{#RANDLIB}\macros; Flags: recursesubdirs; Components: {#COMPN_RANDLIB}
;Source: modules\{#RANDLIB}\macros\names; DestDir: {app}\modules\{#RANDLIB}\macros; Flags: recursesubdirs; Components: {#COMPN_RANDLIB}
;Source: modules\{#RANDLIB}\macros\*.bin; DestDir: {app}\modules\{#RANDLIB}\macros; Flags: recursesubdirs; Components: {#COMPN_RANDLIB}
;Source: modules\{#RANDLIB}\macros\*.sci; DestDir: {app}\modules\{#RANDLIB}\macros; Flags: recursesubdirs; Components: {#COMPN_RANDLIB}
Source: modules\{#RANDLIB}\macros\buildmacros.sce; DestDir: {app}\modules\{#RANDLIB}\macros; Flags: recursesubdirs; Components: {#COMPN_RANDLIB}
;
Source: modules\{#RANDLIB}\demos\*.*; DestDir: {app}\modules\{#RANDLIB}\demos; Flags: recursesubdirs; Components: {#COMPN_RANDLIB}
;
;--------------------------------------------------------------------------------------------------------------
