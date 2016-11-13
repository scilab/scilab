// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - 2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
// Copyright (C) 2012 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

function sortedRoots=sortRoots(rootsToSort)
    //Sort roots using rounded values to avoid rounding errors
    //Here 10000 is ok due to roots values
    [tmp,kRoots]=gsort(round(10000*[real(rootsToSort) imag(rootsToSort)]),"lr","i");
    sortedRoots = rootsToSort(kRoots);
endfunction

function checkroots(p,expectedroots,varargin)
    // Checks the roots function against given roots.
    //
    // 1. Check default algorithm
    myroots=roots(p);
    computedroots = sortRoots(myroots);
    expectedroots  = sortRoots(expectedroots);
    assert_checkalmostequal(computedroots,expectedroots,varargin(:));
    //
    // 2. Check "e" algorithm
    myroots=roots(p,"e");
    computedroots = sortRoots(myroots);
    expectedroots  = sortRoots(expectedroots);
    assert_checkalmostequal(computedroots,expectedroots,varargin(:));
    //
    // 3. Check "f" algorithm
    if ( isreal(p) ) then
        myroots=roots(p,"f");
        computedroots = sortRoots(myroots);
        expectedroots  = sortRoots(expectedroots);
        assert_checkalmostequal(computedroots,expectedroots,varargin(:));
    end
endfunction

//   Check the computation of the roots of a polynomial
//   with different kinds of polynomials and different
//   kinds of roots :
//   - real poly,
//   - complex poly,
//   - real roots,
//   - complex roots.

//roots : 3 real roots
p=-6+11*%s-6*%s^2+%s^3;
expectedroots  = [1; 2; 3];
checkroots(p,expectedroots,100*%eps);
//roots : 3 real roots + polynomials algebra
p=-6+11*%s-6*%s^2+%s^3;
q = p+0;
expectedroots  = [1; 2; 3];
checkroots(q,expectedroots,100*%eps);
//roots : 3 complex roots
p=-6-%i*6+(11+%i*5)*%s+(-6-%i)*%s^2+%s^3;
expectedroots  = [1+%i; 2 ; 3];
checkroots(p,expectedroots,1d-12,20*%eps);
//roots : 3 complex roots + polynomials algebra
p=-6-%i*6+(11+%i*5)*%s+(-6-%i)*%s^2+%s^3;
q = p+0;
expectedroots  = [1+%i; 2 ; 3];
checkroots(p,expectedroots,1d-12,20*%eps);
// roots : no root at all
p=1;
v=[];
checkroots(p,[]);
q = p+0;
checkroots(q,[]);
//roots : 2 complex roots
p=1+%s+%s^2;
expectedroots  = [-0.5 - sqrt(3.)/2.*%i; -0.5 + sqrt(3.)/2.*%i ];
checkroots(p,expectedroots,10*%eps);
//roots : 2 roots equals 0
p=%s^2;
expectedroots  = [0. ; 0. ];
checkroots(p,expectedroots,%eps);

// 2 real roots with a zero derivate at the root
p=(%s-%pi)^2;
expectedroots  = [%pi;%pi];
checkroots(p,expectedroots,10*%eps);

// Tests from CPOLY
// M. A. Jenkins and J. F. Traub. 1972.
// Algorithm 419: zeros of a complex polynomial.
// Commun. ACM 15, 2 (February 1972), 97-99.
//
// EXAMPLE 1.  POLYNOMIAL WITH ZEROS 1,2,...,10.
P=[];
PI=[];
P(1)=1;
P(2)=-55;
P(3)=1320;
P(4)=-18150;
P(5)=157773;
P(6)=-902055;
P(7) = 3416930;
P(8)=-8409500;
P(9)=12753576;
P(10)=-10628640;
P(11)=3628800;
PI(1:11) = 0;
P = complex(P,PI);
E = (1:10)';
R = roots(P);
E = sortRoots(E);
R = sortRoots(R);
assert_checkalmostequal(R, E, 1.e-10);
// EXAMPLE 2. ZEROS ON IMAGINARY AXIS DEGREE 3.
// x^3-10001.0001*i*x^2-10001.0001*x+i
P = [];
PI=[];
P(1)=1;
P(2)=0;
P(3)=-10001.0001;
P(4)=0;
PI(1)=0;
PI(2)=-10001.0001;
PI(3)=0;
PI(4)=1;
P = complex(P,PI);
E = [
0.0001*%i
%i
10000*%i
];
R = roots(P);
E = sortRoots(E);
R = sortRoots(R);
assert_checkalmostequal(R, E, 1.e-15, 1.e-10);

// plot(real(R),imag(R),"bo")
// xtitle("Roots","Real","Imaginary")

// EXAMPLE 3. ZEROS AT 1+I,1/2*(1+I)....1/(2**-9)*(1+I)
P = [];
PI=[];
P(1)=1.0;
P(2)=-1.998046875;
P(3)=0.0;
P(4)=.7567065954208374D0;
P(5)=-.2002119533717632D0;
P(6)=1.271507365163416D-2;
P(7)=0;
P(8)=-1.154642632172909D-5;
P(9)=1.584803612786345D-7;
P(10)=-4.652065399568528D-10;
P(11)=0;
PI(1)=0;
PI(2)=P(2);
PI(3)=2.658859252929688D0;
PI(4)=-7.567065954208374D-1;
PI(5)=0;
PI(6)=P(6);
PI(7)=-7.820779428584501D-4;
PI(8)=-P(8);
PI(9)=0;
PI(10)=P(10);
PI(11)=9.094947017729282D-13;
P = complex(P,PI);
R = roots(P);
E = (1+%i)*2.^((0:-1:-9)');
E = sortRoots(E);
R = sortRoots(R);
assert_checkalmostequal(R, E, 1.e-13, 1.e-14);

// EXAMPLE 4. MULTIPLE ZEROS
// Real part:
// 288 - 1344*x + 2204*x^2 - 920*x^3 - 1587*x^4 + 2374*x^5 - 1293*x^6 + 284*x^7 + 3*x^8 - 10*x^9 + x^10
// Imaginary part:
// 504*x - 2352*x^2 + 4334*x^3 - 3836*x^4 + 1394*x^5 + 200*x^6 - 334*x^7 + 100*x^8 - 10*x^9
P = [];
PI=[];
P(1)=1;
P(2)=-10;
P(3)=3;
P(4)=284;
P(5)=-1293;
P(6)=2374;
P(7)=-1587;
P(8)=-920;
P(9)=2204;
P(10)=-1344;
P(11)=288;
PI(1)=0;
PI(2)=-10;
PI(3)=100;
PI(4)=-334;
PI(5)=200;
PI(6)=1394;
PI(7) =-3836;
PI(8)=4334;
PI(9)=-2352;
PI(10)=504;
PI(11)=0;
P = complex(P,PI);
R = roots(P);
E = [
1
1
1
1
2*%i
2*%i
2*%i
3
3
4*%i
];
E = sortRoots(E);
R = sortRoots(R);
assert_checkalmostequal(R, E, 1.e-3, 1.e-3);

// EXAMPLE 5. 12 ZEROS EVENLY DISTRIBUTE ON A CIRCLE OF
// RADIUS 1. CENTERED AT 0+2I
// Real part:
// 4095 - 67584*x^2 + 126720*x^4 - 59136*x^6 + 7920*x^8 - 264*x^10 + x^12
// Imaginary part:
// 24576*x - 112640x^3 + 101376x^5 - 25344x^7 + 1760x^9 - 24x^11

P = [];
PI=[];
P(1)=1;
P(2)=0;
P(3)=-264;
P(4)=0;
P(5)=7920;
P(6)=0;
P(7)=-59136;
P(8)=0;
P(9)=126720;
P(10)=0;
P(11)=-67584;
P(12)=0;
P(13)=4095;
PI(1)=0;
PI(2)=-24;
PI(3)=0;
PI(4)=1760;
PI(5)=0;
PI(6)=-25344;
PI(7)=0;
PI(8)=101376;
PI(9)=0;
PI(10)=-112640;
PI(11)=0;
PI(12)=24576;
PI(13)=0;
P = complex(P,PI);
R = roots(P);
S3=sqrt(3);
E = [
-1 + 2*%i
%i
3*%i
1+2*%i
(1/2)*(-S3+3*%i)
(1/2)*(-S3+5*%i)
-(1/2)*%i*(S3+(-4-%i))
(1/2)*((1+4*%i)-%i*S3)
(1/2)*%i*(S3+(4+%i))
(1/2)*((1+4*%i)+%i*S3)
(1/2)*(S3+3*%i)
(1/2)*(S3+5*%i)
];
E = sortRoots(E);
R = sortRoots(R);
assert_checkalmostequal(R, E, 1.e-10, 1.e-8);

assert_checkequal(roots([4 3 2 1]), roots(poly([1 2 3 4], "x", "coeff")));
assert_checkequal(roots([4 3 2 1] + [1 2 3 4]*%i), roots(poly([1 2 3 4]+[4 3 2 1]*%i,"x","coeff")));

