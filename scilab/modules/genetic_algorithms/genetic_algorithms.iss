; ==============================================================================
; Script d'installation Inno Setup pour scilab
; Version TRUNK
; Copyright INRIA 2008
; ==============================================================================
;
;-------------------------------------------------------------------------------
; genetic_algorithms module
;-------------------------------------------------------------------------------
;
#define GENETICALGORITHMS "genetic_algorithms"
;
Source: modules\{#GENETICALGORITHMS}\VERSION.xml; DestDir: {app}\modules\{#GENETICALGORITHMS}; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\readme.txt; DestDir: {app}\modules\{#GENETICALGORITHMS}; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\license.txt; DestDir: {app}\modules\{#GENETICALGORITHMS}; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\changelog.txt; DestDir: {app}\modules\{#GENETICALGORITHMS}; Components: {#COMPN_SCILAB}
;
Source: modules\{#GENETICALGORITHMS}\etc\{#GENETICALGORITHMS}.quit; DestDir: {app}\modules\{#GENETICALGORITHMS}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\etc\{#GENETICALGORITHMS}.start; DestDir: {app}\modules\{#GENETICALGORITHMS}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#GENETICALGORITHMS}\examples\*.*; DestDir: {app}\modules\{#GENETICALGORITHMS}\examples; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#GENETICALGORITHMS}\help\addchapter.sce; DestDir: {app}\modules\{#GENETICALGORITHMS}\help; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#GENETICALGORITHMS}\macros\lib; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\macros\names; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\macros\*.bin; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\macros\*.sci; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\macros\buildmacros.sce; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\macros\buildmacros.bat; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#GENETICALGORITHMS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#GENETICALGORITHMS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#GENETICALGORITHMS}\tests\*.*; DestDir: {app}\modules\{#GENETICALGORITHMS}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
