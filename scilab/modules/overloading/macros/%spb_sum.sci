// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 -  INRIA - Serge Steer
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=%spb_sum(a,d,typ)
  rhs=argn(2)
  if rhs==1 then
    d='*'
    typ="double"
  elseif rhs==2 then 
    if or(d==['native','double']) then
      typ=d
      d='*'
    else
      typ="double"
    end
  end
  
  if size(d,'*')<>1 then 
    if type(d)==10 then
      error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"sum",2))
    else
      error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"sum",2))
    end
  end
  dims=size(a)
  if type(d)==10 then
    d=find(d==['m','*','r','c'])
    if d==[] then
      error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                     "sum",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
    end
    d=d-2
  end
  
  if d==-1 then 
    //sum(x,'m'), determine the summation direction
    d=find(dims>1,1)
    if d==[] then d=0,end
  end
  if d<0 then
    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                     "sum",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
  end
  
  ij=spget(a);
  select d
  case 0 then
    //sum of all elements
    if typ=='native' then
      r=size(ij,1)<>0
    else
      r=size(ij,1)
    end
  case 1 then
    if typ=='native' then
      r(1,dims(2))=%f
      for k=1:dims(2)
        r(k)=or(ij(:,2)==k)
      end
    else
      r=zeros(1,dims(2));
      for k=1:dims(2)
        r(k)=size(find(ij(:,2)==k),'*')
      end
    end
    r=sparse(r)
  case 2 then
    if typ=='native' then
      r(dims(1),1)=%f
      for k=1:dims(1)
        r(k)=or(ij(:,1)==k)
      end
    else
      r=zeros(ndims(1),1);
      for k=1:dims(1)
        r(k)=size(find(ij(:,1)==k),'*')
      end
    end
    r=sparse(r)
  else
    if typ=='native' then
      r=a
    else
      r=sparse(ij,ones(size(ij,1),1),dims)
    end
  end
 
endfunction
