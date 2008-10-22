// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [t]=gencall(arg)
//gencall(arg) genere une instruction call fortran  a partir du vecteur de
//chaine arg dont le premier element est le nom du sous programme et les
//suivants les arguments d'appels
//!
n=prod(size(arg))
t=' call '+arg(1)+'('
if n==0 then return,end
for k=2:n-1,
  t=t+arg(k)+','
end
t=t+arg(n)+')'
endfunction
