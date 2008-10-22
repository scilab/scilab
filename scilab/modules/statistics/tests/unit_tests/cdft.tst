// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// =============================================================================
// Tests for cdft() function
// using a table
// =============================================================================

prec    = 1.e-5;

Tab     = [0.9,0.5,0.3,0.20,0.10,0.05,0.02];
Df      = [1,2,3,4,5,6,7];
Th      = [0.158,0.816,1.250,1.533,2.015,2.447,2.998];
[P1,Q1] = cdft("PQ",Th,Df);
[P2,Q2] = cdft("PQ",-Th,Df);

if norm(Tab-(Q1+P2)) > 0.1 then pause, end

[P,Q] = cdft("PQ",Th,Df);
Th1   = cdft("T",Df,P,Q);
Df1   = cdft("Df",P,Q,Th);

if norm(Th1-Th) > prec then pause,end
if norm(Df1-Df) > prec then pause,end
