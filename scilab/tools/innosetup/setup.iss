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

; data to modify with version
#ifdef SCILAB_X64
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
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
DefaultDirName={code:DefDirRoot}\{#ScilabBaseDirectory}
DefaultGroupName={#ScilabName}
SetupIconFile=tools\innosetup\scilab.ico
LicenseFile=COPYING
ChangesAssociations=yes
WindowVisible=false
AppPublisher=Scilab Enterprises
AppPublisherURL=http://www.scilab.org
AppSupportURL=http://bugzilla.scilab.org/
AppUpdatesURL=http://www.scilab.org/products/scilab/download
WizardImageStretch=no
WizardImageBackColor=clBlack
WizardImageFile=tools\innosetup\ScilabLogo.bmp
WizardSmallImageFile=tools\innosetup\ScilabLogoSmall.bmp
BackColor=clGray
BackColor2=clBlack
BackColorDirection=lefttoright
AppCopyright=Scilab Enterprises - Copyright © {#CurrentYear}
UninstallDisplayIcon={app}\bin\wscilex.exe
Compression=lzma/ultra64
InternalCompressLevel=ultra64
SolidCompression=true
VersionInfoVersion={#ScilabVersion}
VersionInfoCompany=Scilab Enterprises
; minimun right to install Scilab
PrivilegesRequired=none
;-------------------------------------------------------------------------------
;
