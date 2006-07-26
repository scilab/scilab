function [z]=eval3d(fun,x,y)
//Etant donnes une discretisation des abscisses (x) et des ordonnees (y)
//  eval3d(fun,x [,y]) retourne la matrice [fun(x(i),y(j))] des valeurs de la
//  fonction decrite par la macro fun aux points (x(i),y(j)).
//Attention fun doit savoir gerer des arguments x et y vectoriels.
//  Si fun ne sait pas gerer des arguments vectoriels, utiliser 
//  feval(x,y,fun) 
//Si y n'est pas fourni il est suppose identique a x
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==3 then
  nx=prod(size(x));ny=prod(size(y))
  z=matrix(fun(ones(1,ny).*.matrix(x,1,nx),matrix(y,1,ny).*.ones(1,nx)),nx,ny)
else
  nx=prod(size(x))
  z=matrix(fun(ones(1,nx).*.matrix(x,1,nx),matrix(x,1,nx).*.ones(1,nx)),nx,nx)
end
endfunction
