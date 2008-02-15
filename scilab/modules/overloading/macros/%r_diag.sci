// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function d=%r_diag(a,k)
// %r_diag - implement diag function for  rational matrix ,..

[lhs,rhs]=argn(0)
if rhs==1 then k=0,end
[m,n]=size(a('num'))
if m<>1&n<>1 then
  d=syslin(a('dt'),diag(a('num'),k),diag(a('den'),k))
else
  mn=max(m,n)
  den=ones(mn,mn)
  den=den-diag(diag(den,k))+diag(a('den'),k)
  d=syslin(a('dt'),diag(a('num'),k),den)
end
endfunction
