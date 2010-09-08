// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x=%hm_sum(m,d,typ)
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
      error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"sum",2))
    else
      error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"sum",2))
    end
  end
 
  if type(d)==10 then
    d=find(d==['m','*','r','c'])
    if d==[] then
      error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                     "sum",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
    end
    d=d-2
  end
  dims=m.dims;
    
  if d==-1 then 
    //sum(x,'m'), determine the summation direction
    d=find(dims>1,1)
    if d==[] then d=0,end
  end
  if d<0 then
    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                     "sum",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
  end
  
  if d==0 then 
    //sum of all elements
     x=sum(m.entries,typ(:))
     return
  end
  if d>size(dims,'*') then
    //requested summation direction exceeds array dims, return the array, converted
    //to double if necessary.
    tm=type(m.entries)
    if (tm==8&typ==list('double'))|(tm==4&typ<>list('native')) then 
        m.entries=double(m.entries),
    end    
    x=m
    return
  end


  if type(dims==8) then flag=1; dims=double(dims); else flag=0;end
  N=size(dims,'*');
  p1=prod(dims(1:d-1));// step to build one vector on which sum is applied
  p2=p1*dims(d);//step for beginning of next vectors
  ind=(0:p1:p2-1)';// selection for building one vector
  deb=(1:p1);
  I=ind*ones(deb)+ones(ind)*deb 

  ind=(0:p2:prod(dims)-1);
  I=ones(ind).*.I+ind.*.ones(I)
  x=sum(matrix(m.entries(I),dims(d),-1),1,typ(:))

  dims(d)=1
  while  dims($)==1 then dims($)=[],end
  if d==N then
    dims=dims(1:$)
  else
    dims(d)=1
  end
  if size(dims,'*')==2 then 
    x=matrix(x,dims(1),dims(2))
  elseif dims<>[] then
    if flag==1 then dims=int32(dims);end
    x=hypermat(dims,x)
  end

endfunction
