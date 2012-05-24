// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10282 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10282
//
// <-- Short Description -->
// repmat does not work for scalar input on dims greater than 2 neither for N-D arrays
//Scalar input
assert_checkequal(repmat(1,[2 1 2]),matrix(ones(4,1),[2 1 2]));

assert_checkequal(repmat("Scilab",[1 1 2]),matrix(["Scilab";"Scilab"],[1 1 2]));

assert_checkequal(repmat(%s,[1 2 2]),matrix(%s*ones(4,1),[1 2 2]));

h1=repmat(1/%s,[1 2 2]);
h2=matrix((1/%s)*ones(4,1),[1 2 2]);
assert_checkequal(h1.num,h2.num);
assert_checkequal(h1.den,h2.den);

// N-D arrays inputs
x=matrix(1:4,1,2,2);
assert_checkequal(repmat(x,[1 2]),[x x]);
assert_checkequal(repmat(x,[2 1]),[x;x]);
assert_checkequal(repmat(x,[2 1 2]),matrix([1;1;2;2;3;3;4;4;1;1;2;2;3;3;4;4],[2 2 4]));
