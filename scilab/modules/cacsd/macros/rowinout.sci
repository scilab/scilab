function [Inn,X,Gbar]=rowinout(G)
// Inner-outer factorization (and row compression) of (lxp) G =:[A,B,C,D] with l>=p
// G is assumed to be tall (l>=p) without zero on the imaginary axis
// and with a D matrix which is full column rank. G must also be stable for
// having Gbar stable.
//
// G admits the following inner-outer factorization:
//
//          G = [ Inn ] | Gbar |
//                      |  0   |
//
// where Inn is square and inner (all pass and stable) and Gbar square and outer i.e:
// Gbar is square bi-proper and bi-stable (Gbar inverse is also proper and stable);
//
// Note that:
//          [ Gbar ]
//    X*G = [  -   ]
//          [  0   ]
// is a row compression of G where X = Inn inverse is all-pass:
//               T
// X is lxl and X (-s) X(s) = Identity (all-pass property).
//
// Copyright INRIA
G1=G(1);
flag='ss';if G1(1)=='r' then flag='tf';G=tf2ss(G);end
[rows,cols]=size(G);
[A,B,C,D]=abcd(G);
sqD=inv(sqrtm(D'*D));
//----------------
[w,rt]=rowcomp(D);
Dorto=w(rt+1:rows,:)';
// Inner factor:
//-------------
[F,Xc]=lqr(G);
Af=A+B*F;Cf=C+D*F;
Inn=syslin('c',Af,[B*sqD,-pinv(Xc)*C'*Dorto],Cf,[D*sqD,Dorto]);
X=invsyslin(Inn);
//----------------------------
// Outer factor:
Gbar=invsyslin(syslin('c',Af,B*sqD,F,sqD));
if flag=='tf' then Inner=ss2tf(Inner);Gbar=ss2tf(Gbar);X=ss2tf(X);end
endfunction
