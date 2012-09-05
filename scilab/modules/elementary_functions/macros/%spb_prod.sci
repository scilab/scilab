// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 -  INRIA - Serge Steer
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=%spb_prod(a,d,typ)
  if argn(2)==1 then
    typ=list()
    d='*'
  elseif argn(2)==2 then 
    if argn(2)==2& or(d==['native','double']) then
      typ=list(d)
      d='*'
    else
      typ=list()
    end
  else
    typ=list(typ)
  end
  if size(d,'*')<>1 then 
    if type(d)==10 then
      error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"prod",2))
    else
      error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"prod",2))
    end
  end
 
  if type(d)==10 then
    d=find(d==['m','*','r','c'])
    if d==[] then
      error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                     "prod",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
    end
    d=d-2
  end
  dims=size(a);
    
  if d==-1 then //'m'
    d=find(dims>1,1)
    if d==[] then d=0,end
  end
  if d<0 then
    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                     "prod",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
  end

  select d
  case 0 then
     ij=spget(a)
     r=size(ij,1)==prod(dims)
     if typ<>list('native') then r=bool2s(r),end
  case 1 then
    for k=1:dims(2)
      r(1,k)=prod(a(:,k),typ(:))
    end
    r=sparse(r)
  case 2 then
    for k=1:dims(1)
      r(k,1)=prod(a(k,:),typ(:))
    end
    r=sparse(r)
  else
    r=a
    if typ<>list('native') then r=bool2s(r),end
  end

endfunction
