;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2008
;##############################################################################################################
;--------------------------------------------------------------------------------------------------------------
; scicos_blocks module
;--------------------------------------------------------------------------------------------------------------
;
#define SCICOS_BLOCKS "scicos_blocks"
;--------------------------------------------------------------------------------------------------------------
;
Source: bin\scicos_blocks.dll; DestDir: {app}\bin; Components: {#COMPN_XCOS}
Source: bin\scicos_blocks_f.dll; DestDir: {app}\bin; Components: {#COMPN_XCOS}
;
Source: bin\scicos_blocks.lib; DestDir: {app}\bin; Components: {#COMPN_XCOS}
Source: bin\scicos_blocks_f.lib; DestDir: {app}\bin; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS_BLOCKS}\VERSION.xml; DestDir: {app}\modules\{#SCICOS_BLOCKS}; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\readme.txt; DestDir: {app}\modules\{#SCICOS_BLOCKS}; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\license.txt; DestDir: {app}\modules\{#SCICOS_BLOCKS}; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\changelog.txt; DestDir: {app}\modules\{#SCICOS_BLOCKS; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS_BLOCKS}\etc\{#SCICOS_BLOCKS}.quit; DestDir: {app}\modules\{#SCICOS_BLOCKS}\etc; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\etc\{#SCICOS_BLOCKS}.start; DestDir: {app}\modules\{#SCICOS_BLOCKS}\etc; Components: {#COMPN_XCOS}
;
;Source: modules\{#SCICOS_BLOCKS}\includes\*.h; DestDir: {app}\modules\{#SCICOS_BLOCKS}\includes; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\includes\blocks.h; DestDir: {app}\modules\{#SCICOS_BLOCKS}\includes; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\includes\scicos_block.h; DestDir: {app}\modules\{#SCICOS_BLOCKS}\includes; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\includes\scicos_block4.h; DestDir: {app}\modules\{#SCICOS_BLOCKS}\includes; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS_BLOCKS}\macros\lib; DestDir: {app}\modules\{#SCICOS_BLOCKS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\macros\names; DestDir: {app}\modules\{#SCICOS_BLOCKS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\macros\*.bin; DestDir: {app}\modules\{#SCICOS_BLOCKS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\macros\*.sci; DestDir: {app}\modules\{#SCICOS_BLOCKS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\macros\*.sce; DestDir: {app}\modules\{#SCICOS_BLOCKS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\macros\*.mo; DestDir: {app}\modules\{#SCICOS_BLOCKS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
Source: modules\{#SCICOS_BLOCKS}\macros\*.moc; DestDir: {app}\modules\{#SCICOS_BLOCKS}\macros; Flags: recursesubdirs; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS_BLOCKS}\blocks\*.h5; DestDir: {app}\modules\{#SCICOS_BLOCKS}\blocks; Flags: recursesubdirs; Components: {#COMPN_XCOS}
;
Source: bin\modelicac.exe; DestDir: {app}\bin; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS_BLOCKS}\src\scripts\*.sce; DestDir: {app}\modules\{#SCICOS_BLOCKS}\src\scripts; Components: {#COMPN_XCOS}
;
Source: modules\{#SCICOS_BLOCKS}\tests\*.*; DestDir: {app}\modules\{#SCICOS_BLOCKS}\tests; Flags: recursesubdirs; Components: {#COMPN_XCOS} and {#COMPN_TESTS}
;--------------------------------------------------------------------------------------------------------------
;
