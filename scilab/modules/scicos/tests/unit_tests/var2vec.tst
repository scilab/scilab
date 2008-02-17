// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

a=list("cos",[1.1,2]);
b=var2vec(a);
if type(b) <> 1 then pause,end
if size(b) <> [12 1] then pause,end