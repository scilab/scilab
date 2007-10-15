;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; optimization module
;--------------------------------------------------------------------------------------------------------------
;
#define OPTIMIZATION "optimization"
;
Source: modules\{#OPTIMIZATION}\VERSION.xml; DestDir: {app}\modules\{#OPTIMIZATION}; Components: {#COMPN_OPTIM}; Check: set_with_optimization
Source: modules\{#OPTIMIZATION}\readme.txt; DestDir: {app}\modules\{#OPTIMIZATION}; Components: {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\licence.txt; DestDir: {app}\modules\{#OPTIMIZATION}; Components: {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\changelog.txt; DestDir: {app}\modules\{#OPTIMIZATION}; Components: {#COMPN_OPTIM}
;
Source: modules\{#OPTIMIZATION}\sci_gateway\optimization_gateway.xml; DestDir: {app}\modules\{#OPTIMIZATION}\sci_gateway; Components: {#COMPN_OPTIM}
;
Source: modules\{#OPTIMIZATION}\etc\{#OPTIMIZATION}.quit; DestDir: {app}\modules\{#OPTIMIZATION}\etc; Components: {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\etc\{#OPTIMIZATION}.start; DestDir: {app}\modules\{#OPTIMIZATION}\etc; Components: {#COMPN_OPTIM}
;
Source: modules\{#OPTIMIZATION}\help\addchapter.sce; DestDir: {app}\modules\{#OPTIMIZATION}\help; Flags: recursesubdirs; Components:  {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\help\removechapter.sce; DestDir: {app}\modules\{#OPTIMIZATION}\help; Flags: recursesubdirs; Components: {#COMPN_OPTIM}
;
Source: modules\{#OPTIMIZATION}\help\*.htm; DestDir: {app}\modules\{#OPTIMIZATION}\help; Flags: recursesubdirs; Components: {#COMPN_OPTIM}
;
Source: modules\{#OPTIMIZATION}\locales\*.xml; DestDir: {app}\modules\{#OPTIMIZATION}\locales; Flags: recursesubdirs; Components: {#COMPN_OPTIM}
;
Source: modules\{#OPTIMIZATION}\includes\*.h; DestDir: {app}\modules\{#OPTIMIZATION}\includes; Components: {#COMPN_OPTIM}
;
Source: modules\{#OPTIMIZATION}\macros\buildmacros.sce; DestDir: {app}\modules\{#OPTIMIZATION}\macros; Components: {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\macros\buildmacros.bat; DestDir: {app}\modules\{#OPTIMIZATION}\macros; Components: {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\macros\cleanmacros.bat; DestDir: {app}\modules\{#OPTIMIZATION}\macros; Components: {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\macros\lib; DestDir: {app}\modules\{#OPTIMIZATION}\macros; Components: {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\macros\names; DestDir: {app}\modules\{#OPTIMIZATION}\macros; Components: {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\macros\*.bin; DestDir: {app}\modules\{#OPTIMIZATION}\macros; Components: {#COMPN_OPTIM}
Source: modules\{#OPTIMIZATION}\macros\*.sci; DestDir: {app}\modules\{#OPTIMIZATION}\macros; Components: {#COMPN_OPTIM}
;
Source: modules\{#OPTIMIZATION}\demos\*.*; DestDir: {app}\modules\{#OPTIMIZATION}\demos; Flags: recursesubdirs; Components: {#COMPN_OPTIM}
;
Source: modules\{#OPTIMIZATION}\unit_tests\*.ref; DestDir: {app}\modules\{#OPTIMIZATION}\unit_tests; Flags: recursesubdirs; Components: {#COMPN_OPTIM} and {#COMPN_DEVTOOLS}
Source: modules\{#OPTIMIZATION}\unit_tests\*.tst; DestDir: {app}\modules\{#OPTIMIZATION}\unit_tests; Flags: recursesubdirs; Components: {#COMPN_OPTIM} and {#COMPN_DEVTOOLS}
;
;
;--------------------------------------------------------------------------------------------------------------
