// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//=============================================
ff = fftw_flags();
if ff <> 64 then pause,end;
//=============================================
//change flags
r = fftw_flags(["FFTW_MEASURE";"FFTW_CONSERVE_MEMORY"]);
if r <> 4 then pause,end;
//=============================================
//change flags and display current value of fftw flags (both integer and strings)
[a,S]=fftw_flags("FFTW_PATIENT");
if a <> 32 then pause,end;
if S <>'FFTW_PATIENT' then pause,end;
//=============================================
