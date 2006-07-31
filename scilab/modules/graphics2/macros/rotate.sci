function [xy]=rotate(xy,teta,orig)
// effectue une rotation
//
// xy   : matrice a deux lignes
// teta : angle en radian , 0 si omis
// orig : centre de la rotation, <0;0> si omis
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
select rhs
 case 2 then orig=[0;0];
 case 3 then orig=matrix(orig,2,1);
 else error(39)
end;
//
[m,n]=size(xy)
if m<>2 then  error('xy doit etre un vecteur a 2 lignes [x;y]'),end
//
xy=xy-orig*ones(1,n)
c=cos(teta),s=sin(teta)
xy=[c s;-s,c]*xy+orig*ones(1,n)
endfunction
