// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 -  INRIA - Serge Steer
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function a=%p_sum(a,d,typ)
//sum of a polynomial matrix. Sum of Polynomial hypermatrices are
//handled by %hm_sum
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
  dims=size(a);
    
  if d==-1 then //'m'
    d=find(dims>1,1)
    if d==[] then d=0,end
  end
  select d
  case 0 then
    a=sum(a)
  case 1 then
    a=ones(1,dims(1))*a;
  case 2 then
    a=a*ones(dims(2),1);  
  else
    a=a
  end

endfunction
