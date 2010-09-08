// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function a=%r_r_s(a,b)
// a/b a rational matrix, b scalar matrix
//author Serge Steer
//!
  if size(b,'*')==0 then a=[],return,end
  [mb,nb]=size(b);
  if mb*nb==1 then 
    a.num=a.num/b,
    if or(size(a)==-1)|mb==-1 then 
      a.den=a.den*eye(),
    end
    return,
  end
  [ma,na]=size(a.num)
  ma=abs(ma);na=abs(na) //handling eye()
  mb=abs(mb);nb=abs(nb)  //handling eye()
  if na==1 then
    a=rlist(a.num/b,ones(nb,mb)*a.den,a.dt)
  else
    [num,den]=a(['num','den']);
    dd=[];nn=[]
    for i=1:ma,
      [y,fact]=lcm(den(i,:)),
      nn=[nn;(num(i,:).*fact)/b];
      dd=[dd;y]
    end
    [num,den]=simp(nn,dd*ones(1,mb))
    a=rlist(num,den,a.dt)
  end
endfunction
