;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; symbolic module
;--------------------------------------------------------------------------------------------------------------
;
#define SYMBOLIC "symbolic"
;
Source: bin\symbolic.dll; DestDir: {app}\bin; DestName: symbolic.dll; Components: {#COMPN_SYMB}
Source: bin\symbolic_f.dll; DestDir: {app}\bin; DestName: symbolic_f.dll; Components: {#COMPN_SYMB}
;
Source: modules\{#SYMBOLIC}\VERSION; DestDir: {app}\modules\{#SYMBOLIC}; Components: {#COMPN_SYMB}; Check: set_with_symbolic
Source: modules\{#SYMBOLIC}\readme.txt; DestDir: {app}\modules\{#SYMBOLIC}; Components: {#COMPN_SYMB}
Source: modules\{#SYMBOLIC}\licence.txt; DestDir: {app}\modules\{#SYMBOLIC}; Components: {#COMPN_SYMB}
Source: modules\{#SYMBOLIC}\changelog.txt; DestDir: {app}\modules\{#SYMBOLIC}; Components: {#COMPN_SYMB}
;
Source: modules\{#SYMBOLIC}\sci_gateway\symbolic_gateway.xml; DestDir: {app}\modules\{#SYMBOLIC}\sci_gateway; Components: {#COMPN_SYMB}
;
Source: modules\{#SYMBOLIC}\etc\{#SYMBOLIC}.quit; DestDir: {app}\modules\{#SYMBOLIC}\etc; Components: {#COMPN_SYMB}
Source: modules\{#SYMBOLIC}\etc\{#SYMBOLIC}.start; DestDir: {app}\modules\{#SYMBOLIC}\etc; Components: {#COMPN_SYMB}
;
Source: modules\{#SYMBOLIC}\help\addchapter.sce; DestDir: {app}\modules\{#SYMBOLIC}\help; Flags: recursesubdirs; Components:   {#COMPN_SYMB}
Source: modules\{#SYMBOLIC}\help\removechapter.sce; DestDir: {app}\modules\{#SYMBOLIC}\help; Flags: recursesubdirs; Components:  {#COMPN_SYMB}
;
Source: modules\{#SYMBOLIC}\help\*.htm; DestDir: {app}\modules\{#SYMBOLIC}\help; Flags: recursesubdirs; Components: {#COMPN_SYMB}
;
Source: modules\{#SYMBOLIC}\languages\*.xml; DestDir: {app}\modules\{#SYMBOLIC}\languages; Flags: recursesubdirs; Components: {#COMPN_SYMB}
;
Source: modules\{#SYMBOLIC}\includes\*.h; DestDir: {app}\modules\{#SYMBOLIC}\includes; Components: {#COMPN_SYMB}
;
;Source: modules\{#SYMBOLIC}\macros\lib; DestDir: {app}\modules\{#SYMBOLIC}\macros; Flags: recursesubdirs; Components: {#COMPN_SYMB}
;Source: modules\{#SYMBOLIC}\macros\names; DestDir: {app}\modules\{#SYMBOLIC}\macros; Flags: recursesubdirs; Components: {#COMPN_SYMB}
;Source: modules\{#SYMBOLIC}\macros\*.bin; DestDir: {app}\modules\{#SYMBOLIC}\macros; Flags: recursesubdirs; Components: {#COMPN_SYMB}
;Source: modules\{#SYMBOLIC}\macros\*.sci; DestDir: {app}\modules\{#SYMBOLIC}\macros; Flags: recursesubdirs; Components: {#COMPN_SYMB}
Source: modules\{#SYMBOLIC}\macros\buildmacros.sce; DestDir: {app}\modules\{#SYMBOLIC}\macros; Flags: recursesubdirs; Components: {#COMPN_SYMB}
;
;Source: modules\{#SYMBOLIC}\demos\*.*; DestDir: {app}\modules\{#SYMBOLIC}\demos; Flags: recursesubdirs; Components: {#COMPN_SYMB}
;
;--------------------------------------------------------------------------------------------------------------
