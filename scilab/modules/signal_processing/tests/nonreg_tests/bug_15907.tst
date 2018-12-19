// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15907 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15907
//
// <-- Short Description -->
// filter is corrupting its input state array (Scilab 6 regression)

[cells fact zzeros zpoles] = eqiir('lp','ellip',[2*%pi/10,4*%pi/10],0.02,0.001);
h = fact*poly(zzeros,'z')/poly(zpoles,'z');
b = coeff(h.num)($:-1:1);
a = coeff(h.den)($:-1:1);
si=zeros(max(length(a),length(b))-1,1);
pulse=zeros(1000,1);
pulse(5)=1;
[impulse_response so]=filter(b,a,pulse,si);
   
assert_checkequal(si,zeros(max(length(a),length(b))-1,1));