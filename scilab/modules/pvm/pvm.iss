;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; pvm module
;--------------------------------------------------------------------------------------------------------------
;
#define PVM "pvm"

;Source: bin\pvm.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\nopvm.dll; DestDir: {app}\bin; DestName: pvm.dll; Components: {#COMPN_SCILAB}

;
;Source: modules\{#PVM}\changelog.txt; DestDir: {app}\modules\{#PVM}; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\license.txt; DestDir: {app}\modules\{#PVM}; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\readme.txt; DestDir: {app}\modules\{#PVM}; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\VERSION.xml; DestDir: {app}\modules\{#PVM}; Components: {#COMPN_PVM}
;
;Source: modules\{#PVM}\includes\*.h; DestDir: {app}\modules\{#PVM}\includes; Components: {#COMPN_PVM}
;
;Source: modules\{#PVM}\sci_gateway\pvm_gateway.xml; DestDir: {app}\modules\{#PVM}\sci_gateway; Components: {#COMPN_PVM}
;
;Source: modules\{#PVM}\etc\{#PVM}.quit; DestDir: {app}\modules\{#PVM}\etc; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\etc\{#PVM}.start; DestDir: {app}\modules\{#PVM}\etc; Components: {#COMPN_PVM}
;
;Source: modules\{#PVM}\help\addchapter.sce; DestDir: {app}\modules\{#PVM}\help; Flags: recursesubdirs; Components:  {#COMPN_PVM}
;
;Source: modules\{#PVM}\macros\buildmacros.sce; DestDir: {app}\modules\{#PVM}\macros; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\macros\buildmacros.bat; DestDir: {app}\modules\{#PVM}\macros; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\macros\cleanmacros.bat; DestDir: {app}\modules\{#PVM}\macros; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\macros\*.bin; DestDir: {app}\modules\{#PVM}\macros; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\macros\*.sci; DestDir: {app}\modules\{#PVM}\macros; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\macros\lib; DestDir: {app}\modules\{#PVM}\macros; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\macros\names; DestDir: {app}\modules\{#PVM}\macros; Components: {#COMPN_PVM}
;
;Source: modules\{#PVM}\demos\*.*; DestDir: {app}\modules\{#PVM}\demos; Flags: recursesubdirs; Components: {#COMPN_PVM}
;
;Source: modules\{#PVM}\src\pvm3\Readme.Win32; DestDir: {app}\modules\{#PVM}\src\pvm3; Flags: ignoreversion; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\src\pvm3\bin\WIN32\hoster.exe; DestDir: {app}\modules\{#PVM}\src\pvm3\bin\WIN32; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\src\pvm3\bin\WIN32\pvmgs.exe; DestDir: {app}\modules\{#PVM}\src\pvm3\bin\WIN32; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\src\pvm3\include\*.h; DestDir: {app}\modules\{#PVM}\src\pvm3\include; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\src\pvm3\lib\pvm.bat; DestDir: {app}\modules\{#PVM}\src\pvm3\lib; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\src\pvm3\lib\pvmd.bat; DestDir: {app}\modules\{#PVM}\src\pvm3\lib; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\src\pvm3\lib\WIN32\*.lib; DestDir: {app}\modules\{#PVM}\src\pvm3\lib\WIN32; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\src\pvm3\lib\WIN32\*.exe; DestDir: {app}\modules\{#PVM}\src\pvm3\lib\WIN32; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\src\pvm3\lib\WIN32\*.dll; DestDir: {app}\modules\{#PVM}\src\pvm3\lib\WIN32; Components: {#COMPN_PVM}
;
;Source: modules\{#PVM}\pvmconf.bat; DestDir: {app}\modules\{#PVM}; Components: {#COMPN_PVM}
;Source: modules\{#PVM}\pvmconf.sce; DestDir: {app}\modules\{#PVM}; Components: {#COMPN_PVM}
;
;Source: modules\{#PVM}\tests\*.*; DestDir: {app}\modules\{#PVM}\tests; Flags: recursesubdirs; Components: {#COMPN_PVM} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
