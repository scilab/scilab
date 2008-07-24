// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//
// companion.tst --
//   Test "companion" for real and complex polynomials
//   with linear, quadratic and cubic polynomials.
//

// Linear real polynomial
p=1+2*%s;
computed=companion(p);
expected=[-1/2];
if abs(expected-computed)>%eps then pause,end
// Quadratic real polynomial
p=1+2*%s+3*%s^2;
computed=companion(p);
expected=[-2/3 , -1/3;1 , 0];
if abs(expected-computed)>%eps then pause,end
// Cubic real polynomial
p=1+2*%s+3*%s^2+4*%s^3;
computed=companion(p);
expected=[-3/4 , -2/4 , -1/4; 1 , 0 , 0 ; 0 , 1, 0];
if abs(expected-computed)>%eps then pause,end
// Linear imaginary polynomial
p=1+%i+2*%s;
computed=companion(p);
expected=[-(1+%i)/2];
if abs(expected-computed)>%eps then pause,end
// Quadratic real polynomial
p=1+%i+2*%s+3*%s^2;
computed=companion(p);
expected=[-2/3 , -(1+%i)/3;1 , 0];
if abs(expected-computed)>%eps then pause,end
// Cubic real polynomial
p=1+%i+2*%s+3*%s^2+4*%s^3;
computed=companion(p);
expected=[-3/4 , -2/4 , -(1+%i)/4; 1 , 0 , 0 ; 0 , 1, 0];
if abs(expected-computed)>%eps then pause,end

