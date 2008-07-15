// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//who

a    = 33;
bbb  = -1;
vars = who("get");
if or(vars(1:2)<>["bbb";"a"]) then pause,end
