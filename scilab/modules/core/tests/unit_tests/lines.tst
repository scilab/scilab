//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//lines

ln  = lines();
lines(30);
ln1 = lines();

if ln1(2)<>30 then pause,end
lines(45,70);
ln1=lines();
if or(ln1<>[70,45]) then pause,end
