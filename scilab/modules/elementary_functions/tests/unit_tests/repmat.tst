// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

assert_checkequal(repmat(1:3,2,2),[1,2,3,1,2,3;1,2,3,1,2,3]);
assert_checkequal(repmat(1+2*%i,2),[ 1+%i*2, 1+%i*2; 1+%i*2, 1+%i*2]);
a=int8([1 0 1;0 1 0]);
assert_checkequal(repmat(a,2,3,2),matrix(int8([1;0;1;0;0;1;0;1;1;0;1;0;1;0;1;0;0;1; ...
0;1;1;0;1;0;1;0;1;0;0;1;0;1;1;0;1;0;1;0;1;0;0;1;0;1; ...
1;0;1;0;1;0;1;0;0;1;0;1;1;0;1;0;1;0;1;0;0;1;0;1;1;0; ...
1;0]),4,9,2));
assert_checkequal(repmat("Scilab",3,2),["Scilab","Scilab";"Scilab","Scilab";"Scilab","Scilab"]);
H=repmat([1/%s;2/(%s+1)],1,3);
s=poly(0,"s");
H_ref=[1/s,1/s,1/s;2/(1+s),2/(1+s),2/(1+s)];
assert_checkequal(numer(H),numer(H_ref));
assert_checkequal(denom(H),denom(H_ref));
assert_checkequal(repmat([],2,2),[]);
B_ref=int8(matrix([1;3;7;15;31;63;127;55;1;3;7;15;31;63;127;55], [2 4 2]));
assert_checkequal(repmat(int8([1,7,31,127;3,15,63,55]),1,1,2),B_ref);
C_ref = matrix(1:24, [2 3 2 2]);
assert_checkequal(repmat(C_ref,1),C_ref);
assert_checkequal(repmat(1:2, 2),[1 2 1 2; 1 2 1 2]);
assert_checkequal(repmat(matrix(1:2, [1 2]),2),[1 2 1 2; 1 2 1 2]);
assert_checkequal(repmat([1,2;3,4],[2,3]),[1,2,1,2,1,2;3,4,3,4,3,4;1,2,1,2,1,2;3,4,3,4,3,4]);
assert_checkequal(repmat(int8([1,2;3,4]),[2,3]),int8([1,2,1,2,1,2;3,4,3,4,3,4;1,2,1,2,1,2;3,4,3,4,3,4]));
assert_checkequal(repmat(matrix(1:2, 1, 2),[2,3]),[1,2,1,2,1,2;1,2,1,2,1,2]);
