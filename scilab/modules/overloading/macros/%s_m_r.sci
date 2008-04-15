// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function f2=%s_m_r(n1,f2)
// %s_m_r(n1,f2)  
//operation  f2=n1*f2
//author Serge Steer INRIA
//!
  [n2,d2]=f2(['num','den']);
  sz1=size(n1);sz2=size(n2);
  if prod(sz1)==0|prod(sz2)==0 then  f2=[],return,end


  indef=%f
  if or(sz1==-1) then 
    n1=n1+0;sz1=[1 1];
    if  prod(sz2)==1 then indef=%t,else error(14),end
  end
  if  or(sz2==-1) then 
    n2=n2+0;d2=d2+0;sz2=[1 1];
    if prod(sz1)==1 then indef=%t,else error(14),end
  end  

  //
  if prod(sz1)==1 then
    num=n1*n2,
    den=d2
  elseif prod(sz2)==1 then
    num=n1*n2,
    den=d2(ones(n1))
  else,
    if size(sz1,'*')>2|size(sz2,'*')>2 then error(10),end
    if sz1(2)<>sz2(1) then error(10),end,
    l1=sz1(1);m1=sz1(2);m2=sz2(2);
    for j=1:m2,
      [y,fact]=lcm(d2(:,j)),
      n2(:,j)=n2(:,j).*fact,
      den(1:l1,j)=ones(l1,1)*y,
      for i=1:l1,
	num(i,j)=n1(i,:)*n2(:,j),
      end,
    end,
    [num,den]=simp(num,den),
  end,

  if indef then
    num=num*eye()
    den=den*eye()
  end
  f2=rlist(num,den,f2.dt)
endfunction
