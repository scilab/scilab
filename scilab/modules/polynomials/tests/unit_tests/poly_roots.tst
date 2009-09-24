// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// poly_roots.tst --
//   Check the computation of the roots of a polynomial
//   with different kinds of polynomials and different 
//   kinds of roots :
//   - real poly,
//   - complex poly,
//   - real roots,
//   - complex roots.


//roots : 3 real roots -> RPOLY
p=-6+11*%s-6*%s^2+%s^3;
myroots=roots(p);
computedroots = gsort(myroots);
expectedroots  = [3. ;2. ; 1.];
if (abs(computedroots-expectedroots)>400*%eps) then pause,end
//roots : 3 real roots + polynomials algebra -> RPOLY
p=-6+11*%s-6*%s^2+%s^3;
myroots=roots(p+0);
computedroots = gsort(myroots);
expectedroots  = [3. ;2. ; 1.];
if (abs(computedroots-expectedroots)>400*%eps) then pause,end
//roots : 3 complex roots -> Companion matrix used
p=-6-%i*6+(11+%i*5)*%s+(-6-%i)*%s^2+%s^3;
myroots=roots(p);
computedroots = gsort(myroots);
expectedroots  = [3. ;2. ; 1.+%i];
if (abs(computedroots-expectedroots)>400*%eps) then pause,end
//roots : 3 complex roots + polynomials algebra -> Companion matrix used
p=-6-%i*6+(11+%i*5)*%s+(-6-%i)*%s^2+%s^3;
myroots=roots(p+0);
computedroots = gsort(myroots);
expectedroots  = [3. ;2. ; 1.+%i];
if (abs(computedroots-expectedroots)>400*%eps) then pause,end
// roots : no root at all -> RPOLY
p=1;v=[];
if (roots(p)<>v) then pause,end
if (roots(p+0)<>v) then pause,end
//roots : 2 complex roots -> RPOLY
p=1+%s+%s^2;
myroots=roots(p);
computedroots = gsort(myroots);
expectedroots  = [-0.5 + sqrt(3.)/2.*%i ; -0.5 - sqrt(3.)/2.*%i];
if (abs(computedroots-expectedroots)>400*%eps) then pause,end
//roots : 2 roots equals 0 -> RPOLY
p=%s^2;
myroots=roots(p);
computedroots = gsort(myroots);
expectedroots  = [0. ; 0. ];
if (abs(computedroots-expectedroots)>400*%eps) then pause,end
//roots : 3 real roots -> forced to companion matrix with "e" option
p=-6+11*%s-6*%s^2+%s^3;
myroots=roots(p,"e");
computedroots = gsort(myroots);
expectedroots  = [3. ;2. ; 1.];
if (abs(computedroots-expectedroots)>400*%eps) then pause,end
//roots : 2 complex roots -> forced to companion matrix with "e" option
p=1+%s+%s^2;
myroots=roots(p,"e");
computedroots = gsort(myroots);
expectedroots  = [-0.5 + sqrt(3.)/2.*%i ; -0.5 - sqrt(3.)/2.*%i];
if (abs(computedroots-expectedroots)>400*%eps) then pause,end
// 2 real roots with a zero derivate at the root -> RPOLY
p=(%s-%pi)^2
myroots=roots(p);
computedroots = gsort(myroots);
expectedroots  = [%pi;%pi];
if (abs(computedroots-expectedroots)>%eps) then pause,end
// 2 real roots with a zero derivate at the root -> forced to companion matrix with "e" option
p=(%s-%pi)^2
myroots=roots(p,"e");
computedroots = gsort(myroots);
expectedroots  = [%pi;%pi];
if (abs(computedroots-expectedroots)>10*%eps) then pause,end
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
// 3 real roots with a zero derivate at the root -> RPOLY
// *** PRECISION PROBLEM : only simple precision computed, instead of double precision ***
p=(%s-%pi)^3
myroots=roots(p);
computedroots = gsort(myroots);
expectedroots  = [%pi;%pi;%pi];
if (abs(computedroots-expectedroots)>10^9*%eps) then pause,end
// 3 real roots with a zero derivate at the root -> forced to companion matrix with "e" option
// *** PRECISION PROBLEM : not even simple precision computed, instead of double precision expected ***
p=(%s-%pi)^3
myroots=roots(p,"e");
computedroots = gsort(myroots);
expectedroots  = [%pi;%pi;%pi];
if (abs(computedroots-expectedroots)>10^11*%eps) then pause,end
// 4 real roots with a zero derivate at the root -> RPOLY
// *** PRECISION PROBLEM : only simple precision computed, instead of double precision expected ***
p=(%s-%pi)^4
myroots=roots(p);
computedroots = gsort(myroots);
expectedroots  = [%pi;%pi;%pi;%pi];
if (abs(computedroots-expectedroots)>10^9*%eps) then pause,end
// 4 real roots with a zero derivate at the root -> forced to companion matrix with "e" option
// *** PRECISION PROBLEM : not even simple precision computed, instead of double precision expected ***
p=(%s-%pi)^4
myroots=roots(p,"e");
computedroots = gsort(myroots);
expectedroots  = [%pi;%pi;%pi;%pi];
if (abs(computedroots-expectedroots)>10^13*%eps) then pause,end
// 10 real roots with a zero derivate at the root -> forced to companion matrix with "e" option
// *** PRECISION PROBLEM : only one one true digit ***
p=(%s-%pi)^10
myroots=roots(p);
computedroots = gsort(myroots);
expectedroots  = [%pi;%pi;%pi;%pi;%pi;%pi;%pi;%pi;%pi;%pi];
if (abs(computedroots-expectedroots)>10^10*%eps) then pause,end
// 10 real roots with a zero derivate at the root -> forced to companion matrix with "e" option
// *** PRECISION PROBLEM : only one one true digit ***
p=(%s-%pi)^10
myroots=roots(p,"e");
computedroots = gsort(myroots);
expectedroots  = [%pi;%pi;%pi;%pi;%pi;%pi;%pi;%pi;%pi;%pi];
if (abs(computedroots-expectedroots)>10^15*%eps) then pause,end




