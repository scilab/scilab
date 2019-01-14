// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// Unit test for Scilab QR
pi=%pi;
e=%e;
I3=eye(3,3);
I4=eye(4,4);
IC3=%i*I3;
IC4=%i*I4;
a=rand(3,4);b=rand(3,4);ac=a+%i*rand(3,4);bc=b+%i*rand(3,4);
//
assert_checktrue(abs(a*(I4/a)*a-a)< 10*%eps);
assert_checktrue(abs(a*(IC4/a)*a-%i*a)< 10*%eps);
assert_checktrue(abs(a*(a\ I3)*a-a)< 10*%eps);
assert_checktrue(abs(a*(a\ IC3)*a-%i*a)< 10*%eps);
assert_checktrue(abs(ac*(I4/ac)*ac-ac)< 10*%eps);
assert_checktrue(abs(ac*(IC4/ac)*ac-%i*ac)< 10*%eps);
assert_checktrue(abs(ac*(ac\ I3)*ac-ac)< 10*%eps);
assert_checktrue(abs(ac*(ac\ IC3)*ac-%i*ac)< 10*%eps);
//
assert_checktrue(abs(a/b-a*(I4/b))< 10*%eps);
assert_checktrue(abs(ac/b-ac*(I4/b))< 10*%eps);
assert_checktrue(abs(a/bc-a*(I4/bc))< 10*%eps);
assert_checktrue(abs(ac/bc-ac*(I4/bc))< 10*%eps);
//
assert_checktrue(abs(a\ b -(a\ I3)*b)< 10*%eps);
assert_checktrue(abs(a\ bc-(a\ I3)*bc)< 10*%eps);
assert_checktrue(abs(ac\ b-(ac\ I3)*b)< 10*%eps);
assert_checktrue(abs(ac\ bc-(ac\ I3)*bc)< 10*%eps);
//
//elemt-wise
a=rand(3,2);ai=a+rand(3,2)*%i;
de=2;
assert_checktrue(norm(ai.*de-ai*de,1) <1000*%eps);
assert_checktrue(norm(de.*ai-de*ai,1) <1000*%eps);
assert_checktrue(norm(ai.*de-ai*de,1) <1000*%eps);
assert_checktrue(norm(de.*ai-de*ai,1) <1000*%eps);
de=de+3*%i;
assert_checktrue(norm(ai.*de-ai*de,1) <1000*%eps);
assert_checktrue(norm(de.*ai-de*ai,1) <1000*%eps);
assert_checktrue(norm(ai.*de-ai*de,1) <1000*%eps);
assert_checktrue(norm(de.*ai-de*ai,1) <1000*%eps);
//
de=2;def=de*ones(a);
assert_checktrue(norm(a./de-a./def,1) <1000*%eps);
assert_checktrue(norm(ai./de-ai./def,1) <1000*%eps);
//
de=2+3*%i;def=de*ones(a);
assert_checktrue(norm(a./de-a./def,1) <1000*%eps);
assert_checktrue(norm(ai./de-ai./def,1) <1000*%eps);
de=2;def=de*ones(a);
assert_checktrue(norm(a.\de-a.\def,1) <1000*%eps);
assert_checktrue(norm(ai.\de-ai.\def,1) <1000*%eps);
//
de=2+3*%i;def=de*ones(a);
assert_checktrue(norm(a.\de-a.\def,1) <1000*%eps);
assert_checktrue(norm(ai.\de-ai.\def,1) <1000*%eps);

///////////
de=2;def=de*ones(a);
assert_checktrue(norm(de.\a-de.\a,1) <1000*%eps);
assert_checktrue(norm(de.\ai-def.\ai,1) <1000*%eps);
//
de=2+3*%i;def=de*ones(a);
assert_checktrue(norm(de.\a-def.\a,1) <1000*%eps);
assert_checktrue(norm(de.\ai-def.\ai,1) <1000*%eps);
de=2;def=de*ones(a);
assert_checktrue(norm(de./a-def./a,1) <1000*%eps);
assert_checktrue(norm(de./ai-def./ai,1) <1000*%eps);
//
de=2+3*%i;def=de*ones(a);
assert_checktrue(norm(de./a-def./a,1) <1000*%eps);
assert_checktrue(norm(de./ai-def./ai,1) <1000*%eps);
//
[p,r]=qr(a);
assert_checktrue(abs(p*r-a)< 100*%eps);
[p,r]=qr(a');
assert_checktrue(abs(p*r-a')< 100*%eps);
[p,r,x]=qr(a);
assert_checktrue(abs(p*r*x'-a)< 100*%eps);
[p,r]=qr(ac);
assert_checktrue(abs(p*r-ac)< 100*%eps);
[p,r,x]=qr(ac);
assert_checktrue(abs(p*r-ac*x)< 100*%eps);
//
assert_checktrue(abs(cond(diag([1 2 3 4]))-4)< 10*%eps);
assert_checktrue(abs(cond(diag([1 %i 3 4]))-4)< 10*%eps);
v=[1 2 3 4 5];
assert_checktrue(abs(norm(v,1)-15)< 10*%eps);
assert_checktrue(abs(norm(v,'inf')-5)< 10*%eps);
assert_checktrue(abs(norm(v,2)-sqrt(55))< 10*%eps);
assert_checktrue(abs(norm(v,'fro')-sqrt(55))< 10*%eps);
v=v';
assert_checktrue(abs(norm(v,1)-15)< 10*%eps);
assert_checktrue(abs(norm(v,'inf')-5)< 10*%eps);
assert_checktrue(abs(norm(v,2)-sqrt(55))< 10*%eps);
assert_checktrue(abs(norm(v,'fro')-sqrt(55))< 10*%eps);
v=[%i 2 3 4 5];
assert_checktrue(abs(norm(v,'inf')-5)< 10*%eps);
assert_checktrue(abs(norm(v,2)-sqrt(55))< 10*%eps);
assert_checktrue(abs(norm(v,'fro')-sqrt(55))< 10*%eps);
v=v';
assert_checktrue(abs(norm(v,1)-15)< 10*%eps);
assert_checktrue(abs(norm(v,'inf')-5)< 10*%eps);
assert_checktrue(abs(norm(v,2)-sqrt(55))< 10*%eps);
assert_checktrue(abs(norm(v,'fro')-sqrt(55))< 10*%eps);
a=[diag([1 2 3]);[0 0 0]];
assert_checktrue(abs(norm(a,1)-3)< 10*%eps);
assert_checktrue(abs(norm(a,'inf')-3)< 10*%eps);
assert_checktrue(abs(norm(a,2)-3)< 10*%eps);
assert_checktrue(abs(norm(a,'fro')-sqrt(14))< 10*%eps);
a=a';
assert_checktrue(abs(norm(a,1)-3)< 10*%eps);
assert_checktrue(abs(norm(a,'inf')-3)< 10*%eps);
assert_checktrue(abs(norm(a,2)-3)< 10*%eps);
assert_checktrue(abs(norm(a,'fro')-sqrt(14))< 10*%eps);
a=[diag([%i,2,3]),[0;0;0]];
assert_checktrue(abs(norm(a,'inf')-3)< 10*%eps);
assert_checktrue(abs(norm(a,2)-3)< 10*%eps);
assert_checktrue(abs(norm(a,'fro')-sqrt(14))< 10*%eps);
a=a';
assert_checktrue(abs(norm(a,1)-3)< 10*%eps);
assert_checktrue(abs(norm(a,'inf')-3)< 10*%eps);
assert_checktrue(abs(norm(a,2)-3)< 10*%eps);
assert_checktrue(abs(norm(a,'fro')-sqrt(14))< 10*%eps);
//
a=rand(3,5);ac=a+%i*rand(3,5);
[u,s,v]=svd(a);u*s*v'-a;
assert_checktrue(abs(svd(a)-diag(s))< 10*%eps);
[u,s,v]=svd(ac);u*s*v'-ac;
assert_checktrue(abs(svd(ac)-diag(s))< 10*%eps);
//
[u,s,v]=svd(a,0);u*s*v'-a;
assert_checktrue(abs(svd(a,0)-diag(s,0))< 10*%eps);
[u,s,v]=svd(ac,0);u*s*v'-ac;
assert_checktrue(abs(svd(ac,0)-diag(s))< 10*%eps);
a=a';ac=ac';
a=rand(3,5);ac=a+%i*rand(3,5);
[u,s,v]=svd(a);u*s*v'-a;
assert_checktrue(abs(svd(a)-diag(s))< 10*%eps);
[u,s,v]=svd(ac);u*s*v'-ac;
assert_checktrue(abs(svd(ac)-diag(s))< 10*%eps);
//
[u,s,v]=svd(a,0);u*s*v'-a;
assert_checktrue(abs(svd(a,0)-diag(s,0))< 10*%eps);
[u,s,v]=svd(ac,0);u*s*v'-ac;
assert_checktrue(abs(svd(ac,0)-diag(s))< 10*%eps);
//
assert_checktrue(abs(a*pinv(a)*a-a)< 10*%eps);
assert_checktrue(abs(ac*pinv(ac)*ac -ac)< 10*%eps);
a=a';ac=ac';
assert_checktrue(abs(a*pinv(a)*a-a)< 10*%eps);
assert_checktrue(abs(ac*pinv(ac)*ac-ac)< 10*%eps);
//
assert_checktrue(abs(rank(a)-3)< 10*%eps);
assert_checktrue(abs(rank(ac)-3)< 10*%eps);
