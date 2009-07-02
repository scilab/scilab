// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

iext = linspace(1,5,5);
ds = [ones(1:5),zeros(1:5)];
fg = linspace(0,0.5,10);
wt = ones(fg);
y = remez(iext,ds,fg,wt);

if max(abs(y - [0.500 0.852 0.000 0.000])) > 0.001 then pause, end
