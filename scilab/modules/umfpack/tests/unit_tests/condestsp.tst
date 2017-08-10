// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - Bruno PINCON
// Copyright (C) 2008 - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

warning("off");

A = sparse( [ 2  3  0  0  0;
              3  0  4  0  6; 
              0 -1 -3  2  0; 
              0  0  1  0  0; 
              0  4  2  0  1] );
K1 = condestsp(A);
if norm(K1,39.385965) < 1E-6 then pause, end

// verif by direct computation
K1e = norm(A,1)*norm(inv(full(A)),1);
if norm(K1e,39.385965) < 1E-6 then pause, end
if K1e <> K1 then pause, end

// another example
[A] = ReadHBSparse(SCI+"/modules/umfpack/demos/arc130.rua");
K1 = condestsp(A);
if round(K1) <> 10798708075 then pause, end

// this example is not so big so that we can do the verif
K1e = norm(A,1)*norm(inv(full(A)),1);
if round(K1e) <> 10798708075 then pause, end
if round(K1) <> round(K1e) then pause, end

// if you have already the lu factors condestsp(A,Lup) is faster
// because lu factors are then not computed inside condestsp
Lup = umf_lufact(A);
K1 = condestsp(A,Lup);
if round(K1) <> 10798708075 then pause, end

umf_ludel(Lup)         // clear memory
