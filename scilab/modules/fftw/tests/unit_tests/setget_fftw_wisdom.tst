// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//=================================
//return fftw wisdom
txt=get_fftw_wisdom();
if size(txt)<> [3 1] then pause,end
//=================================
//set fftw wisdom
set_fftw_wisdom(txt);
//=================================
//reset fftw wisdom
fftw_forget_wisdom()
//=================================