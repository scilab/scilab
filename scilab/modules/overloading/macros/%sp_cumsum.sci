// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 -  INRIA - Serge Steer
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=%sp_cumsum(a,d,typ)
  rhs=argn(2)
  if rhs==1 then
    d='*'
  elseif rhs==2 then 
    if argn(2)==2& or(d==['native','double']) then
      d='*'
    end
  end
  if size(d,'*')<>1 then 
    if type(d)==10 then
      error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"cumsum",2))
    else
      error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"cumsum",2))
    end
  end
  if type(d)==10 then
    d=find(d==['m','*','r','c'])
    if d==[] then
      error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                     "cumsum",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
    end
    d=d-2
  end
  
  dims=size(a)

  if d==-1 then 
    //sum(x,'m'), determine the summation direction
    d=find(dims>1,1)
    if d==[] then d=0,end
  end
  if d<0 then
    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                     "cumsum",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
  end
  
  select d
  case 0 then
    ij=spget(a)
    if size(ij,1)==0 then r=a,return,end
    mnj=min(ij(:,2));
    r=sparse([],[],dims);
    r(1:dims(1),mnj:dims(2))=sparse(cumsum(full(a(:,mnj:$))))
  case 1 then
    r=sparse([],[],dims);
    for k=1:dims(2)
      r(:,k)=cumsum(a(:,k))
    end
  case 2 then
    r=sparse([],[],dims);
    for k=1:dims(1)
      r(k,:)=cumsum(a(k,:))
    end
  else
    r=a
  end
 
endfunction
