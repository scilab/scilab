// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//=============================================
ff = fftw_flags();
assert_checkequal(double(ff),64);
//=============================================
//change flags
r = fftw_flags(["FFTW_MEASURE";"FFTW_CONSERVE_MEMORY"]);
assert_checkequal(double(r),4);
//=============================================
//change flags and display current value of fftw flags (both integer and strings)
[a,S]=fftw_flags("FFTW_PATIENT");
assert_checkequal(double(a),32);
assert_checkequal(S,'FFTW_PATIENT');
//=============================================
