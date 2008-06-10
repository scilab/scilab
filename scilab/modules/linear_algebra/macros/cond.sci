
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function c=cond(A)
// c=cond(A) condition number for 2-norm
  if type(A)==1 then
    if A==[] then c=1,return,end
    if size(A,1)<>size(A,2) then 
      error(msprintf(gettext("%s: Wrong size for input argument #%d: A square matrix expected.\n"),'cond',1));
    end
    s=svd(A)
    if s($)==0 then
      c=%inf
    else
      c=s(1)/s($)
    end
  else
    if type(A)==16|type(A)==17 then
      n=getfield(1,A);n=n(1)
    else
      [t,n]=typename()
      n=stripblanks(n(find(t==type(A))))
    end
    fun='%'+n+'_cond'
    if exists(fun)==1 then
      execstr('y='+fun+'(A,flag)')
      
    else
       error(msprintf(gettext('%s: Type not defined for ''%s''. Check argument or define function %s.'),'cond',n,fun));
    end
  end
endfunction
