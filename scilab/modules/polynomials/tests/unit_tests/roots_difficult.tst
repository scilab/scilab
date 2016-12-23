// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - 2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
// Copyright (C) 2012 - INRIA - Serge Steer
// Copyright (C) 2016 - Scilab Enterprises - Clement David
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NOT FIXED YET -->

//
// Caution !
// The following are difficult root-finding problems
// with expected precision problems.
// See "Principles for testing polynomial
// zerofinding programs"
// Jenkins, Traub
// 1975
// p.28
// "The accuracy which one may expect to achieve in calculating
// zeros is limited by the condition of these zeros. In particular,
// for multiple zeros perturbations of size epsilon in the
// coefficients cause perturbations of size epsilon^(1/m)
// in the zeros."
//
//
// 3 real roots with a zero derivate at the root
// Really difficult problem : only simple precision computed, instead of double precision ***
p=(%s-%pi)^3;
expectedroots  = [%pi;%pi;%pi];
checkroots(p,expectedroots,%eps^(1/3),5*%eps^(1/3));
// 4 real roots with a zero derivate at the root
// Really difficult problem : only simple precision
p=(%s-%pi)^4;
expectedroots  = [%pi;%pi;%pi;%pi];
checkroots(p,expectedroots,%eps^(1/4),5*%eps^(1/4))
// 10 real roots with a zero derivate at the root
// Really difficult problem : only one correct digit
p=(%s-%pi)^10;
expectedroots  = [%pi;%pi;%pi;%pi;%pi;%pi;%pi;%pi;%pi;%pi];
checkroots(p,expectedroots,%eps^(1/10),8*%eps^(1/10))

// "Numerical computing with Matlab", Cleve Moler.
A = diag(1:20);
p = poly(A,"x");
e = [1:20]';
checkroots(p,e,%eps,0.2);
