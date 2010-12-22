// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 -  INRIA - Serge Steer
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function a=%r_cumprod(a,d,typ)
//prod of a rational matrix
  if argn(2)==1 then
    d='*'
  elseif argn(2)==2 then 
    if argn(2)==2& or(d==['native','double']) then
      d='*'
    end
  end
  if size(d,'*')<>1 then 
    if type(d)==10 then
      error(msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"),"cumprod",2))
    else
      error(msprintf(_("%s: Wrong size for input argument #%d: A scalar expected.\n"),"cumprod",2))
    end
  end
  dims=size(a);
  
  if type(d)==10 then
    d=find(d==['m','*','r','c'])
    if d==[] then
      error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                     "cumprod",2,"""*"",""r"",""c"",""m"",1:"+string(size(dims,'*'))))
    end
    d=d-2
  end
     
  if d==-1 then //'m'
    d=find(dims>1,1)
    if d==[] then d=0,end
  end
  if d<0 then
    error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
                     "cumprod",2,"""*"",""r"",""c"",""m"",1:"+string(ndims(a))))
  end
  if d==0 then //'*'
    a=rlist(cumprod(a.num),cumprod(a.den),a.dt)
    if simp_mode() then a=simp(a),end
  elseif or(d==[1 2]) then
    a=rlist(cumprod(a.num,d),cumprod(a.den,d),a.dt)
    if simp_mode() then a=simp(a),end
  else
    a=a
  end

endfunction
