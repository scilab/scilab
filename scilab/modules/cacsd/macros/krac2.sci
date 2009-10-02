// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function kp=krac2(n)

  select typeof(n)
  case 'rational' then
    [n,d,dom]=n(['num','den','dt'])
  case 'state-space' then
    n=ss2tf(n);[n,d,dom]=n(['num','den','dt'])
  else
     error(msprintf(gettext("%s: Wrong type for input argument #%d: Linear state space or a transfer function expected.\n"),"krac2",1))
  end
  if dom<>'c' then 
    error(msprintf(gettext("%s: Wrong value for input argument #%d: Continuous time system expected.\n"),"krac2",1))
  end
  if size(n,'*')<>1 then
    error(msprintf(gettext("%s: Wrong size for input argument #%d: Single input, single output system expected.\n"),"krac2",1))
  end

  x=[];
  q1=derivat(n/d);s=roots(q1(2));
  //
  for a=s',
    if abs(imag(a))<=10*%eps then 
      x=[x;a],
    end,
  end
  //x(x==0)=[]
  if x==[] then;return,end
  kp=gsort(-real(freq(d,n,real(x))))
endfunction
