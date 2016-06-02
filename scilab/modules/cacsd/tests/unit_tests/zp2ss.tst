//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
function y=csort(x)
  eps=1e-10
  y=gsort(real(x(abs(imag(x))<eps)),"g","i")
  y=[y;gsort(x(imag(x)<-eps),"g","i")]
  y=[y;gsort(x(imag(x)>eps),"g","i")] 
endfunction


Z=[2;1;0.5-%i;0.5+%i];P=[4;5;3-%i;3+%i];K=1;
[z,p,k]=ss2zp(zp2ss(Z,P,K));
assert_checkalmostequal(csort(Z),csort(z));
assert_checkalmostequal(csort(P),csort(p));
assert_checkalmostequal(K,k);

Z=[0.5;0.6];P=[4;5;3-%i;3+%i];K=1;
[z,p,k]=ss2zp(zp2ss(Z,P,K));
assert_checkalmostequal(csort(Z),csort(z));
assert_checkalmostequal(csort(P),csort(p));
assert_checkalmostequal(K,k);

Z=[0.5];P=[4;5;3-%i;3+%i];K=1;
[z,p,k]=ss2zp(zp2ss(Z,P,K));
assert_checkalmostequal(csort(Z),csort(z));
assert_checkalmostequal(csort(P),csort(p));
assert_checkalmostequal(K,k);


Z=[2;1;0.5-%i;0.5+%i];P=[5-%i;5+%i;3-%i;3+%i];K=1;
[z,p,k]=ss2zp(zp2ss(Z,P,K));
assert_checkalmostequal(csort(Z),csort(z));
assert_checkalmostequal(csort(P),csort(p));
assert_checkalmostequal(K,k);

Z=[2;1;0.5-%i;0.5+%i;8];P=[5-%i;5+%i;7;3-%i;3+%i];K=1;
[z,p,k]=ss2zp(zp2ss(Z,P,K));
assert_checkalmostequal(csort(Z),csort(z));
assert_checkalmostequal(csort(P),csort(p));
assert_checkalmostequal(K,k);

Z=[2];P=[5-%i;5+%i;7;3-%i;3+%i];K=1;
[z,p,k]=ss2zp(zp2ss(Z,P,K));
assert_checkalmostequal(csort(Z),csort(z));
assert_checkalmostequal(csort(P),csort(p));
assert_checkalmostequal(K,k);


Z=[2,1-%i;1,1+%i;0.5-%i,2;0.5+%i,%inf;8,%inf] ;
P=[5-%i;5+%i;7;3-%i;3+%i];K=[1;3];
[z,p,k]=ss2zp(zp2ss(Z,P,K));
assert_checkalmostequal(csort(Z(:,1)),csort(z(:,1)));
assert_checkalmostequal(csort(Z(:,2)),csort(z(:,2)));
assert_checkalmostequal(csort(P),csort(p));
assert_checkalmostequal(K,k);
