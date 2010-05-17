; ==============================================================================
; Script d'installation Inno Setup pour scilab
; ==============================================================================
;
;-------------------------------------------------------------------------------
; parameters module
;-------------------------------------------------------------------------------
;
#define PARAMETERS "parameters"
;
Source: bin\parameters.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: modules\{#PARAMETERS}\includes\parameters.h; DestDir: {app}\modules\{#PARAMETERS}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#PARAMETERS}\VERSION.xml; DestDir: {app}\modules\{#PARAMETERS}; Components: {#COMPN_SCILAB}
Source: modules\{#PARAMETERS}\readme.txt; DestDir: {app}\modules\{#PARAMETERS}; Components: {#COMPN_SCILAB}
Source: modules\{#PARAMETERS}\license.txt; DestDir: {app}\modules\{#PARAMETERS}; Components: {#COMPN_SCILAB}
Source: modules\{#PARAMETERS}\changelog.txt; DestDir: {app}\modules\{#PARAMETERS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#PARAMETERS}\etc\{#PARAMETERS}.quit; DestDir: {app}\modules\{#PARAMETERS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#PARAMETERS}\etc\{#PARAMETERS}.start; DestDir: {app}\modules\{#PARAMETERS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#PARAMETERS}\macros\lib; DestDir: {app}\modules\{#PARAMETERS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#PARAMETERS}\macros\names; DestDir: {app}\modules\{#PARAMETERS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#PARAMETERS}\macros\*.bin; DestDir: {app}\modules\{#PARAMETERS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#PARAMETERS}\macros\*.sci; DestDir: {app}\modules\{#PARAMETERS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#PARAMETERS}\macros\buildmacros.sce; DestDir: {app}\modules\{#PARAMETERS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#PARAMETERS}\macros\buildmacros.bat; DestDir: {app}\modules\{#PARAMETERS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#PARAMETERS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#PARAMETERS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#PARAMETERS}\tests\*.*; DestDir: {app}\modules\{#PARAMETERS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;-------------------------------------------------------------------------------

