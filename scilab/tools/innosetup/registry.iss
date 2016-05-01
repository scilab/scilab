;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2010 - Allan CORNET
;
; Copyright (C) 2012 - 2016 - Scilab Enterprises
;
; This file is hereby licensed under the terms of the GNU GPL v2.0,
; pursuant to article 5.3.4 of the CeCILL v.2.1.
; This file was originally licensed under the terms of the CeCILL v2.1,
; and continues to be available under such terms.
; For more information, see the COPYING file which you should have received
; along with this program.
;
;-------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;-------------------------------------------------------------------------------

; files assocociation .sce .sci .tst .bin .sav .sod
;-------------------------------------------------------------------------------
; Files Association (registry input)
;-------------------------------------------------------------------------------
;http://msdn2.microsoft.com/en-us/library/bb776870(VS.85).aspx
;http://msdn2.microsoft.com/en-us/library/bb776883.aspx
;http://msdn2.microsoft.com/en-us/library/bb776820.aspx
;-------------------------------------------------------------------------------
;Scilab PATH
Root: HKLM; Subkey: "SOFTWARE\Scilab"; ValueType: string; ValueData: {#ScilabName}; Flags: noerror; ValueName: LASTINSTALL
;-------------------------------------------------------------------------------
Root: HKLM; Subkey: "SOFTWARE\Scilab\{#ScilabName}"; ValueType: string; ValueData: {app}; Flags: createvalueifdoesntexist uninsdeletekey noerror; ValueName: SCIPATH
Root: HKLM; Subkey: "SOFTWARE\Scilab\{#ScilabName}"; ValueType: string; ValueData: {cm:LANGUAGE}; Flags: createvalueifdoesntexist uninsdeletekey noerror; ValueName: LANGUAGE
;-------------------------------------------------------------------------------
#ifdef SCILAB_X64
Root: HKLM; Subkey: "SOFTWARE\Scilab\{#ScilabName}"; ValueType: string; ValueData: X64; Flags: createvalueifdoesntexist uninsdeletekey noerror; ValueName: OS_ARCH
#else
Root: HKLM; Subkey: "SOFTWARE\Scilab\{#ScilabName}"; ValueType: string; ValueData: X86; Flags: createvalueifdoesntexist uninsdeletekey noerror; ValueName: OS_ARCH
#endif
;-------------------------------------------------------------------------------
#define ARGUMENT_ACTION_OPEN "-O"
#define ARGUMENT_ACTION_RUN "-X"
#define ARGUMENT_ACTION_PRINT "-P"
;-------------------------------------------------------------------------------
;Files .sce
#define APPLICATION_EXTENSION_SCE "Scilab5.sce"
#define SCE_ENTRY ".sce"
#define ICON_SCE_POS 7
;
Root: HKCR; Subkey: {#SCE_ENTRY}; ValueType: string; ValueData: {#APPLICATION_EXTENSION_SCE}; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCESCI
Root: HKCR; Subkey: {#SCE_ENTRY}\ShellNew; ValueName : NullFile ;ValueType: string; ValueData: ""; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCESCI
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_SCE}; ValueType: string; ValueData: "{#ScilabName} Application ({#SCE_ENTRY})"; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCESCI
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_SCE}\DefaultIcon; ValueType: string; ValueData: {app}\bin\wscilex.exe,{#ICON_SCE_POS}; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCESCI
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_SCE}\shell\open\command; ValueType: string; ValueData: """{app}\bin\wscilex.exe"" {#ARGUMENT_ACTION_OPEN} ""%1"""; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCESCI
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_SCE}\shell\{cm:RegKeyRunWith} {#ScilabName}\command; ValueType: string; ValueData: """{app}\bin\wscilex.exe"" {#ARGUMENT_ACTION_RUN} ""%1"""; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCESCI;
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_SCE}\shell\print\command; ValueType: string; ValueData: """{app}\bin\wscilex.exe"" {#ARGUMENT_ACTION_PRINT} ""%1"""; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCESCI
;
;-------------------------------------------------------------------------------
;Files .sci
#define APPLICATION_EXTENSION_SCI "Scilab5.sci"
#define SCI_ENTRY ".sci"
#define ICON_SCI_POS 8
;
Root: HKCR; Subkey: {#SCI_ENTRY}; ValueType: string; ValueData: {#APPLICATION_EXTENSION_SCI}; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCESCI
Root: HKCR; Subkey: {#SCI_ENTRY}\ShellNew; ValueName : NullFile ; ValueType: string; ValueData: ""; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCESCI
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_SCI}; ValueType: string; ValueData: "{#ScilabName} Application ({#SCI_ENTRY})"; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCESCI
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_SCI}\DefaultIcon; ValueType: string; ValueData: {app}\bin\wscilex.exe,{#ICON_SCI_POS}; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCESCI
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_SCI}\shell\open\command; ValueType: string; ValueData: """{app}\bin\wscilex.exe"" {#ARGUMENT_ACTION_OPEN} ""%1"""; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCESCI
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_SCI}\shell\{cm:RegKeyRunWith} {#ScilabName}\command; ValueType: string; ValueData: """{app}\bin\wscilex.exe"" {#ARGUMENT_ACTION_RUN} ""%1"""; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCESCI;
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_SCI}\shell\print\command; ValueType: string; ValueData: """{app}\bin\wscilex.exe"" {#ARGUMENT_ACTION_PRINT} ""%1"""; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCESCI
;
;-------------------------------------------------------------------------------
;Files .tst
#define APPLICATION_EXTENSION_TST "Scilab5.tst"
#define TST_ENTRY ".tst"
#define ICON_TST_POS 9
;
Root: HKCR; Subkey: {#TST_ENTRY}; ValueType: string; ValueData: {#APPLICATION_EXTENSION_TST}; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateTSTDEM
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_TST}; ValueType: string; ValueData: "{#ScilabName} Application"; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateTSTDEM
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_TST}\DefaultIcon; ValueType: string; ValueData: {app}\bin\wscilex.exe,{#ICON_TST_POS}; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateTSTDEM
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_TST}\shell\open\command; ValueType: string; ValueData: """{app}\bin\wscilex.exe"" {#ARGUMENT_ACTION_OPEN} ""%1"""; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateTSTDEM
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_TST}\shell\{cm:RegKeyRunWith} {#ScilabName}\command; ValueType: string; ValueData: """{app}\bin\wscilex.exe"" {#ARGUMENT_ACTION_RUN} ""%1"""; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateTSTDEM;
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_TST}\shell\print\command; ValueType: string; ValueData: """{app}\bin\wscilex.exe"" {#ARGUMENT_ACTION_PRINT} ""%1"""; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateTSTDEM
;
;-------------------------------------------------------------------------------
;Files .dem
#define APPLICATION_EXTENSION_DEM "Scilab5.dem"
#define DEM_ENTRY ".dem"
#define ICON_DEM_POS 4
;
Root: HKCR; Subkey: {#DEM_ENTRY}; ValueType: string; ValueData: {#APPLICATION_EXTENSION_DEM}; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateTSTDEM
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_DEM}; ValueType: string; ValueData: "{#ScilabName} Application"; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateTSTDEM
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_DEM}\DefaultIcon; ValueType: string; ValueData: {app}\bin\wscilex.exe,{#ICON_DEM_POS}; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateTSTDEM
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_DEM}\shell\open\command; ValueType: string; ValueData: """{app}\bin\wscilex.exe"" {#ARGUMENT_ACTION_OPEN} ""%1"""; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateTSTDEM
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_DEM}\shell\{cm:RegKeyRunWith} {#ScilabName}\command; ValueType: string; ValueData: """{app}\bin\wscilex.exe"" {#ARGUMENT_ACTION_RUN} ""%1"""; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateTSTDEM;
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_DEM}\shell\print\command; ValueType: string; ValueData: """{app}\bin\wscilex.exe"" {#ARGUMENT_ACTION_PRINT} ""%1"""; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateTSTDEM
;
;-------------------------------------------------------------------------------
;Files .zcos
#define APPLICATION_EXTENSION_ZCOS "Scilab5.zcos"
#define ZCOS_ENTRY ".zcos"
#define ICON_ZCOS_POS 11
;
Root: HKCR; Subkey: {#ZCOS_ENTRY}; ValueType: string; ValueData: {#APPLICATION_EXTENSION_ZCOS}; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCICOS
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_ZCOS}; ValueType: string; ValueData: "{#ScilabName} Application"; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCICOS
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_ZCOS}\DefaultIcon; ValueType: string; ValueData: {app}\bin\wscilex.exe,{#ICON_ZCOS_POS}; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCICOS
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_ZCOS}\shell\{cm:RegKeyRunWith} {#ScilabName}\command; ValueType: string; ValueData: """{app}\bin\wscilex.exe"" {#ARGUMENT_ACTION_RUN} ""%1"""; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCICOS;
;
;-------------------------------------------------------------------------------
;Files .xcos
#define APPLICATION_EXTENSION_XCOS "Scilab5.xcos"
#define XCOS_ENTRY ".xcos"
#define ICON_XCOS_POS 2
;
Root: HKCR; Subkey: {#XCOS_ENTRY}; ValueType: string; ValueData: {#APPLICATION_EXTENSION_XCOS}; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCICOS
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_XCOS}; ValueType: string; ValueData: "{#ScilabName} Application"; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCICOS
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_XCOS}\DefaultIcon; ValueType: string; ValueData: {app}\bin\wscilex.exe,{#ICON_XCOS_POS}; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCICOS
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_XCOS}\shell\{cm:RegKeyRunWith} {#ScilabName}\command; ValueType: string; ValueData: """{app}\bin\wscilex.exe"" {#ARGUMENT_ACTION_RUN} ""%1"""; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCICOS;
;
;-------------------------------------------------------------------------------
;Files .cos
#define APPLICATION_EXTENSION_COS "Scilab5.cos"
#define COS_ENTRY ".cos"
#define ICON_COS_POS 2
;
Root: HKCR; Subkey: {#COS_ENTRY}; ValueType: string; ValueData: {#APPLICATION_EXTENSION_COS}; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCICOS
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_COS}; ValueType: string; ValueData: "{#ScilabName} Application"; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCICOS
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_COS}\DefaultIcon; ValueType: string; ValueData: {app}\bin\wscilex.exe,{#ICON_COS_POS}; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCICOS
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_COS}\shell\{cm:RegKeyRunWith} {#ScilabName}\command; ValueType: string; ValueData: """{app}\bin\wscilex.exe"" {#ARGUMENT_ACTION_RUN} ""%1"""; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCICOS;
;
;-------------------------------------------------------------------------------
;Files .cosf
#define APPLICATION_EXTENSION_COSF "Scilab5.cosf"
#define COSF_ENTRY ".cosf"
#define ICON_COSF_POS 3
;
Root: HKCR; Subkey: {#COSF_ENTRY}; ValueType: string; ValueData: {#APPLICATION_EXTENSION_COSF}; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCICOS
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_COSF}; ValueType: string; ValueData: "{#ScilabName} Application"; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCICOS
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_COSF}\DefaultIcon; ValueType: string; ValueData: {app}\bin\wscilex.exe,{#ICON_COSF_POS}; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCICOS
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_COSF}\shell\{cm:RegKeyRunWith} {#ScilabName}\command; ValueType: string; ValueData: """{app}\bin\wscilex.exe"" {#ARGUMENT_ACTION_RUN} ""%1"""; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSCICOS;
;-------------------------------------------------------------------------------
;Files .sod
#define APPLICATION_EXTENSION_SOD "Scilab5.sod"
#define SOD_ENTRY ".sod"
#define ICON_SOD_POS 10
;
Root: HKCR; Subkey: {#SOD_ENTRY}; ValueType: string; ValueData: {#APPLICATION_EXTENSION_SOD}; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSOD
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_SOD}; ValueType: string; ValueData: "{#ScilabName} Application"; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSOD
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_SOD}\DefaultIcon; ValueType: string; ValueData: {app}\bin\wscilex.exe,{#ICON_SOD_POS}; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSOD
;
Root: HKCR; Subkey: {#APPLICATION_EXTENSION_SOD}\shell\{cm:RegKeyRunWith} {#ScilabName}\command; ValueType: string; ValueData: """{app}\bin\wscilex.exe"" {#ARGUMENT_ACTION_RUN} ""%1"""; Flags: deletekey uninsdeletekey noerror; Tasks: AssociateSOD;
;-------------------------------------------------------------------------------
Root: HKCU; Subkey: SOFTWARE\Scilab\{#ScilabName}\Settings; ValueType: string; ValueData: {cm:LANGUAGE}; Flags: createvalueifdoesntexist uninsdeletekey noerror; ValueName: LANGUAGE
;
;-------------------------------------------------------------------------------
;
