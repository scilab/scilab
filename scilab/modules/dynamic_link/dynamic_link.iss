;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; dynamic_link module
;--------------------------------------------------------------------------------------------------------------
;
#define DYNAMIC_LINK "dynamic_link"
;
Source: bin\{#DYNAMIC_LINK}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#DYNAMIC_LINK}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\dynamic_link_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\dynamic_link_f.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}

;
Source: modules\{#DYNAMIC_LINK}\VERSION.xml; DestDir: {app}\modules\{#DYNAMIC_LINK}; Components: {#COMPN_SCILAB}
Source: modules\{#DYNAMIC_LINK}\readme.txt; DestDir: {app}\modules\{#DYNAMIC_LINK}; Components: {#COMPN_SCILAB}
Source: modules\{#DYNAMIC_LINK}\licence.txt; DestDir: {app}\modules\{#DYNAMIC_LINK}; Components: {#COMPN_SCILAB}
Source: modules\{#DYNAMIC_LINK}\changelog.txt; DestDir: {app}\modules\{#DYNAMIC_LINK}; Components: {#COMPN_SCILAB}
;
Source: modules\{#DYNAMIC_LINK}\etc\{#DYNAMIC_LINK}.quit; DestDir: {app}\modules\{#DYNAMIC_LINK}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#DYNAMIC_LINK}\etc\{#DYNAMIC_LINK}.start; DestDir: {app}\modules\{#DYNAMIC_LINK}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#DYNAMIC_LINK}\sci_gateway\dynamic_link_gateway.xml; DestDir: {app}\modules\{#DYNAMIC_LINK}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#DYNAMIC_LINK}\help\addchapter.sce; DestDir: {app}\modules\{#DYNAMIC_LINK}\help; Flags: recursesubdirs; Components:  {#COMPN_SCILAB}
;
Source: modules\{#DYNAMIC_LINK}\help\*.htm; DestDir: {app}\modules\{#DYNAMIC_LINK}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#DYNAMIC_LINK}\src\scripts\Makedll.incl; DestDir: {app}\modules\{#DYNAMIC_LINK}\src\scripts; Components: {#COMPN_SCILAB}
Source: modules\{#DYNAMIC_LINK}\src\scripts\TEMPLATE_MAKEFILE.VC; DestDir: {app}\modules\{#DYNAMIC_LINK}\src\scripts; Components: {#COMPN_SCILAB}
;
Source: modules\{#DYNAMIC_LINK}\includes\*.h; DestDir: {app}\modules\{#DYNAMIC_LINK}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#DYNAMIC_LINK}\macros\lib; DestDir: {app}\modules\{#DYNAMIC_LINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#DYNAMIC_LINK}\macros\names; DestDir: {app}\modules\{#DYNAMIC_LINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#DYNAMIC_LINK}\macros\*.bin; DestDir: {app}\modules\{#DYNAMIC_LINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#DYNAMIC_LINK}\macros\*.sci; DestDir: {app}\modules\{#DYNAMIC_LINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#DYNAMIC_LINK}\macros\buildmacros.sce; DestDir: {app}\modules\{#DYNAMIC_LINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#DYNAMIC_LINK}\macros\buildmacros.bat; DestDir: {app}\modules\{#DYNAMIC_LINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#DYNAMIC_LINK}\macros\cleanmacros.bat; DestDir: {app}\modules\{#DYNAMIC_LINK}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;Source: modules\{#DYNAMIC_LINK}\demos\*.*; DestDir: {app}\modules\{#DYNAMIC_LINK}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;Source: modules\{#DYNAMIC_LINK}\examples\*.*; DestDir: {app}\modules\{#DYNAMIC_LINK}\examples; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#DYNAMIC_LINK}\tests\*.*; DestDir: {app}\modules\{#DYNAMIC_LINK}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
