// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 7101 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7101
//
// <-- Short Description -->
// The roots / Jenkins-Traub algorithm does not produce the roots, sometimes.
// The solution is to use the eigenvalues of the companion matrix.

//
// sort_merge_comparison --
//   Returns -1 if x < y,
//   returns 0 if x==y,
//   returns +1 if x > y
//
function order = sort_merge_comparison ( x , y )
    if x < y then
        order = -1
    elseif x==y then
        order = 0
    else
        order = 1
    end
endfunction

//
// sort_merge --
//   Returns the sorted array x.
// Arguments
//   x : the array to sort
//   compfun : the comparison function
// Bruno Pincon
// "quelques tests de rapidité entre différents logiciels matriciels"
// Modified by Michael Baudin to manage a comparison function
//
function [x] = sort_merge ( varargin )
    [lhs,rhs]=argn();
    if rhs<>1 & rhs<>2 then
        errmsg = sprintf("Unexpected number of arguments : %d provided while 1 or 2 are expected.",rhs);
        error(errmsg)
    end
    // Get the array x
    x = varargin(1);
    // Get the comparison function compfun
    if rhs==1 then
        compfun = sort_merge_comparison;
    else
        compfun = varargin(2);
    end
    // Proceed...
    n = length(x)
    if n > 1 then
        m = floor(n/2);
        p = n-m
        x1 = sort_merge ( x(1:m) , compfun )
        x2 = sort_merge ( x(m+1:n) , compfun )
        i = 1;
        i1 = 1;
        i2 = 1;
        for i = 1:n
            order = compfun ( x1(i1) , x2(i2) );
            if order<=0 then
                x(i) = x1(i1)
                i1 = i1+1
                if (i1 > m) then
                    x(i+1:n) = x2(i2:p)
                    break
                end
            else
                x(i) = x2(i2)
                i2 = i2+1
                if (i2 > p) then
                    x(i+1:n) = x1(i1:m)
                    break
                end
            end
        end
    end
endfunction

//
// compare_complexrealimag --
//   Returns -1 if a < b,
//   returns 0 if a==b,
//   returns +1 if a > b
// Compare first by real parts, then by imaginary parts.
//
function order = compare_complexrealimag ( a , b )
    ar = real(a)
    br = real(b)
    if ar < br then
        order = -1
    elseif ar > br then
        order = 1
    else
        ai = imag(a)
        bi = imag(b)
        if ai < bi then
            order = -1
        elseif ai == bi then
            order = 0
        else
            order = 1
        end
    end
endfunction

//
// assert_close --
//   Returns 1 if the two real matrices computed and expected are close,
//   i.e. if the relative distance between computed and expected is lesser than epsilon.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_close ( computed, expected, epsilon )
    if expected==0.0 then
        shift = norm(computed-expected);
    else
        shift = norm(computed-expected)/norm(expected);
    end
    if shift < epsilon then
        flag = 1;
    else
        flag = 0;
    end
    if flag <> 1 then pause,end
endfunction

function y = sortmyroots(x)
    // Sort the roots of a polynomial with a customized
    // complex-aware sorting algorithm.
    y = sort_merge ( x , compare_complexrealimag );
endfunction

// Failed on Windows 32 bits
p = [1,1,-7,-15,1,-4,4,7,4,-53,1,-53,-8,3,3,0,9,-15];
r = roots(p);
e = [
2.9977242
- 2.0998215 + 1.0381514 * %i
- 2.0998215 - 1.0381514 * %i
- 1.1261224 + 0.7687233 * %i
- 1.1261224 - 0.7687233 * %i
1.1176579 + 0.5115332 * %i
1.1176579 - 0.5115332 * %i
- 0.7359417 + 0.3731641 * %i
- 0.7359417 - 0.3731641 * %i
0.2849638 + 0.9531919 * %i
0.2849638 - 0.9531919 * %i
0.0897371 + 1.0370037 * %i
0.0897371 - 1.0370037 * %i
- 0.1740455 + 0.9263179 * %i
- 0.1740455 - 0.9263179 * %i
0.6447102 + 0.2914081 * %i
0.6447102 - 0.2914081 * %i
];
e = sortmyroots(e);
r = sortmyroots(r);
assert_close ( r, e, 1.e-6 );

// Failed on Mac, on Windows
p=[1,1,-7,-35,1,-4,4,7,4,-88,1,-88,-8,3,3,0,9,-35];
r = roots(p);
e = [
3.6133489
- 2.3323533 + 2.0888127 * %i
- 2.3323533 - 2.0888127 * %i
1.0856792 + 0.5138318 * %i
1.0856792 - 0.5138318 * %i
- 1.1030013 + 0.6108696 * %i
- 1.1030013 - 0.6108696 * %i
0.3226838 + 0.9451270 * %i
0.3226838 - 0.9451270 * %i
0.0250044 + 1.0210451 * %i
0.0250044 - 1.0210451 * %i
- 0.2556563 + 0.9467085 * %i
- 0.2556563 - 0.9467085 * %i
- 0.7512303 + 0.3765797 * %i
- 0.7512303 - 0.3765797 * %i
0.7021994 + 0.3415821 * %i
0.7021994 - 0.3415821 * %i
];
e = sortmyroots(e);
r = sortmyroots(r);
assert_close ( r, e, 1.e-6 );

// Failed on Linux
p=[1,1,-7,-80,1,-4,4,7,4,-27,1,-27,-8,3,3,0,9,-80];
r = roots(p);
e = [
- 2.7595524 + 3.1924496 * %i
- 2.7595524 - 3.1924496 * %i
4.5006465
- 0.9689444 + 0.2683252 * %i
- 0.9689444 - 0.2683252 * %i
- 0.8111357 + 0.6166997 * %i
- 0.8111357 - 0.6166997 * %i
- 0.3893539 + 0.9194344 * %i
- 0.3893539 - 0.9194344 * %i
0.0061369 + 1.0065796 * %i
0.0061369 - 1.0065796 * %i
0.4195701 + 0.9089127 * %i
0.4195701 - 0.9089127 * %i
0.9590394 + 0.2589039 * %i
0.9590394 - 0.2589039 * %i
0.7939168 + 0.5672744 * %i
0.7939168 - 0.5672744 * %i
];
e = sortmyroots(e);
r = sortmyroots(r);
assert_close ( r, e, 1.e-6 );

// Failed on Windows 32 bits
p=[1,0,1,1,1,-1,-1,1,1,0,1,0,-1,-1,1,-2,0,0,1,-1,1];
r = roots(p);
e = [
0.5444059 + 1.3082079 * %i
0.5444059 - 1.3082079 * %i
- 1.0517348 + 0.2347104 * %i
- 1.0517348 - 0.2347104 * %i
- 0.5893898 + 0.9840032 * %i
- 0.5893898 - 0.9840032 * %i
- 0.8170407 + 0.5459189 * %i
- 0.8170407 - 0.5459189 * %i
- 0.6570402 + 0.7150468 * %i
- 0.6570402 - 0.7150468 * %i
0.0129780 + 0.9748750 * %i
0.0129780 - 0.9748750 * %i
0.9192290 + 0.4894403 * %i
0.9192290 - 0.4894403 * %i
0.8691302 + 0.0832523 * %i
0.8691302 - 0.0832523 * %i
0.4975871 + 0.6807740 * %i
0.4975871 - 0.6807740 * %i
0.2718754 + 0.7528695 * %i
0.2718754 - 0.7528695 * %i
];
e = sortmyroots(e);
r = sortmyroots(r);
assert_close ( r, e, 1.e-6 );

// A loop on several polynomials
for i=-100:100
    if ( modulo(i,20)==0 ) then
        mprintf("i=%d\n",i);
    end
    for j=-100:100
        p=[1 1 -7 j 1 -4 4 7 4 i 1 i -8 3 3 0 9  j];
        roots(p);
    end;
end;

// A loop on random polynomials.
// The coefficients are integers
for i = 1:3000
    if ( modulo(i,1000)==0 ) then
        mprintf("i=%d\n",i);
    end
    p = [1 round(4*rand(1,20)-2)];
    roots(p);
end
