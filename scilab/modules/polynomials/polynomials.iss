;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; polynomials module
;--------------------------------------------------------------------------------------------------------------
;
#define POLYNOMIALS "polynomials"
;
Source: modules\{#POLYNOMIALS}\VERSION; DestDir: {app}\modules\{#POLYNOMIALS}; Components: {#COMPN_POLY}; Check: set_with_polynomials
Source: modules\{#POLYNOMIALS}\readme.txt; DestDir: {app}\modules\{#POLYNOMIALS}; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\licence.txt; DestDir: {app}\modules\{#POLYNOMIALS}; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\changelog.txt; DestDir: {app}\modules\{#POLYNOMIALS}; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\sci_gateway\polynomials; DestDir: {app}\modules\{#POLYNOMIALS}\sci_gateway; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\etc\{#POLYNOMIALS}.quit; DestDir: {app}\modules\{#POLYNOMIALS}\etc; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\etc\{#POLYNOMIALS}.start; DestDir: {app}\modules\{#POLYNOMIALS}\etc; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\help\en_US\addchapter.sce; DestDir: {app}\modules\{#POLYNOMIALS}\help\en_US; Components:  {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\help\en_US\removechapter.sce; DestDir: {app}\modules\{#POLYNOMIALS}\help\en_US; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\help\fr_FR\addchapter.sce; DestDir: {app}\modules\{#POLYNOMIALS}\help\fr_FR; Components:  {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\help\fr_FR\removechapter.sce; DestDir: {app}\modules\{#POLYNOMIALS}\help\fr_FR; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\help\en_US\*.htm; DestDir: {app}\modules\{#POLYNOMIALS}\help\en_US; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\help\fr_FR\*.htm; DestDir: {app}\modules\{#POLYNOMIALS}\help\fr_FR; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\languages\en_US\*.xml; DestDir: {app}\modules\{#POLYNOMIALS}\languages\en_US; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\languages\fr_FR\*.xml; DestDir: {app}\modules\{#POLYNOMIALS}\languages\fr_FR; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\includes\*.h; DestDir: {app}\modules\{#POLYNOMIALS}\includes; Components: {#COMPN_POLY}
;
Source: modules\{#POLYNOMIALS}\macros\buildmacros.sce; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\macros\lib; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\macros\names; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\macros\*.bin; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_POLY}
Source: modules\{#POLYNOMIALS}\macros\*.sci; DestDir: {app}\modules\{#POLYNOMIALS}\macros; Components: {#COMPN_POLY}
;
;Source: modules\{#POLYNOMIALS}\demos\*.*; DestDir: {app}\modules\{#POLYNOMIALS}\demos; Flags: recursesubdirs; Components: {#COMPN_POLY}
;
;--------------------------------------------------------------------------------------------------------------
