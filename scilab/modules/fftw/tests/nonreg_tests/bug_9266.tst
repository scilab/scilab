// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9266 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9266
//
// <-- Short Description -->
//
// fftw crashed on Windows with MKL wrapper
// (install scilab on windows with fftw support and MKL wrapper)

a = rand(3, 3);
r = fftw(a);

ref = [    5.0053826             ,   -1.0513065 + 0.6838074*%i,  -1.0513065 - 0.6838074*%i  ; ..
        -0.4155955 - 0.5198981*%i,  -0.2145198-0.3890532*%i,    0.1296924-1.0547338*%i  ; ..
       - 0.4155955 + 0.5198981*%i,    0.1296924 + 1.0547338*%i,  - 0.2145198 + 0.3890532*%i  ];

assert_checkalmostequal(r, ref, %eps * 10e8);
