// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

deff('[x] = myplus(y,z)','x=y+z');
if isdef('myplus') <> %t then pause,end
if myplus(3,2) <> 5 then pause,end

deff('[x] = mymacro(y,z)',['a=3*y+1'; 'x=a*z+y']);
if isdef('mymacro') <> %t then pause,end
if mymacro(5,2) <> 37 then pause,end
