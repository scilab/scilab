; ==============================================================================
; Script d'installation Inno Setup pour scilab
; Version TRUNK
; Copyright INRIA 2008
; ==============================================================================

;-------------------------------------------------------------------------------
; simulated_annealing module
;-------------------------------------------------------------------------------
;
#define SIMANNEALING "simulated_annealing"
;
Source: modules\{#SIMANNEALING}\VERSION.xml; DestDir: {app}\modules\{#SIMANNEALING}; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\readme.txt; DestDir: {app}\modules\{#SIMANNEALING}; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\license.txt; DestDir: {app}\modules\{#SIMANNEALING}; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\changelog.txt; DestDir: {app}\modules\{#SIMANNEALING}; Components: {#COMPN_SCILAB}
;
Source: modules\{#SIMANNEALING}\etc\{#SIMANNEALING}.quit; DestDir: {app}\modules\{#SIMANNEALING}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\etc\{#SIMANNEALING}.start; DestDir: {app}\modules\{#SIMANNEALING}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#SIMANNEALING}\examples\*.*; DestDir: {app}\modules\{#SIMANNEALING}\examples; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#SIMANNEALING}\Work\*.*; DestDir: {app}\modules\{#SIMANNEALING}\Work; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#SIMANNEALING}\help\addchapter.sce; DestDir: {app}\modules\{#SIMANNEALING}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#SIMANNEALING}\macros\lib; DestDir: {app}\modules\{#SIMANNEALING}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\macros\names; DestDir: {app}\modules\{#SIMANNEALING}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\macros\*.bin; DestDir: {app}\modules\{#SIMANNEALING}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\macros\*.sci; DestDir: {app}\modules\{#SIMANNEALING}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\macros\buildmacros.sce; DestDir: {app}\modules\{#SIMANNEALING}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\macros\buildmacros.bat; DestDir: {app}\modules\{#SIMANNEALING}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#SIMANNEALING}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SIMANNEALING}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#SIMANNEALING}\tests\*.*; DestDir: {app}\modules\{#SIMANNEALING}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
