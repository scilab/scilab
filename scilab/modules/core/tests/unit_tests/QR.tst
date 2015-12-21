//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// Unit test for Scilab QR
pi=%pi;
i=%i;
e=%e;

a=rand(3,4);b=rand(3,4);ac=a+i*rand(3,4);bc=b+i*rand(3,4);
//
assert_checktrue(abs(a*(1/a)*a-a)< 10*%eps);
assert_checktrue(abs(a*(i/a)*a-i*a)< 10*%eps);
assert_checktrue(abs(a*(a\ 1)*a-a)< 10*%eps);
assert_checktrue(abs(a*(a\ i)*a-i*a)< 10*%eps);
assert_checktrue(abs(ac*(1/ac)*ac-ac)< 10*%eps);
assert_checktrue(abs(ac*(i/ac)*ac-i*ac)< 10*%eps);
assert_checktrue(abs(ac*(ac\ 1)*ac-ac)< 10*%eps);
assert_checktrue(abs(ac*(ac\ i)*ac-i*ac)< 10*%eps);
//
assert_checktrue(abs(a/b-a*(1/b))< 10*%eps);
assert_checktrue(abs(ac/b-ac*(1/b))< 10*%eps);
assert_checktrue(abs(a/bc-a*(1/bc))< 10*%eps);
assert_checktrue(abs(ac/bc-ac*(1/bc))< 10*%eps);
//
assert_checktrue(abs(a\ b -(a\ 1)*b)< 10*%eps);
assert_checktrue(abs(a\ bc-(a\ 1)*bc)< 10*%eps);
assert_checktrue(abs(ac\ b-(ac\ 1)*b)< 10*%eps);
assert_checktrue(abs(ac\ bc-(ac\ 1)*bc)< 10*%eps);
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
assert_checktrue(abs(cond(diag([1 i 3 4]))-4)< 10*%eps);
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
v=[i 2 3 4 5];
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
a=[diag([i,2,3]),[0;0;0]];
assert_checktrue(abs(norm(a,'inf')-3)< 10*%eps);
assert_checktrue(abs(norm(a,2)-3)< 10*%eps);
assert_checktrue(abs(norm(a,'fro')-sqrt(14))< 10*%eps);
a=a';
assert_checktrue(abs(norm(a,1)-3)< 10*%eps);
assert_checktrue(abs(norm(a,'inf')-3)< 10*%eps);
assert_checktrue(abs(norm(a,2)-3)< 10*%eps);
assert_checktrue(abs(norm(a,'fro')-sqrt(14))< 10*%eps);
//
a=rand(3,5);ac=a+i*rand(3,5);
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
a=rand(3,5);ac=a+i*rand(3,5);
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
