// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

m = mode();
mode(7);

// This is a small introduction to the polynomials objects in
// Scilab.
// 1. Define the polynomial
// There are several ways to define a polynomial in Scilab.
// The first way is to define a variable and then define the
// "natural" mathematical definition.
// Define x as a variable, that is, a polynomial of degree 0
x=poly(0,"x")
// Define p as a polynomial of variable x
p1=-6+11*x-6*x^2+x^3
// The second way is to define the coefficients of the polynomial
// as a vector.
mycoeff=[-6 11 -6 1]
p2=poly(mycoeff,"x","coeff")
// The third way is to define the polynomials by its roots.
myroots=[1 2 3]
p3=poly(myroots,"x","roots")
// The 4th way is to define the polynomial as the characteristic
// polynomial of a given matrix.
// In the following case, we are giving the companion matrix of our
// favorite polynomial.
mymatrix=[6 -11 6;1 0 0;0 1 0]
p4=poly(mymatrix,"x")
// 2. Algebraic manipulations
// We can can process simple algebraic computations
// with the polynomials, considered as basic objects.
p5 = p1+p2
p6 = p1-p2
p7 = p1*p2
p8 = p3-p1*p4
// We can test the equality of two polynomials
are12equal = (p1==p2)
are12diff = (p1<>p2)

// 3. Polynomial analysis
// Scilab provides several functions to inquire a specific
// polynomial.
// The "coeff" function returns a vector containing the
// coefficients of a given polynomial.
coeff1=coeff(p1)
// One can compute the derivative of the polynomial with
// the "derivat" function, which returns the derivative
// of the given polynomial or rational polynomial.
p10=derivat(p1)
// One can compute numerically the roots of one given polynomial with the
// "roots" function, which returns the vectors of roots.
theroots=roots(p1)
// The companion matrix is so that its characteristic polynomial
// is p.
comp1=companion(p1)
// One can check that the eigenvalues of the companion matrix of p1
// are the roots of p1.
roots1=spec(comp1)
// One can factor a given polynomial with the "factor" function.
// Here the p1 polynomial can be written p1=g*(x-1)*(x-2)*(x-3)
// with "g" the gain.
[lnum,g]=factors(p1)

// 4. Rational polynomial
// Rational polynomials can be defined as well.
p9=(1+2*x+3*x^2)/(4+5*x+6*x^2)
// The derivative can be computed with "derivat", as
// as regular polynomial.
p11=derivat(p9)
// Notice that polynomials and rational polynomials do
// not have the same type.
typeof(p1)
typeof(p9)
// The numerator and denominator polynomials of the rational polynomial
// can be extracted with the "numer" and "denom" functions.
numer(p9)
denom(p9)
// Rational polynomials can be simplified with the "simp" function,
// which returns the numerator and denominator of the given couple
// of polynomials.
[n,d]=simp((x+1)*(x+2),(x+1)*(x-2))


mode(m);

// clear variables

clear       n; clear         d; clear     denom; clear numer;
clear  typeof; clear       p11; clear        p9; clear %p_r_p;
clear    lnum; clear         g; clear   factors; clear roots1;
clear   comp1; clear companion; clear  theroots; clear p10;
clear derivat; clear    coeff1; clear are12diff; clear are12equal;
clear      p8; clear        p7; clear        p6; clear p5;
clear      p4; clear  mymatrix; clear        p3; clear myroots;
clear      p2; clear   mycoeff; clear        p1; clear x;
clear       m;
