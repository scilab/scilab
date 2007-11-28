;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################

; Repertoire tools
;Source: tools\forwin\Forwin.exe; DestDir: {app}\tools\forwin; Components: {#COMPN_SCILAB}
Source: tools\gengatsce\Gengatsce.exe; DestDir: {app}\tools\gengatsce; Components: {#COMPN_SCILAB}
Source: tools\gensomex\Gensomex.exe; DestDir: {app}\tools\gensomex; Components: {#COMPN_SCILAB}
Source: tools\gensosce\Gensosce.exe; DestDir: {app}\tools\gensosce; Components: {#COMPN_SCILAB}
;Source: tools\lib\Lib.exe; DestDir: {app}\tools\lib; Components: {#COMPN_SCILAB}
;Source: tools\name\Name.exe; DestDir: {app}\tools\name; Components: {#COMPN_SCILAB}
Source: tools\Perl\Readme.txt; DestDir: {app}\tools\Perl; Components: {#COMPN_SCILAB}
;Source: tools\printer\BEpsf.exe; DestDir: {app}\tools\printer; Components: {#COMPN_SCILAB}
;Source: tools\printer\Blatdoc.exe; DestDir: {app}\tools\printer; Components: {#COMPN_SCILAB}
;Source: tools\printer\Blatdocs.exe; DestDir: {app}\tools\printer; Components: {#COMPN_SCILAB}
;Source: tools\printer\Blatexpr2.exe; DestDir: {app}\tools\printer; Components: {#COMPN_SCILAB}
;Source: tools\printer\Blatexpr.exe; DestDir: {app}\tools\printer; Components: {#COMPN_SCILAB}
;Source: tools\printer\Blatexprs.exe; DestDir: {app}\tools\printer; Components: {#COMPN_SCILAB}
;Source: tools\printer\Blpr.exe; DestDir: {app}\tools\printer; Components: {#COMPN_SCILAB}
;Source: tools\printer\lpr\lpr.exe; DestDir: {app}\tools\printer; Components: {#COMPN_SCILAB}
;Source: tools\tomake\Tomake.exe; DestDir: {app}\tools\tomake; Components: {#COMPN_SCILAB}
Source: tools\curl\*.*; DestDir: {app}\tools\curl; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: tools\zip\*.*; DestDir: {app}\tools\zip; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
