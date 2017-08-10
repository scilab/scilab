//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
// Copyright (C) 2009 - 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3934 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3439
//
// <-- Short Description -->
//   Online Help of ricc() function crashes Scilab

//Standard formulas to compute Riccati solutions
A=rand(3,3);
B=rand(3,2);
C=rand(3,3);
C=C*C';
R=rand(2,2);
R=R*R'+eye();
B=B*inv(R)*B';
X=ricc(A,B,C,'cont');
computed = A'*X+X*A-X*B*X+C;
expected = zeros(3,3);
assert_checkalmostequal ( computed, expected, [] , 1.e4 * %eps );

H=[A -B;-C -A'];
[T,d]=schur(eye(H),H,'cont');
T=T(:,1:d);
X1=T(4:6,:)/T(1:3,:);
assert_checkalmostequal ( X1, X, 1.e4 * %eps );

[T,d]=schur(H,'cont');
T=T(:,1:d);
X2=T(4:6,:)/T(1:3,:);
assert_checkalmostequal ( X2, X, 1.e4 * %eps );

// Discrete time case
F=A;
B=rand(3,2);
G1=B;
G2=R;
G=G1/G2*G1';
H=C;
X=ricc(F,G,H,'disc');
expected =  [
    2.0390067    1.3494625    1.3988904  
    1.3494625    1.4794503    1.6987296  
    1.3988904    1.6987296    2.3494632  
    ];
assert_checkalmostequal ( computed, expected, 1.e7 );

                    
