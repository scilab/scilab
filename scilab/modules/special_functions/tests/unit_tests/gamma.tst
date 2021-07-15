// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- Short Description -->
// Unitary tests of the gamma() function
//

// Gamma complete
// --------------
assert_checkequal(gamma([]), []);

M = [0.5 1.5 ; 2.5 3.5];
H = matrix(M,2,1,2);        // 3D hypermatrix
Ref = [1.77245385090551610 0.88622692545275805      // R 4.0.3
       1.32934038817913702 3.32335097044784256 ];
assert_checkequal(gamma(M), Ref);
assert_checkequal(gamma(H), matrix(Ref,2,1,2));


// Gamma incomplete
// ----------------
assert_checkequal(%s_gamma(0.5,[]), []);
assert_checkequal(%s_gamma([],0.5), []);
// x scalar, a array
Ref = [0.68268949213708585 0.19874804309879918      // R 4.0.3
       0.03743422675270363 0.0051714634834845166 ];
assert_checkalmostequal(%s_gamma(0.5, M), Ref, 20*%eps);
assert_checkalmostequal(%s_gamma(0.5, H), matrix(Ref,2,1,2), 20*%eps);
assert_checkequal(%s_gamma(%inf, M), ones(2,2));
assert_checkequal(%s_gamma(%inf, H), ones(2,1,2));
// x array, a scalar
Ref = [0.68268949213708585 0.91673548333644994      // R 4.0.3
       0.97465268132253169 0.99184902840649725 ];
assert_checkalmostequal(%s_gamma(M, 0.5), Ref, 2*%eps);
assert_checkalmostequal(%s_gamma(H, 0.5), matrix(Ref,2,1,2), 2*%eps);
// x and a arrays
Ref = [0.68268949213708585 0.60837482372891105      // R 4.0.3
       0.58411981300449223 0.57112014244694542 ];
assert_checkalmostequal(%s_gamma(M, M), Ref, 10*%eps);
assert_checkalmostequal(%s_gamma(H, H), matrix(Ref,2,1,2), 10*%eps);


// Gamma incomplete COMPLEMENTARY
// ------------------------------
assert_checkequal(%s_gamma(0.5,[],"upper"), []);
assert_checkequal(%s_gamma([],0.5,"upper"), []);
// x scalar, a array
Ref = 1- [0.68268949213708585 0.19874804309879918      // R 4.0.3
          0.03743422675270363 0.0051714634834845166 ];
assert_checkalmostequal(%s_gamma(0.5, M, "upper"), Ref, 5*%eps);
assert_checkalmostequal(%s_gamma(0.5, H, "upper"), matrix(Ref,2,1,2), 5*%eps);
assert_checkequal(%s_gamma(%inf, M, "upper"), zeros(2,2));
assert_checkequal(%s_gamma(%inf, H, "upper"), zeros(2,1,2));
// x array, a scalar
Ref = 1 - [0.68268949213708585 0.91673548333644994      // R 4.0.3
           0.97465268132253169 0.99184902840649725 ];
assert_checkalmostequal(%s_gamma(M, 0.5, "upper"), Ref, 50*%eps);
assert_checkalmostequal(%s_gamma(H, 0.5, "upper"), matrix(Ref,2,1,2), 50*%eps);
// x and a arrays
Ref = 1 - [0.68268949213708585 0.60837482372891105      // R 4.0.3
           0.58411981300449223 0.57112014244694542 ];
assert_checkalmostequal(%s_gamma(M, M, "upper"), Ref, 10*%eps);
assert_checkalmostequal(%s_gamma(H, H, "upper"), matrix(Ref,2,1,2), 10*%eps);


// Gamma incomplete generalized
// ----------------------------
assert_checkequal(%s_gamma([],0.5,2), []);
assert_checkequal(%s_gamma(0.5,[],2), []);
assert_checkequal(%s_gamma(0.5,0.5,[]), []);
// x scalar, a and b arrays
Ref = [0.52049987781304685   0.31772966966378619
       0.22350492887667642   0.16477451738965726 ];
assert_checkalmostequal(%s_gamma(0.5, M, M), Ref, %eps);
assert_checkalmostequal(%s_gamma(0.5, H, H), matrix(Ref,2,1,2), %eps);
// x and a arrays, b scalar
assert_checkalmostequal(%s_gamma(M, M, 0.5), Ref, %eps);
assert_checkalmostequal(%s_gamma(H, H, 0.5), matrix(Ref,2,1,2), %eps);
// x array, a scalar, b array
Ref = [0.52049987781304685   0.96610514647531076
       0.99959304798255499   0.99999925690162761 ];
assert_checkalmostequal(%s_gamma(M, 0.5, M), Ref, %eps);
assert_checkalmostequal(%s_gamma(H, 0.5, H), matrix(Ref,2,1,2), %eps);
// x, a, and b arrays
Ref = [0.52049987781304685   0.78770971263986711
       0.9714568766738326    0.99906979057712397 ];
assert_checkalmostequal(%s_gamma(M, M, M), Ref, %eps);
assert_checkalmostequal(%s_gamma(H, H, H), matrix(Ref,2,1,2), %eps);


// Gamma incomplete generalized COMPLEMENTARY
// ------------------------------------------
assert_checkequal(%s_gamma([],0.5,2, "upper"), []);
assert_checkequal(%s_gamma(0.5,[],2, "upper"), []);
assert_checkequal(%s_gamma(0.5,0.5,[], "upper"), []);
// x scalar, a and b arrays
Ref = 1 - [0.52049987781304685   0.31772966966378619
           0.22350492887667642   0.16477451738965726 ];
assert_checkalmostequal(%s_gamma(0.5, M, M, "upper"), Ref, %eps);
assert_checkalmostequal(%s_gamma(0.5, H, H, "upper"), matrix(Ref,2,1,2), %eps);
// x and a arrays, b scalar
assert_checkalmostequal(%s_gamma(M, M, 0.5, "upper"), Ref, %eps);
assert_checkalmostequal(%s_gamma(H, H, 0.5, "upper"), matrix(Ref,2,1,2), %eps);
// x array, a scalar, b array
Ref = [0.47950012218695315   3.389485352468927D-02
       4.069520174449587D-04 7.430983723414120D-07 ];
assert_checkalmostequal(%s_gamma(M, 0.5, M, "upper"), Ref, %eps);
assert_checkalmostequal(%s_gamma(H, 0.5, H, "upper"), matrix(Ref,2,1,2), %eps);
// x, a, and b arrays
Ref = [0.47950012218695315    0.21229028736013292
       2.854312332616740D-02  9.302094228759846D-04 ];
assert_checkalmostequal(%s_gamma(M, M, M, "upper"), Ref, %eps);
assert_checkalmostequal(%s_gamma(H, H, H, "upper"), matrix(Ref,2,1,2), %eps);
