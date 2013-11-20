// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// Set the seed to always get the same random numbers
rand("seed", 0);
grand("setsd", 0);

//
// 1. Permute some column vectors, and check that the output is basically correct.
//
// With Complexes.
//
X = (2:10)';
P = grand(5,"prm",X*%i);
assert_checkequal ( isreal(P) , %f );
assert_checkequal ( typeof(P) , "constant" );
assert_checkequal ( size(P) , [9 5] );
assert_checkequal ( gsort(P,"r","i") , X*%i*ones(1,5) );
//
// With Integers.
//
P = grand(5,"prm",int8(X));
assert_checkequal ( typeof(P) , "int8" );
assert_checkequal ( size(P) , [9 5] );
assert_checkequal ( gsort(P,"r","i") , int8(X)*ones(1,5) );
//
P = grand(5,"prm",int16(X));
assert_checkequal ( typeof(P) , "int16" );
assert_checkequal ( size(P) , [9 5] );
assert_checkequal ( gsort(P,"r","i") , int16(X)*ones(1,5) );
//
P = grand(5,"prm",int32(X));
assert_checkequal ( typeof(P) , "int32" );
assert_checkequal ( size(P) , [9 5] );
assert_checkequal ( gsort(P,"r","i") , int32(X)*ones(1,5) );
//
// With Strings.
//
X = string(X);
P = grand(1,"prm",X);
assert_checkequal ( typeof(P) , "string" );
assert_checkequal ( size(P) , size(X) );
assert_checkequal ( P , ["3" "9" "5" "7" "10" "6" "8" "2" "4"]');
//
// With Booleans.
//
X = [%f %t %t %t %t %f]';
P = grand(1,"prm",X);
assert_checkequal ( typeof(P) , "boolean" );
assert_checkequal ( size(P) , size(X) );
assert_checkequal ( P , [%t %f %t %t %f %t]' );
//
// With Polynomials.
//
s = poly(0,"s");
X = [0 s 1+s 1+s^2 s^4]';
P = grand(1,"prm",X);
assert_checkequal ( typeof(P) , "polynomial" );
assert_checkequal ( size(P) , size(X) );
assert_checkequal ( P , [1+s^2 1+s s s^4 0]' );
//
// With Sparses.
//
X = sparse([1 2 3 0 0 0 0 5 6])';
P = grand(1,"prm",X);
assert_checkequal ( typeof(P) , "sparse" );
assert_checkequal ( size(P) , size(X) );
assert_checkequal ( P , sparse([0 0 0 3 5 6 2 1 0])' );

//
// 2. With row vectors / matrices.
//
X = 2:10;
P = grand(1,"prm",X);
assert_checkequal ( size(P) , size(X) );
assert_checkequal ( gsort(P,"g","i") , X );
X(2, :) = 12:20;
P = grand(2,"prm",X*%i);
refP = [
13  9   20  3   10  2   4  5  15
19  14  6   18  12  16  7  8  17 ];
refP(:, :, 2) = [
14  7  8  9   13  10  15  19  4
12  5  2  17  6   16  18  3   20 ];
assert_checkequal ( size(P) , [size(X) 2] );
assert_checkequal ( P, refP*%i );
//
// With Integers.
//
X_int = int32(X);
P = grand(2,"prm",X_int);
refP = int32([
5  17  20  7   15  10  2   13  4
3  16  8   12  19  9   18  14  6 ]);
refP(:, :, 2) = int32([
16  13  6  2  20  19  18  14  7
9   12  3  8  15  17  10  4   5 ]);
assert_checkequal ( size(P) , [size(X_int) 2] );
assert_checkequal ( P, refP );
//
// With Strings.
//
X_str = string(X);
P = grand(2,"prm",X_str);
refP = string([
19  7   14  10  18  17  5  3  4
9   15  2   20  13  12  6  8  16 ]);
refP(:, :, 2) = string([
13  20  9   6  8   14  3  4   16
15  5   19  2  18  12  7  10  17 ]);
assert_checkequal ( size(P) , [size(X_str) 2] );
assert_checkequal ( P, refP );
//
// With Hypermatrices.
//
X(:, :, 2) = X+20;
P = grand(2,"prm",X);
assert_checkequal ( size(P) , [size(X) 2] );
assert_checkequal ( members(P, 2:40), ones(P) );
//
// Of Complexes.
//
P = grand(2,"prm",X*%i);
assert_checkequal ( size(P) , [size(X) 2] );
assert_checkequal ( members(P, (2:40)*%i), ones(P) );
//
// Of Integers.
//
P = grand(2,"prm",int16(X));
assert_checkequal ( size(P) , [size(X) 2] );
assert_checkequal ( members(P, int16(2:40)), ones(P) );
//
// Of Strings.
//
Xs = string(X);
Ps = grand(2,"prm",Xs);
assert_checkequal ( size(Ps) , [size(Xs) 2] );
assert_checkequal ( members(Ps, string(2:40)), ones(Ps) );
//
// Of Booleans.
//
Xb = floor(10*rand(2, 9, 2));
Xb = Xb<5;
Pb = grand(2,"prm",Xb);
refPb = gsort(Pb+0,"g","i");
assert_checkequal ( size(Pb) , [size(Xb) 2] );
assert_checkequal ( refPb(1:36), zeros(36, 1) );
assert_checkequal ( refPb(37:72), ones(36, 1) );
//
// Of Ploynomials.
//
Xp = matrix(X(:)+s, 2, 9, 2);
Pp = grand(2,"prm",Xp);
refPp = matrix(Pp(:)-s, 2, 9, 2, 2);
refPp = gsort(coeff(refPp(:)),"g","i");
assert_checkequal ( size(Pp) , [size(Xp) 2] );
assert_checkequal ( members(2:40, refPp), 2*([ones(1,9) 0 ones(1,9) 0 ones(1,9) 0 ones(1,9)]) );
//
// Sparse hypermatrices do not exist yet.
//

//
// Bug #6689
//
X = [%i 1-%i 2+3*%i].';
P = grand(4, "prm", X);
refP = [
2+3*%i  %i      2+3*%i  1-%i
%i      2+3*%i  %i      %i
1-%i    1-%i    1-%i    2+3*%i ];
assert_checkequal ( typeof(P) , "constant" );
assert_checkequal ( size(P) , [3 4] );
assert_checkequal ( P , refP );

refMsg = msprintf(_("%s: Wrong type for input argument: Matrix (full or sparse) or Hypermatrix of Reals, Complexes, Integers, Booleans, Strings or Polynomials expected.\n"), "grand");
assert_checkerror("grand(2, ""prm"", list())", refMsg);
