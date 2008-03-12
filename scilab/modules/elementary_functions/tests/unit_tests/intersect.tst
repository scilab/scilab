// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
A = [ 1 8 4 5 2 1];
B = [ 9 7 4 2 1 4];
[v,ka,kb] = intersect(A,B);
if or(A(ka) <> B(kb)) then pause,end

A = 'a'+string(A);
B = 'b'+string(B);
[v,ka,kb] = intersect(A,B);
if or(A(ka) <> B(kb)) then pause,end


A = [ "elt1" "elt3" "elt4"];
B = [ "elt5" "elt1" "elt3"];
[v, ka, kb] = intersect(A,B);
if or(A(ka) <> B(kb)) then pause,end


A = [ "elt1" "elt3" "elt4"];
B = [ "elt5" "elt6" "elt2" "elt1" "elt3"];

[v, ka, kb] = intersect(A,B);
if or(A(ka) <> B(kb)) then pause,end

// =============================================================================