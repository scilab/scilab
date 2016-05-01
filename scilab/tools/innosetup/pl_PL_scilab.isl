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

pl_PL.LANGUAGE =pl_PL

pl_PL.MsgBoxJRENotFound =Java Runtime Environment not detected.
pl_PL.MsgBoxJREURL =Please download & Install JRE at http://www.java.com/en/download/
pl_PL.MsgBoxJREReinstall =Reinstall Scilab after your JRE update.
pl_PL.MsgBoxSSERequired =CPU SSE2 instructions are required for Scilab to run correctly.
pl_PL.MsgBoxX64Ready =For increase performance, it will be better to use Scilab x64 bits version on this Operating system.

pl_PL.MsgBoxJavaDetection1 =Java is not installed on this computer and this program needs it.
pl_PL.MsgBoxJavaDetection2 =Please choose to install Java Runtime:
pl_PL.MsgBoxWebOpen =Can not open web page.
pl_PL.MsgBoxDevToolsRequired1 =You choose to install toolbox_skeleton module but development_tools module is not selected.
pl_PL.MsgBoxDevToolsRequired2 =Please install development_tools module.
pl_PL.MsgBoxDevToolsRequired3 =You choose to install tests module but development_tools module is not selected.
pl_PL.MsgBoxTclsciRequired2 =Please install tclsci module.
pl_PL.MsgBoxWinVer =Windows 2000 is required.
pl_PL.MsgBoxScicosRequired1 =You choose to install Xcos module but Scicos module is not selected.
pl_PL.MsgBoxScicosRequired2 =Please also install Scicos module.

;
pl_PL.ButtonAboutModules =&About modules ...
;
pl_PL.DescriptionEquDif =Differential Equations Module
pl_PL.DescriptionInterpolation =Interpolation Module
pl_PL.DescriptionPolynomials =Polynomials Module
pl_PL.DescriptionSymbolic =Symbolic Module
pl_PL.DescriptionStats =Statistics Module
pl_PL.DescriptionSparse =Sparse Matrix Module
pl_PL.DescriptionLinAlg =Linear Algebra Module
pl_PL.DescriptionSigProc =Signal processing Module
pl_PL.DescriptionCacsd =C.A.C.S.D Module
pl_PL.DescriptionGraphics =Graphics Module
pl_PL.DescriptionOptim =Optimization Module
pl_PL.DescriptionMetanet =Metanet Toolbox
pl_PL.DescriptionScicos =Scicos Toolbox
pl_PL.DescriptionArnoldi =ARnoldi PACKage Toolbox
pl_PL.DescriptionUmfpack =UMFPACK for Scilab
pl_PL.DescriptionCompatFunctions =Compatibility functions
pl_PL.DescriptionM2sci =M2SCI Toolbox
pl_PL.DescriptionMatio =MAT File Input/Output Toolbox
pl_PL.DescriptionExternalObjects =External objects
pl_PL.DescriptionSound =Sound file handling Toolbox
pl_PL.DescriptionRandlib =Randlib Toolbox
pl_PL.DescriptionMexlib =Mexlib Tools
pl_PL.DescriptionIntersci =Intersci Toolbox
pl_PL.DescriptionTclTK =TCL/TK in Scilab
pl_PL.DescriptionSpreadsheet =Spreadsheet Toolbox
pl_PL.DescriptionJVM =Java Runtime
pl_PL.DescriptionAtlas =Atlas library 3.8.0 CPU optimized
pl_PL.DescriptionFftwModule =FFTW interface for Scilab
pl_PL.DescriptionMKLFftwLibrary =FFTW library optimized by Intel Math Kernel Library
pl_PL.DescriptionRefFftwLibrary =Reference FFTW library
pl_PL.DescriptionParametersTlbx =Parameters Toolbox
pl_PL.DescriptionGeneticTlbx =Genetic Algorithms Toolbox
pl_PL.DescriptionSimulAnnealingTlbx =Simulated Annealing Toolbox
pl_PL.DescriptionDevTools =Development Tools
pl_PL.DescriptionTests =Tests for Scilab
pl_PL.DescriptionTlbxSkeleton =A toolbox skeleton (to extend Scilab)
pl_PL.DescriptionComConnector =COM connector
pl_PL.DescriptionSciNotes =SciNotes: Editor for Scilab
pl_PL.DescriptionXcos =Xcos
pl_PL.DescriptionParallel =Parallel Computing with Scilab
pl_PL.DescriptionModulesManager =Tools for management of extern modules
pl_PL.DescriptionModuleJVM =JVM Module
;
pl_PL.ConsoleIconName =Scilab Console
;
pl_PL.WebIconScilab =Scilab on the Web
;
pl_PL.ReadMeIcon =Read Me
;
pl_PL.UnInstallIcon =Uninstall Scilab
;
pl_PL.RegKeyRunWith =Run with
;
pl_PL.DescriptionVC2008Redist =Install Microsoft Visual C++ 2008 Redistributable Package
;
pl_PL.DescriptionConfigureAtlas =Configure Atlas
;
pl_PL.DescriptionLaunchScilab =Launch Scilab
;
pl_PL.DescriptionCreateDesktopIcon =Create a &desktop icon
;
pl_PL.DescriptionQuickLaunch =Create a shortcut in Quick Launch Bar
;
pl_PL.DescriptionAssociateSceSci =Associate *.sce,*.sci files with Scilab
;
pl_PL.DescriptionAssociateTstDem =Associate *.tst files with Scilab
;
pl_PL.DescriptionAssociateBinSav =Associate *.bin, *.sav files with Scilab
;
pl_PL.DescriptionAssociateGraph =Associate *.graph *.graphb files with Scilab
;
pl_PL.DescriptionAssociateScicos =Associate *.xcos *.cos *.cosf files with Scilab
;
pl_PL.DescriptionDefaultInstall =Installation (Default)
pl_PL.DescriptionFullInstall =Full installation
pl_PL.DescriptionCustomInstall =Custom installation
pl_PL.DescriptionCLIInstall =Command Line Minimal Installation (no gui)
;
pl_PL.DescriptionCPUOptimization =CPU Optimization for Scilab
pl_PL.DescriptionATLASLibrary =Atlas Library for Scilab
pl_PL.DescriptionRefBlasLibrary =Blas, Lapack Reference libraries for Scilab
pl_PL.DescriptionMKLLibrary =Intel Math Kernel Library for Scilab
;
pl_PL.DoTasksJustAfterInstallMsg2 =BLAS, LAPACK reference libraries will be installed and used.
pl_PL.DoTasksJustAfterInstallMsg3 =FFTW3 reference library will be installed and used.
;
pl_PL.DescriptionAtoms =ATOMS
;-------------------------------------------------------------------------------
;
