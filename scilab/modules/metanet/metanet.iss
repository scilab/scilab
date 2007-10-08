;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; metanet module
;--------------------------------------------------------------------------------------------------------------
;
#define METANET "metanet"
;
Source: bin\metanet_f.dll; DestDir: {app}\bin; Components: {#COMPN_METANET}; Check: set_with_metanet
Source: bin\metanet.dll; DestDir: {app}\bin; DestName: metanet.dll; Components: {#COMPN_METANET}
Source: bin\nometanet.dll; DestDir: {app}\bin; DestName: metanet.dll; Components: not ({#COMPN_METANET})
;
Source: modules\{#METANET}\VERSION.xml; DestDir: {app}\modules\{#METANET}; Components: {#COMPN_METANET}
Source: modules\{#METANET}\readme.txt; DestDir: {app}\modules\{#METANET}; Components: {#COMPN_METANET}
Source: modules\{#METANET}\licence.txt; DestDir: {app}\modules\{#METANET}; Components: {#COMPN_METANET}
Source: modules\{#METANET}\changelog.txt; DestDir: {app}\modules\{#METANET}; Components: {#COMPN_METANET}
;
Source: modules\{#METANET}\sci_gateway\metanet_gateway.xml; DestDir: {app}\modules\{#METANET}\sci_gateway; Components: {#COMPN_METANET}
;
Source: modules\{#METANET}\etc\{#METANET}.quit; DestDir: {app}\modules\{#METANET}\etc; Components: {#COMPN_METANET}
Source: modules\{#METANET}\etc\{#METANET}.start; DestDir: {app}\modules\{#METANET}\etc; Components: {#COMPN_METANET}
;
Source: modules\{#METANET}\help\addchapter.sce; DestDir: {app}\modules\{#METANET}\help; Flags: recursesubdirs; Components:  {#COMPN_METANET}
Source: modules\{#METANET}\help\removechapter.sce; DestDir: {app}\modules\{#METANET}\help; Flags: recursesubdirs; Components: {#COMPN_METANET}
;
Source: modules\{#METANET}\help\*.htm; DestDir: {app}\modules\{#METANET}\help; Flags: recursesubdirs; Components: {#COMPN_METANET}
;
Source: modules\{#METANET}\locales\*.xml; DestDir: {app}\modules\{#METANET}\locales; Flags: recursesubdirs; Components: {#COMPN_METANET}
;
Source: modules\{#METANET}\includes\*.h; DestDir: {app}\modules\{#METANET}\includes; Components: {#COMPN_METANET}
;
Source: modules\{#METANET}\macros\lib; DestDir: {app}\modules\{#METANET}\macros; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\names; DestDir: {app}\modules\{#METANET}\macros; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\*.bin; DestDir: {app}\modules\{#METANET}\macros; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\*.sci; DestDir: {app}\modules\{#METANET}\macros; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\buildmacros.sce; DestDir: {app}\modules\{#METANET}\macros; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\buildmacros.bat; DestDir: {app}\modules\{#METANET}\macros; Components: {#COMPN_METANET}
Source: modules\{#METANET}\macros\cleanmacros.bat; DestDir: {app}\modules\{#METANET}\macros; Components: {#COMPN_METANET}
;
Source: modules\{#METANET}\demos\*.*; DestDir: {app}\modules\{#METANET}\demos; Flags: recursesubdirs; Components: {#COMPN_METANET}
;
;--------------------------------------------------------------------------------------------------------------
