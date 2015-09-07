//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - SE - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
A = [8,8,9;1,8,6;2,5,9];
B = [0,6;7,8;4,0];
C = [8,5,5;9,8,1;5,0,7];

C=C*C';
R=[2,9; 5,7];

R=R*R'+eye();
B=B*inv(R)*B';
X=ricc(A,B,C,'cont');
X_ref=[44.84688,2.304207,66.647924;2.304207,27.392655,10.625534;66.647924,10.625534,118.86653];
assert_checkalmostequal(X, X_ref, 1e-6);
