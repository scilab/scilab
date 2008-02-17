// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

y=impl([1;0;0],[-0.04;0.04;0],0,0.4,'resid','aplusp');
if size(y) <> [3 1] then pause,end
if norm( y - [0.9851721;0.0000339;0.0147941]) < %eps then pause,end