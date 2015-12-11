// ============================================================================
// Scilab ( http://www.scilab.org\ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

r = 2;
R = [1,2;3,4];
T = 2 * R;
c = 1 + 2*%i;
C = [1+2*%i,2+4*%i;3+6*%i,4+8*%i];
RR = [1, 2, 3;4, 5, 6];
TT = 2 * RR;

// check that input are not modified.
R \ T;
assert_checkequal(R, [1,2;3,4]);
assert_checkequal(T, 2*[1,2;3,4]);

RR \ TT;
assert_checkequal(RR, [1, 2, 3;4, 5, 6]);
assert_checkequal(TT, 2*[1, 2, 3;4, 5, 6]);

//double \ double

//r \ r
assert_checkalmostequal(r \ r, 1);
//r \ c
assert_checkalmostequal(r \ c, c / r);
//c \ r
assert_checkalmostequal(c \ r, r / c);
//c \ c
assert_checkalmostequal(c \ c, 1);



//double \ DOUBLE

//r \ R
assert_checkalmostequal(r \ R, R / r);
//r \ C
assert_checkalmostequal(r \ C, C / r);
//c \ R
assert_checkalmostequal(c \ R, R / c);
//c \ C
assert_checkalmostequal(c \ C, C / c);


//DOUBLE \ double

//R \ r
assert_checkalmostequal(R \ r, [-4,2;3,-1]);
//R \ c
assert_checkalmostequal(R \ c, [-2-4*%i , 1+2*%i ; 1.5+3*%i, -0.5-%i]);
//C \ r
assert_checkalmostequal(C \ r, [-0.8+1.6*%i,0.4-0.8*%i;0.6-1.2*%i,-0.2+0.4*%i]);
//C \ c
assert_checkalmostequal(C \ c, [-2,1;1.5,-0.5]);


//DOUBLE \ DOUBLE

//R \ R
assert_checkalmostequal(R \ R, R / R);
//R \ C
assert_checkalmostequal(R \ C, C / R);
//C \ R
assert_checkalmostequal(C \ R, R / C, [], %eps);
//C \ C
assert_checkalmostequal(C \ C, C / C);

// dot left division
//double .\ double

//r .\ r
assert_checkalmostequal(r .\ r, 1);
//r .\ c
assert_checkalmostequal(r .\ c, c ./ r);
//c .\ r
assert_checkalmostequal(c .\ r, r ./ c);
//c .\ c
assert_checkalmostequal(c .\ c, 1);



//double .\ DOUBLE

//r .\ R
assert_checkalmostequal(r .\ R, R ./ r);
//r .\ C
assert_checkalmostequal(r .\ C, C ./ r);
//c .\ R
assert_checkalmostequal(c .\ R, R ./ c);
//c .\ C
assert_checkalmostequal(c .\ C, C ./ c);


//DOUBLE .\ double

//R .\ c
assert_checkalmostequal(R .\ c, c ./ R);
//C .\ r
assert_checkalmostequal(C .\ r, r ./ C);
//C .\ c
assert_checkalmostequal(C .\ c, c ./ C);


//DOUBLE .\ DOUBLE

//R .\ R
assert_checkalmostequal(R .\ R, R ./ R);
//R .\ C
assert_checkalmostequal(R .\ C, C ./ R);
//C .\ R
assert_checkalmostequal(C .\ R, R ./ C);
//C .\ C
assert_checkalmostequal(C .\ C, C ./ C);
