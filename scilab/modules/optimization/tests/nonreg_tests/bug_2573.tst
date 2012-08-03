// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2573 1949 and 2101 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2573
//
// <-- Short Description -->
//qld gives erroneous answer

Q=[52.260163 -92.994537; -92.994537 166.55854];
p=[1.8641286;-8.1261342];
C=zeros(1,2);b=0;me=1;
ci=-%inf*ones(2,1);cs=%inf*ones(2,1);

s1=qld(Q,p,C,b,ci,cs,me);
if norm(s1-[7.89465359259093979;4.45660600662314366])>1d-10 then pause,end


//
// Min   1/2 x^T [0.5 -1] x 
//               [-1   3]
//
// with [1 1] x = 1
//      0 <= x1 <= +8
//      0 <= x2 <= +8
//
Q = [0.5 -1; -1 3]; //positive-defined
p = [0; 0];
C = [1 1];
b = 1;
ci = [0; 0];
cs = [];
me = 1;
x0 = [0.5; 0.5];
tol = 1e-8;

[xcomputed,lagr_qld,info] = qld(Q,p,C,b,ci,cs,me,tol);
xexpected = [8;3]/11;
if norm(xcomputed-xexpected)>1d-10 then pause,end
