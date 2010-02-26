// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [z]=eval3d(fun,x,y)
//Etant donnes une discretisation des abscisses (x) et des ordonnees (y)
//  eval3d(fun,x [,y]) retourne la matrice [fun(x(i),y(j))] des valeurs de la
//  fonction decrite par la macro fun aux points (x(i),y(j)).
//Attention fun doit savoir gerer des arguments x et y vectoriels.
//  Si fun ne sait pas gerer des arguments vectoriels, utiliser 
//  feval(x,y,fun) 
//Si y n'est pas fourni il est suppose identique a x
//!
  [lhs,rhs]=argn(0)
  if rhs==3 then
    nx=prod(size(x));ny=prod(size(y))
    z=matrix(fun(ones(1,ny).*.matrix(x,1,nx),matrix(y,1,ny).*.ones(1,nx)),nx,ny)
  elseif rhs==2 then
    nx=prod(size(x))
    z=matrix(fun(ones(1,nx).*.matrix(x,1,nx),matrix(x,1,nx).*.ones(1,nx)),nx,nx)
  else
    error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), "eval3d", 2, 3));
  end
endfunction
