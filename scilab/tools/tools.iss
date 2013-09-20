;##############################################################################################################
; Script d'installation Inno Setup (5.2.1) pour scilab
; Allan CORNET
; Version TRUNK
; Copyright INRIA 2007
;##############################################################################################################

; Repertoire tools
Source: tools\Perl\Readme.txt; DestDir: {app}\tools\Perl; Components: {#COMPN_SCILAB}
Source: tools\curl\*.*; DestDir: {app}\tools\curl; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: tools\zip\*.*; DestDir: {app}\tools\zip; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: tools\gzip\*.*; DestDir: {app}\tools\gzip; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: tools\diff\*.*; DestDir: {app}\tools\diff; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: tools\gettext\*.*; DestDir: {app}\tools\gettext; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
