function [gm]=fspecg(g)
// gm=fspecg(g) : stable factorization.
// g and gm are continuous-time linear systems in state-space form.
//       gtild(g)*g = gtild(gm)*gm
//with gm stable.
// Imaginary-axis poles are forbidden.
//
//!
// Copyright INRIA
[a,b,c,d]=g(2:5),g=[];
a=-a;
b=-b;
h=[-a',c'*c;
   0*eye(a),a];
x=ric_desc(h);h=[]
gm=syslin('c',-a'+c'*c*x,-c',b'-d'*c*x,d')';
endfunction
