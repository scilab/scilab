;
; Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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

; data to modify with version
#ifdef SCILAB_X64
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
#endif
#ifdef SCILAB_ARM64
ArchitecturesAllowed=arm64
ArchitecturesInstallIn64BitMode=arm64
#endif
; Detect if scilab runs
AppMutex={#ScilabBaseDirectory}
;
SourceDir={#BinariesSourcePath}
#ifndef SCILAB_WITHOUT_JRE
OutputBaseFilename={#ScilabBaseFilename}
#else
OutputBaseFilename={#ScilabBaseFilename}-nojre
#endif
AppName={#ScilabName}
AppVerName={#ScilabName}
AppVersion={#ScilabVersion}
;always shown welcome page
DisableWelcomePage=no
;always shown destination path page
DisableDirPage=no
DefaultDirName={code:DefDirRoot}\{#ScilabBaseDirectory}
DefaultGroupName={#ScilabName}
SetupIconFile=tools\innosetup\scilab.ico
LicenseFile=COPYING
ChangesAssociations=yes
WindowVisible=false
AppPublisher=Dassault Systèmes S.E.
AppPublisherURL=https://www.scilab.org/
AppSupportURL=https://gitlab.com/scilab/scilab/-/issues
AppUpdatesURL=https://www.scilab.org/download/
WizardImageStretch=no
WizardImageBackColor=clBlack
WizardImageFile=tools\innosetup\ScilabLogo.bmp
WizardSmallImageFile=tools\innosetup\ScilabLogoSmall.bmp
BackColor=clGray
BackColor2=clBlack
BackColorDirection=lefttoright
AppCopyright=Dassault Systèmes S.E. - Copyright (C) {#CurrentYear}
UninstallDisplayIcon={app}\bin\wscilex.exe
SolidCompression=true
VersionInfoVersion={#ScilabVersion}
VersionInfoCompany=Dassault Systèmes S.E.
; minimum right to install Scilab
PrivilegesRequired=none

#ifndef MR
#define MR="0"
#endif

#if MR == "0"
Compression=lzma2/ultra64
#else
Compression=lzma2/fast
#endif
;-------------------------------------------------------------------------------
;
