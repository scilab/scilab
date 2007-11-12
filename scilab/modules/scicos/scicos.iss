;##############################################################################################################
; Script d'installation Inno Setup (5.1.7) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; scicos module
;--------------------------------------------------------------------------------------------------------------
;
#define SCICOS "scicos"

Source: bin\scicos.lib; DestDir: {app}\bin; DestName: scicos.lib
Source: bin\scicos.dll; DestDir: {app}\bin; DestName: scicos.dll
;
;Source: bin\scicos_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCICOS}; Check: set_with_scicos
;Source: bin\scicos.lib; DestDir: {app}\bin; DestName: scicos.lib; Components: {#COMPN_SCICOS}
;Source: bin\scicos.dll; DestDir: {app}\bin; DestName: scicos.dll; Components: {#COMPN_SCICOS}
;Source: bin\noscicos.dll; DestDir: {app}\bin; DestName: scicos.dll; Components: not ({#COMPN_SCICOS})
;
;Source: modules\{#SCICOS}\VERSION.xml; DestDir: {app}\modules\{#SCICOS}; Components: {#COMPN_SCICOS}
;Source: modules\{#SCICOS}\readme.txt; DestDir: {app}\modules\{#SCICOS}; Components: {#COMPN_SCICOS}
;Source: modules\{#SCICOS}\licence.txt; DestDir: {app}\modules\{#SCICOS}; Components: {#COMPN_SCICOS}
;Source: modules\{#SCICOS}\changelog.txt; DestDir: {app}\modules\{#SCICOS}; Components: {#COMPN_SCICOS}
;
;Source: modules\{#SCICOS}\sci_gateway\scicos_gateway.xml; DestDir: {app}\modules\{#SCICOS}\sci_gateway; Components: {#COMPN_SCICOS}
;
;Source: modules\{#SCICOS}\etc\{#SCICOS}.quit; DestDir: {app}\modules\{#SCICOS}\etc; Components: {#COMPN_SCICOS}
;Source: modules\{#SCICOS}\etc\{#SCICOS}.start; DestDir: {app}\modules\{#SCICOS}\etc; Components: {#COMPN_SCICOS}
;
;Source: modules\{#SCICOS}\help\addchapter.sce; DestDir: {app}\modules\{#SCICOS}\help; Flags: recursesubdirs; Components:  {#COMPN_SCICOS}
;
;Source: modules\{#SCICOS}\help\*.htm; DestDir: {app}\modules\{#SCICOS}\help; Flags: recursesubdirs ;Components: {#COMPN_SCICOS}
;Source: modules\{#SCICOS}\help\images\*.gif; DestDir: {app}\modules\{#SCICOS}\help\images; Components: {#COMPN_SCICOS}
;
;
;Source: modules\{#SCICOS}\includes\*.h; DestDir: {app}\modules\{#SCICOS}\includes; Components: {#COMPN_SCICOS}
;
;Source: modules\{#SCICOS}\macros\buildmacros.sce; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
;Source: modules\{#SCICOS}\macros\buildmacros.bat; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
;Source: modules\{#SCICOS}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
;Source: modules\{#SCICOS}\macros\lib; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
;Source: modules\{#SCICOS}\macros\names; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
;Source: modules\{#SCICOS}\macros\*.bin; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
;Source: modules\{#SCICOS}\macros\*.sci; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
;Source: modules\{#SCICOS}\macros\*.sce; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
;Source: modules\{#SCICOS}\macros\*.mo; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
;Source: modules\{#SCICOS}\macros\*.moc; DestDir: {app}\modules\{#SCICOS}\macros; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
;
;Source: modules\{#SCICOS}\demos\*.*; DestDir: {app}\modules\{#SCICOS}\demos; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
;
;Source: modules\{#SCICOS}\bin\modelicac.exe; DestDir: {app}\modules\{#SCICOS}\bin; Components: {#COMPN_SCICOS}
;
;Source: modules\{#SCICOS}\tcl\*.*; DestDir: {app}\modules\{#SCICOS}\bin; Flags: recursesubdirs; Components: {#COMPN_SCICOS}
;
;Source: modules\{#SCICOS}\unit_tests\*.ref; DestDir: {app}\modules\{#SCICOS}\unit_tests; Flags: recursesubdirs; Components: {#COMPN_SCICOS} and {#COMPN_DEVTOOLS}
;Source: modules\{#SCICOS}\unit_tests\*.tst; DestDir: {app}\modules\{#SCICOS}\unit_tests; Flags: recursesubdirs; Components: {#COMPN_SCICOS} and {#COMPN_DEVTOOLS}
;
;Source: bin\.scicos_pal; DestDir: {app}\bin; Flags: ignoreversion; Components: {#COMPN_SCICOS}
;
;--------------------------------------------------------------------------------------------------------------
