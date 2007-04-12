;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; tfds module
;--------------------------------------------------------------------------------------------------------------
;
#define TFDS "tfds"
;
Source: modules\{#TFDS}\VERSION.xml; DestDir: {app}\modules\{#TFDS}; Components: {#COMPN_TFDS}; Check: set_with_tfds
Source: modules\{#TFDS}\readme.txt; DestDir: {app}\modules\{#TFDS}; Components: {#COMPN_TFDS}
Source: modules\{#TFDS}\licence.txt; DestDir: {app}\modules\{#TFDS}; Components: {#COMPN_TFDS}
Source: modules\{#TFDS}\changelog.txt; DestDir: {app}\modules\{#TFDS}; Components: {#COMPN_TFDS}
;
Source: modules\{#TFDS}\etc\{#TFDS}.quit; DestDir: {app}\modules\{#TFDS}\etc; Components: {#COMPN_TFDS}
Source: modules\{#TFDS}\etc\{#TFDS}.start; DestDir: {app}\modules\{#TFDS}\etc; Components: {#COMPN_TFDS}
;
Source: modules\{#TFDS}\help\addchapter.sce; DestDir: {app}\modules\{#TFDS}\help; Flags: recursesubdirs; Components:  {#COMPN_TFDS}
Source: modules\{#TFDS}\help\removechapter.sce; DestDir: {app}\modules\{#TFDS}\help; Flags: recursesubdirs; Components: {#COMPN_TFDS}
;
Source: modules\{#TFDS}\help\*.htm; DestDir: {app}\modules\{#TFDS}\help; Flags: recursesubdirs; Components: {#COMPN_TFDS}
;
Source: modules\{#TFDS}\languages\*.xml; DestDir: {app}\modules\{#TFDS}\languages; Flags: recursesubdirs; Components: {#COMPN_TFDS}
;
;Source: modules\{#TFDS}\includes\*.h; DestDir: {app}\modules\{#TFDS}\includes; Components: {#COMPN_TFDS}
;
Source: modules\{#TFDS}\macros\lib; DestDir: {app}\modules\{#TFDS}\macros; Components: {#COMPN_TFDS}
Source: modules\{#TFDS}\macros\names; DestDir: {app}\modules\{#TFDS}\macros; Components: {#COMPN_TFDS}
Source: modules\{#TFDS}\macros\*.bin; DestDir: {app}\modules\{#TFDS}\macros; Components: {#COMPN_TFDS}
Source: modules\{#TFDS}\macros\*.sci; DestDir: {app}\modules\{#TFDS}\macros; Components: {#COMPN_TFDS}
Source: modules\{#TFDS}\macros\buildmacros.sce; DestDir: {app}\modules\{#TFDS}\macros; Components: {#COMPN_TFDS}
;
;Source: modules\{#TFDS}\demos\*.*; DestDir: {app}\modules\{#TFDS}\demos; Flags: recursesubdirs; Components: {#COMPN_TFDS}
;
;--------------------------------------------------------------------------------------------------------------
