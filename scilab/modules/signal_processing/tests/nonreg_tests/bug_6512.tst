// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 6512 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6512
//
// <-- Short Description -->
// Check that findm is still ok

res = findm(1);
assert_checktrue(%k(1-res)/%k(res)-1<1e-8);
res = findm(0.1);
assert_checktrue(%k(1-res)/%k(res)-0.1<1e-8);
res = findm(0.5);
assert_checktrue(%k(1-res)/%k(res)-0.5<1e-8);

// Check that zpell is still ok
gain_ref=0.4818507;
po_ref=[-68.292059+161.54792*%i -68.292059-161.54792*%i];
ze_ref=[250.20121*%i, -250.20121*%i];
[ze,po,gain]=zpell(0.2,2,100,200);
assert_checktrue(abs(ze-ze_ref)<1e-5);
assert_checktrue(abs(po-po_ref)<1e-5);
assert_checktrue(abs(gain-gain_ref)<1e-5);
gain_ref=0.3553711;
po_ref=[-157.37848+228.46667*%i,-157.37848-228.46667*%i];
ze_ref=[464.22153*%i -464.22153*%i];
[ze,po,gain]=zpell(0.1,3,150,300);
assert_checktrue(abs(ze-ze_ref)<1e-5);
assert_checktrue(abs(po-po_ref)<1e-5);
assert_checktrue(abs(gain-gain_ref)<1e-5);
gain_ref=0.1880517;
po_ref=[-74.045505+159.77986*%i,-74.045505-159.77986*%i];
ze_ref=[384.0618*%i -384.0618*%i];
[ze,po,gain]=zpell(0.5,5,150,300);
assert_checktrue(abs(ze-ze_ref)<1e-5);
assert_checktrue(abs(po-po_ref)<1e-5);
assert_checktrue(abs(gain-gain_ref)<1e-5);
gain_ref=0.1656341;
po_ref=[-43.753882+156.42721*%i -43.753882-156.42721*%i];
ze_ref=[344.09218*%i -344.09218*%i];
[ze,po,gain]=zpell(0.9,5,150,300);
assert_checktrue(abs(ze-ze_ref)<1e-5);
assert_checktrue(abs(po-po_ref)<1e-5);
assert_checktrue(abs(gain-gain_ref)<1e-5);

// Check example in ell1mag help page
deff("[alpha,BeTa]=alpha_beta(n,m,m1)",...
"if 2*int(n/2)==n then, BeTa=K1; else, BeTa=0;end;alpha=%k(1-m1)/%k(1-m);")
epsilon=0.1;A=10;
m1=(epsilon*epsilon)/(A*A-1);n=5;omegac=6;
m=find_freq(epsilon,A,n);omegar = omegac/sqrt(m);
[alpha,Beta]=alpha_beta(n,m,m1);
assert_checktrue(abs(alpha*delip(1,sqrt(m))-n*%k(m1)-3.553e-15)<1e-6);

// Check example in %k help page
ref=1.7775194;
assert_checktrue(abs(delip(1,sqrt(0.4))-ref)<1e-5);
