;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
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


#define COMPN_SCILAB 'scilab'
;
#define COMPN_GRAPHICS 'scilab\graphics'
#define COMPN_LINALG 'scilab\linear_algebra'
#define COMPN_CACSD 'scilab\cacsd'
#define COMPN_POLY 'scilab\polynomials'
#define COMPN_SYMB 'scilab\symbolic'
#define COMPN_SPARSE 'scilab\sparse'
#define COMPN_EQUDIF 'scilab\differential_equations'
#define COMPN_STATS 'scilab\statistics'
;
; by default
#define COMPN_JVM_MODULE 'scilab\JVM_MODULE'
#define COMPN_JRE 'JRE_1_6'

;
#define COMPN_CPU_OPTIMIZATION 'CPU_OPTIMIZATION'
#define COMPN_REF_CPU_LIBRARY 'CPU_OPTIMIZATION\CPU_REF'
#define COMPN_ATLAS_CPU_LIBRARY 'CPU_OPTIMIZATION\ATLAS'
#define COMPN_MKL_CPU_LIBRARY 'CPU_OPTIMIZATION\MKL'
;
#define COMPN_INTERP 'interpolation'
#define COMPN_SIGPROC 'signal_processing'

#define COMPN_OPTIM 'optimization'
#define COMPN_XCOS 'xcos'

#define COMPN_ARNOLDI 'arnoldi'
#define COMPN_TCLSCI 'tclsci'

#define COMPN_SCINOTES 'scinotes'

#define COMPN_EXTERNAL_OBJECTS 'external_objects'
#define COMPN_SOUND 'sound'
#define COMPN_RANDLIB 'randlib'
#define COMPN_MEXLIB 'mexlib'
#define COMPN_M2SCI 'm2sci'

#define COMPN_INTERSCI 'intersci'

#define COMPN_FFTW 'FFTW'
#define COMPN_FFTW_REF_LIBRARY 'FFTW\REF_LIBRARY'
#define COMPN_FFTW_MKL_LIBRARY 'FFTW\MKL_LIBRARY'

#define COMPN_UMFPACK 'UMFPACK'
#define COMPN_COMPATFUNCTIONS 'compatibility_functions'
#define COMPN_SPREADSHEET 'spreadsheet'
#define COMPN_DEVTOOLS 'development_tools'
#define COMPN_TESTS 'tests'
#define COMPN_TOOLBOX_SKELETON 'toolbox_skeleton'
#define COMPN_PARAMETERS 'parameters'
#define COMPN_GENETICALGORITHMS 'genetic_algorithms'
#define COMPN_SIMANNEALING 'simulated_annealing'
#define COMPN_MATIO 'matio'
#define COMPN_ATOMS 'ATOMS'
#define COMPN_MODULES_MANAGER 'modules_manager'
#define COMPN_PARALLEL 'parallel'
#define COMPN_CHM 'chm_scilab'

;
Name: {#COMPN_SCILAB}; Description: Scilab {#ScilabVersion}; Types: default full custom cli; Flags: fixed;
;
Name: {#COMPN_EQUDIF}; Description: {cm:DescriptionEquDif}; Types: default full custom cli; Flags: fixed;
Name: {#COMPN_POLY}; Description: {cm:DescriptionPolynomials}; Types: default full custom cli; Flags: fixed;
Name: {#COMPN_SYMB}; Description: {cm:DescriptionSymbolic}; Types: default full custom cli; Flags: fixed;
Name: {#COMPN_SPARSE}; Description: {cm:DescriptionSparse}; Types: default full custom cli; Flags: fixed;
Name: {#COMPN_LINALG}; Description: {cm:DescriptionLinAlg}; Types: default full custom cli; Flags: fixed;
Name: {#COMPN_CACSD}; Description: {cm:DescriptionCacsd}; Types: default full custom cli; Flags: fixed;
Name: {#COMPN_STATS}; Description: {cm:DescriptionStats}; Types: default full custom cli; Flags: fixed;
Name: {#COMPN_GRAPHICS}; Description: {cm:DescriptionGraphics}; Types: default full custom; Flags: fixed;
;
Name: {#COMPN_JVM_MODULE}; Description: {cm:DescriptionModuleJVM}; Types: default full custom; Flags: fixed;
;
Name: {#COMPN_SCINOTES}; Description:{cm:DescriptionSciNotes}; Types: default full custom; Flags: fixed;
;
Name: {#COMPN_DEVTOOLS}; Description: {cm:DescriptionDevTools}; Types: default full custom; Flags: fixed;
;
Name: {#COMPN_EXTERNAL_OBJECTS}; Description: {cm:DescriptionExternalObjects}; Types: default full custom; Flags: fixed;
;
Name: {#COMPN_ATOMS}; Description: {cm:DescriptionAtoms}; Types: default full custom; Flags: fixed;
;
Name: {#COMPN_CPU_OPTIMIZATION}; Description:{cm:DescriptionCPUOptimization}; Types: default full custom cli;
;
Name: {#COMPN_MKL_CPU_LIBRARY}; Description:{cm:DescriptionMKLLibrary}; Flags: exclusive
;
#ifndef SCILAB_X64
Name: {#COMPN_ATLAS_CPU_LIBRARY}; Description:{cm:DescriptionATLASLibrary}; Flags: exclusive
#endif
;
Name: {#COMPN_REF_CPU_LIBRARY}; Description:{cm:DescriptionRefBlasLibrary}; Flags: exclusive
;
Name: {#COMPN_FFTW}; Description: {cm:DescriptionFftwModule}; Types: default full custom cli;
Name: {#COMPN_FFTW_MKL_LIBRARY};Description: {cm:DescriptionFftwMKLDownload}; Flags: exclusive
Name: {#COMPN_FFTW_REF_LIBRARY};Description: {cm:DescriptionRefFftwLibrary}; Flags: exclusive
;
Name: {#COMPN_SIGPROC}; Description: {cm:DescriptionSigProc}; Types: default full custom;
Name: {#COMPN_INTERP}; Description: {cm:DescriptionInterpolation}; Types: default full custom;
Name: {#COMPN_OPTIM}; Description: {cm:DescriptionOptim}; Types: default full custom;
;
Name: {#COMPN_XCOS}; Description: {cm:DescriptionXcos}; Types: default full custom;
;
Name: {#COMPN_ARNOLDI}; Description: {cm:DescriptionArnoldi}; Types: default full custom;
Name: {#COMPN_UMFPACK}; Description: {cm:DescriptionUmfpack}; Types: default full custom;
;
Name: {#COMPN_COMPATFUNCTIONS}; Description: {cm:DescriptionCompatFunctions}; Types: default full custom;
Name: {#COMPN_M2SCI}; Description: {cm:DescriptionM2sci}; Types: default full custom;
Name: {#COMPN_MATIO}; Description: {cm:DescriptionMatio}; Types: default full custom;
;
Name: {#COMPN_SOUND}; Description: {cm:DescriptionSound}; Types: default full custom;
Name: {#COMPN_RANDLIB}; Description: {cm:DescriptionRandlib}; Types: default full custom;
;
Name: {#COMPN_MEXLIB}; Description: {cm:DescriptionMexlib}; Types: default full custom;
Name: {#COMPN_INTERSCI}; Description: {cm:DescriptionIntersci}; Types: default full custom;
;
Name: {#COMPN_TCLSCI}; Description: {cm:DescriptionTclTK}; Types: default full custom;
;
Name: {#COMPN_SPREADSHEET}; Description: {cm:DescriptionSpreadsheet}; Types: default full custom;
;
Name: {#COMPN_MODULES_MANAGER}; Description: {cm:DescriptionModulesManager}; Types: default full custom;
;
#ifndef SCILAB_F2C
Name: {#COMPN_PARALLEL}; Description: {cm:DescriptionParallel}; Types: default full custom;
#endif
;
Name: {#COMPN_PARAMETERS}; Description: {cm:DescriptionParametersTlbx}; Types: full custom;
Name: {#COMPN_GENETICALGORITHMS}; Description: {cm:DescriptionGeneticTlbx}; Types: full custom;
Name: {#COMPN_SIMANNEALING}; Description: {cm:DescriptionSimulAnnealingTlbx}; Types: full custom;
;
;
Name: {#COMPN_TESTS}; Description: {cm:DescriptionTests}; Types: full custom; ExtraDiskSpaceRequired: {#TESTS_SIZE}
Name: {#COMPN_TOOLBOX_SKELETON}; Description: {cm:DescriptionTlbxSkeleton}; Types: full custom;
;
#ifndef SCILAB_WITHOUT_JRE
Name: {#COMPN_JRE}; Description: {cm:DescriptionJVM} ({#javaUsed}); Types: default full custom; Flags: fixed;
#endif
;
Name: {#COMPN_CHM}; Description: {cm:DescriptionCHM}; Types: full; ExtraDiskSpaceRequired: {#CHM_SIZE}
;
;-------------------------------------------------------------------------------
;
