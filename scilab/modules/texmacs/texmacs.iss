;--------------------------------------------------------------------------------------------------------------
; texmacs module
;--------------------------------------------------------------------------------------------------------------
;
#define TEXMACS "texmacs"
;
Source: modules\{#TEXMACS}\VERSION; DestDir: {app}\modules\{#TEXMACS}; Components: {#COMPN_TEXMACS}; Check: set_with_texmacs
Source: modules\{#TEXMACS}\readme.txt; DestDir: {app}\modules\{#TEXMACS}; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\licence.txt; DestDir: {app}\modules\{#TEXMACS}; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\changelog.txt; DestDir: {app}\modules\{#TEXMACS}; Components: {#COMPN_TEXMACS}
;
Source: modules\{#TEXMACS}\etc\{#TEXMACS}.quit; DestDir: {app}\modules\{#TEXMACS}\etc; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\etc\{#TEXMACS}.start; DestDir: {app}\modules\{#TEXMACS}\etc; Components: {#COMPN_TEXMACS}
;
Source: modules\{#TEXMACS}\help\en_US\addchapter.sce; DestDir: {app}\modules\{#TEXMACS}\help\en_US; Components:  {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\help\en_US\removechapter.sce; DestDir: {app}\modules\{#TEXMACS}\help\en_US; Components: {#COMPN_TEXMACS}
;
Source: modules\{#TEXMACS}\help\fr_FR\addchapter.sce; DestDir: {app}\modules\{#TEXMACS}\help\fr_FR; Components:  {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\help\fr_FR\removechapter.sce; DestDir: {app}\modules\{#TEXMACS}\help\fr_FR; Components: {#COMPN_TEXMACS}
;
Source: modules\{#TEXMACS}\help\en_US\*.htm; DestDir: {app}\modules\{#TEXMACS}\help\en_US; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\help\fr_FR\*.htm; DestDir: {app}\modules\{#TEXMACS}\help\fr_FR; Components: {#COMPN_TEXMACS}
;
Source: modules\{#TEXMACS}\macros\lib; DestDir: {app}\modules\{#TEXMACS}\macros; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\macros\names; DestDir: {app}\modules\{#TEXMACS}\macros; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\macros\*.bin; DestDir: {app}\modules\{#TEXMACS}\macros; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\macros\*.sci; DestDir: {app}\modules\{#TEXMACS}\macros; Components: {#COMPN_TEXMACS}
Source: modules\{#TEXMACS}\macros\buildmacros.sce; DestDir: {app}\modules\{#TEXMACS}\macros; Components: {#COMPN_TEXMACS}
;
;Source: modules\{#TEXMACS}\demos\*.*; DestDir: {app}\modules\{#TEXMACS}\demos; Flags: recursesubdirs; Components: {#COMPN_TEXMACS}
;
;--------------------------------------------------------------------------------------------------------------
