
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x=diff(x,N,DIM)
  rhs=argn(2)
  dims=size(x),nd=prod(size(dims))
  if rhs<3 then DIM='*',end
  if rhs<2 then N=1,end
  if DIM=='r' then DIM=1,end
  if DIM=='c' then DIM=2,end
  if DIM=='*' then DIM=-1,end
  if size(DIM,'*')<>1 then 
    error(msprintf(gettext("%s: Wrong size for input argument #%d: A scalar expected.\n"),'diff',1));
  end
  DIM=round(DIM)
  if DIM<>-1&DIM<1 then 
    error(msprintf(gettext("%s: Wrong values for input argument #%d: Non-negative integers expected.\n"),'diff',3));
  end
  if DIM>nd then x=[],return,end
  
  if type(N)<>1|size(N,'*')<>1 then 
    error(msprintf(gettext("%s: Wrong size for input argument #%d: A scalar expected.\n"),'diff',2));
  end
  N=round(N)
  if N<0 then 
    error(msprintf(gettext("%s: Wrong values for input argument #%d: Non-negative integers expected.\n"),'diff',3));
  end
 
  
  if N==1 then
    if DIM==-1 then
      x=x(2:$)-x(1:$-1)
    else
      args=emptystr(1,nd)+':'
      arg1=args;arg1(DIM)='2:$';
      arg2=args;arg2(DIM)='1:$-1';
      execstr('x=x('+strcat(arg1,',')+')-x('+strcat(arg2,',')+')')
    end
  else
    for i=1:N, x=diff(x,1,DIM),end
  end
endfunction
