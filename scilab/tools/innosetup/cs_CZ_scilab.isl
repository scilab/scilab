;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2011 - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;
;-------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;-------------------------------------------------------------------------------

cs_CZ.LANGUAGE =cs_CZ

cs_CZ.MsgBoxJRENotFound =Java Runtime Environment not detected.
cs_CZ.MsgBoxJREURL =Please download & Install JRE at http://www.java.com/en/download/
cs_CZ.MsgBoxJREReinstall = Reinstall Scilab after your JRE update.
cs_CZ.MsgBoxSSERequired = CPU SSE2 instructions are required for Scilab to run correctly.
cs_CZ.MsgBoxX64Ready = For increase performance, it will be better to use Scilab x64 bits version on this Operating system.

cs_CZ.MsgBoxJavaDetection1 =Java is not installed on this computer and this program needs it.
cs_CZ.MsgBoxJavaDetection2 =Please choose to install Java Runtime:
cs_CZ.MsgBoxWebOpen =Can not open web page.
cs_CZ.MsgBoxDevToolsRequired1 =You choose to install toolbox_skeleton module but development_tools module is not selected.
cs_CZ.MsgBoxDevToolsRequired2 =Please install development_tools module.
cs_CZ.MsgBoxDevToolsRequired3 =You choose to install tests module but development_tools module is not selected.
cs_CZ.MsgBoxTclsciRequired2 =Please install tclsci module.
cs_CZ.MsgBoxFftw1 =You choose to install FFTW module for Scilab.
cs_CZ.MsgBoxFftw2 =This module requires to download FFTW library (GPL).
cs_CZ.MsgBoxFftw3 =Do you want to open a web page about this add-on ?
cs_CZ.MsgBoxFftw4 =Can not open web page.
cs_CZ.MsgBoxWinVer =Windows 2000 is required.
cs_CZ.MsgBoxScicosRequired1 =You choose to install Xcos module but Scicos module is not selected.
cs_CZ.MsgBoxScicosRequired2 =Please also install Scicos module.

;
cs_CZ.ButtonAboutModules =&About modules ...
;
cs_CZ.DescriptionEquDif =Differential Equations Module
cs_CZ.DescriptionInterpolation =Interpolation Module
cs_CZ.DescriptionPolynomials =Polynomials Module
cs_CZ.DescriptionSymbolic =Symbolic Module
cs_CZ.DescriptionStats =Statistics Module
cs_CZ.DescriptionSparse =Sparse Matrix Module
cs_CZ.DescriptionLinAlg =Linear Algebra Module
cs_CZ.DescriptionSigProc =Signal processing Module
cs_CZ.DescriptionCacsd =C.A.C.S.D Module
cs_CZ.DescriptionGraphics =Graphics Module
cs_CZ.DescriptionOptim =Optimization Module
cs_CZ.DescriptionMetanet =Metanet Toolbox
cs_CZ.DescriptionScicos =Scicos Toolbox
cs_CZ.DescriptionArnoldi =ARnoldi PACKage Toolbox
cs_CZ.DescriptionUmfpack =UMFPACK for Scilab
cs_CZ.DescriptionCompatFunctions =Compatibility functions
cs_CZ.DescriptionM2sci =M2SCI Toolbox
cs_CZ.DescriptionMatio =MAT File Input/Output Toolbox
cs_CZ.DescriptionExternalObjects =External objects
cs_CZ.DescriptionSound =Sound file handling Toolbox
cs_CZ.DescriptionRandlib =Randlib Toolbox
cs_CZ.DescriptionMexlib =Mexlib Tools
cs_CZ.DescriptionIntersci =Intersci Toolbox
cs_CZ.DescriptionTclTK =TCL/TK in Scilab
cs_CZ.DescriptionSpreadsheet =Spreadsheet Toolbox
cs_CZ.DescriptionJVM =Java Runtime
cs_CZ.DescriptionAtlas =Atlas library 3.8.0 CPU optimized
cs_CZ.DescriptionFftwModule = FFTW interface for Scilab
cs_CZ.DescriptionFftwMKLDownload = Download FFTW library optimized by Intel Math Kernel Library
cs_CZ.DescriptionRefFftwLibrary = Reference FFTW library
cs_CZ.DescriptionParametersTlbx =Parameters Toolbox
cs_CZ.DescriptionGeneticTlbx =Genetic Algorithms Toolbox
cs_CZ.DescriptionSimulAnnealingTlbx =Simulated Annealing Toolbox
cs_CZ.DescriptionDevTools =Development Tools
cs_CZ.DescriptionTests =Tests for Scilab
cs_CZ.DescriptionTlbxSkeleton =A toolbox skeleton (to extend scilab)
cs_CZ.DescriptionComConnector = COM connector
cs_CZ.DescriptionSciNotes = SciNotes: Editor for Scilab
cs_CZ.DescriptionXcos = XCos 1.1
cs_CZ.DescriptionParallel = Parallel Computing with Scilab
cs_CZ.DescriptionModulesManager = Tools for management of extern modules 
cs_CZ.DescriptionModuleJVM = JVM Module
;
cs_CZ.ConsoleIconName =Scilab Console
;
cs_CZ.WebIconScilab =Scilab on the Web
;
cs_CZ.ReadMeIcon =Read Me
;
cs_CZ.UnInstallIcon =Uninstall Scilab
;
cs_CZ.RegKeyRunWith =Run with
;
cs_CZ.DescriptionVC2008Redist =Install Microsoft Visual C++ 2008 Redistributable Package
;
cs_CZ.DescriptionConfigureAtlas =Configure Atlas
;
cs_CZ.DescriptionLaunchScilab =Launch Scilab
;
cs_CZ.DescriptionCreateDesktopIcon =Create a &desktop icon
;
cs_CZ.DescriptionQuickLaunch =Create a shortcut in Quick Launch Bar
;
cs_CZ.DescriptionAssociateSceSci =Associate *.sce,*.sci files with scilab
;
cs_CZ.DescriptionAssociateTstDem =Associate *.tst files with scilab
;
cs_CZ.DescriptionAssociateBinSav =Associate *.bin, *.sav files with scilab
;
cs_CZ.DescriptionAssociateGraph =Associate *.graph *.graphb files with scilab
;
cs_CZ.DescriptionAssociateScicos =Associate *.xcos *.cos *.cosf files with scilab
;
cs_CZ.DescriptionDefaultInstall =Installation (Default)
cs_CZ.DescriptionFullInstall =Full installation
cs_CZ.DescriptionCustomInstall =Custom installation
cs_CZ.DescriptionCLIInstall =Command Line Minimal Installation (no gui)
;
cs_CZ.DescriptionCPUOptimization =CPU Optimization for Scilab
cs_CZ.DescriptionATLASLibrary =Atlas Library for Scilab
cs_CZ.DescriptionRefBlasLibrary =Blas, Lapack Reference libraries for Scilab
cs_CZ.DescriptionMKLLibrary =Download Intel Math Kernel Library for Scilab
;
cs_CZ.DownloadMKLLabel =Downloading Intel Math Kernel Library files for Scilab
cs_CZ.DownloadMKLDescription =Please wait while Setup is downloading extra files to your computer.
;
cs_CZ.DownloadCHMLabel =Downloading CHM files for Scilab.
cs_CZ.DownloadCHMDescription =Please wait while Setup is downloading extra files to your computer.
cs_CZ.DoTasksJustAfterInstallMsg4 =CHM files not installed.
cs_CZ.DescriptionCHM=Download scilab help as CHM
;
cs_CZ.DownloadMKLFFTWLabel =Downloading FFTW3 library optimized by Intel Math Kernel.
;
cs_CZ.DoTasksJustAfterInstallMsg2 =BLAS, LAPACK reference libraries will be installed and used.
cs_CZ.DoTasksJustAfterInstallMsg3 =FFTW3 reference library will be installed and used.
;
cs_CZ.NextButtonClickwpReadyMsg1 =Impossible to download Intel Math Kernel Library files for Scilab (Default library will be used).
cs_CZ.NextButtonClickwpReadyMsg2 =Impossible to download FFTW3 library optimized by Intel Math Kernel Library (Default library will be used).
cs_CZ.NextButtonClickwpReadyMsg3 =Impossible to download CHM files(files will be not installed).
;
cs_CZ.DescriptionAtoms =ATOMS
;
cs_CZ.DescriptionHelpScilabCHM =Scilab Help
cs_CZ.ScilabCHM =modules\helptools\chm\scilab_en_US_help.chm
;
;-------------------------------------------------------------------------------
;