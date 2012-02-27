// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 -  INRIA - Serge Steer
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=%spb_cumsum(a,d,typ)
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
  
  if typ<>list('native') then
    r=sparse(dims,0,dims);
  else
    r=sparse(dims,%f,dims);
  end
  
  select d
  case 0 then
    ij=spget(a)
    if size(ij,1)==0 then return,end
    if dims(1)==1 then
        mnj=min(ij(:,2))
        r(1,mnj:$)=sparse(cumsum(full(a(1,mnj:$)),typ(:)))
    elseif dims(2)==1 then
      mni=min(ij(:,1))
      r(mni:$,1)=sparse(cumsum(full(a(mni:$,1)),typ(:)))
    else
      mnj=min(ij(:,2))
      r(:,mnj:dims(2))=sparse(cumsum(full(a(:,mnj:$)),typ(:)))
    end
  case 1 then
    for k=1:dims(2)
      ij=spget(a(:,k))
      if size(ij,1)<>0 then
        mni=min(ij(:,1))
        r(mni:$,k)=sparse(cumsum(full(a(mni:$,k)),typ(:)))
      end
    end
  case 2 then
    for k=1:dims(1)
      ij=spget(a(k,:))
      if size(ij,1)<>0 then
        mnj=min(ij(:,2))
        r(k,mnj:$)=sparse(cumsum(full(a(k,mnj:$)),typ(:)))
      end
    end
  else
    if typ==list('native') then
      r=a
    else
      r=bool2s(a)
    end
  end
 
endfunction
