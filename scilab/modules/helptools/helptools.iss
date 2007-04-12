;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; helptools module
;--------------------------------------------------------------------------------------------------------------
;
#define HELPTOOLS "helptools"
;
Source: modules\{#HELPTOOLS}\VERSION.xml; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\readme.txt; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\licence.txt; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\changelog.txt; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\*.htm; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\*.xsl; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\help.dtd; DestDir: {app}\modules\{#HELPTOOLS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\etc\{#HELPTOOLS}.quit; DestDir: {app}\modules\{#HELPTOOLS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\etc\{#HELPTOOLS}.start; DestDir: {app}\modules\{#HELPTOOLS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\help\addchapter.sce; DestDir: {app}\modules\{#HELPTOOLS}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\help\removechapter.sce; DestDir: {app}\modules\{#HELPTOOLS}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\languages\*.xml; DestDir: {app}\modules\{#HELPTOOLS}\languages; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\help\*.htm; DestDir: {app}\modules\{#HELPTOOLS}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\macros\buildmacros.sce; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\macros\lib; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\macros\names; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\macros\*.bin; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#HELPTOOLS}\macros\*.sci; DestDir: {app}\modules\{#HELPTOOLS}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\bin\sablotron\*.*; DestDir: {app}\modules\{#HELPTOOLS}\bin\sablotron; Components: {#COMPN_SCILAB}
;
Source: modules\{#HELPTOOLS}\examples\*.*; DestDir: {app}\modules\{#HELPTOOLS}\examples; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;--------------------------------------------------------------------------------------------------------------
