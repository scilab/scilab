function []=xrpoly(orig,n,r,teta)
// permet de tracer un polygone regulier
//
// orig : centre du polygone
// n    : nombre de segments
// r    : rayon du cercle le circonscrit
// teta : angle, 0 si omis
// Attention utilise l'echelle d'un appel precedent de plot2d.
//!
// Copyright INRIA
[lhs,rhs]=argn(0);
select rhs
 case 3 then b=eye(2,2);
 case 4 then b=[cos(teta) -sin(teta); sin(teta) cos(teta)];
 else error(39)
end;
//
 ang=2*%pi/n
 pt=(0:n)'*ang;xy=r*[cos(pt),sin(pt)]
 xy=ones(n+1,1)*matrix(orig,1,2)+xy*b
 xpoly(xy(:,1),xy(:,2),"lines",1);
endfunction
