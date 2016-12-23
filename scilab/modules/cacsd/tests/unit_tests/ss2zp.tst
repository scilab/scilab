//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
function y=csort(x)
    eps=1e-10;
    y=gsort(real(x(abs(imag(x))<eps)),"g","i");
    y=[y;gsort(x(imag(x)<-eps),"g","i")];
    y=[y;gsort(x(imag(x)>eps),"g","i")];
endfunction

h=syslin("c",3.5+1.5*%s+2.5*%s^2,0.5+1.5*%s+%s^2);
S =  tf2ss(h);
[z,p,k]=ss2zp(S);
assert_checkalmostequal(k,2.5); //faux
assert_checkalmostequal(csort(p),csort(roots(h.den)));
assert_checkalmostequal(csort(z),csort(roots(h.num)));

h=syslin("c",3.5+1.5*%s+2.5*%s^2,0.5+1.5*%s+%s^3);
S =  tf2ss(h);
[z,p,k]=ss2zp(S);
assert_checkalmostequal(k,2.5);
assert_checkalmostequal(csort(p),csort(roots(h.den)));
assert_checkalmostequal(csort(z),csort(roots(h.num)));

h=syslin("c",3*%s+7,2*%s^2+3*%s+1);
S =  tf2ss(h);
[z,p,k]=ss2zp(S);
assert_checkalmostequal(k,1.5);
assert_checkalmostequal(gsort(p),[-1;-0.5]);
assert_checkalmostequal(z,-7/3);

h=syslin("c",3,2*%s^2+3*%s+1);
S =  tf2ss(h);
[z,p,k]=ss2zp(S);
assert_checkalmostequal(k,1.5);
assert_checkalmostequal(gsort(p),[-1;-0.5]);
assert_checkequal(z,[]);

h=syslin("c",3,-2*%s^4+3*%s+1);
S =  tf2ss(h);
[z,p,k]=ss2zp(S);
assert_checkalmostequal(k,-1.5);
assert_checkalmostequal(gsort(p),gsort(roots(h.den)));
assert_checkequal(z,[]);

h=syslin("c",%s^3+3*%s+7,2*%s^2+3*%s+1);
S =  tf2ss(h);
[z,p,k]=ss2zp(S);
assert_checkalmostequal(k,1/2);
assert_checkalmostequal(csort(z),csort(roots(h.num)));


h=[syslin("c",3,2*%s^2+2*%s+1);syslin("c",5*%s+7,2*%s^2+3*%s+1)];
S=tf2ss(h);
[z,p,k]=ss2zp(S);
assert_checkalmostequal(k,[3/2;5/2]);
assert_checkalmostequal(z(:,1),[-0.5;-1; %inf]);
assert_checkalmostequal(csort(z(:,2)),csort([-1.4;-0.5+%i*0.5;-0.5-%i*0.5]));
assert_checkalmostequal(csort(p),csort(spec(S.A)));

h=[syslin("c",3,2*%s^2+2*%s+1);syslin("c",5*%s+7,(%s+4)*(2*%s^2+3*%s+1))];
S=tf2ss(h);
[z,p,k]=ss2zp(S);
assert_checkalmostequal(k,[3/2;5/2]);
assert_checkalmostequal(csort(z(:,1)),csort([-4;-1;-0.5]));
assert_checkalmostequal(csort(z(:,2)),csort([-0.5+%i*0.5;-0.5-%i*0.5;-1.4]));
assert_checkalmostequal(csort(p),csort(spec(S.A)));


