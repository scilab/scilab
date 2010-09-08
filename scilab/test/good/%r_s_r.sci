// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function f=%r_s_r(s1,s2)
//f=s1-s2 s1,s2 : rational matrices
//!
  if ndims(s1)<2&ndims(s2)<2 then [s1,s2]=sysconv(s1,s2),end
  [num1,den1]=s1(['num','den']),
  [num2,den2]=s2(['num','den']),
  sz1=size(num1)
  sz2=size(num2)
  if and(sz1>=0)&and(sz2>=0) then
    num1=num1(:);den1=den1(:);
    num2=num2(:);den2=den2(:);

    if prod(sz1)==1&prod(sz2)>1 then
      den1=den1(ones(den2))
      num1=num1(ones(num2))
      sz1=sz2
    elseif prod(sz2)==1&prod(sz1)>1 then
      den2=den2(ones(den1))
      num2=num2(ones(num1))
      sz2=sz1
    end
    if and(sz1<>sz2) then error(8),end
    for l=1:prod(sz1)
      [den,fact]=lcm([den1(l);den2(l)])
      num1(l)=[num1(l),-num2(l)]*fact
      den1(l)=den
    end,
    [num1,den1]=simp(num1,den1),
    f=rlist(matrix(num1,sz1),matrix(den1,sz1),s1.dt)
  else

    if size(sz1,'*')>2|size(sz2,'*')>2 then error(8),end
    
    if or(sz1<0)&or(sz2<0) then
      // both are eye*x
      [den1,fact]=lcm([den1;den2])
      [num1,den1]=simp([num1,-num2]*fact,den1)
      f=rlist(num1*eye(),den1*eye(),s1('dt')),
    elseif  or(sz1<0) then
      den1=den1+0;num1=num1+0
      for l=1:min(sz2)
	[den,fact]=lcm([den1;den2(l,l)])
	num2(l,l)=[num1,-num2(l,l)]*fact
	den2(l,l)=den
      end
      [num2,den2]=simp(num2,den2),
      f=rlist(num2,den2,s1.dt)
    elseif or(sz2<0) then
      den2=den2+0;num2=num2+0
      for l=1:min(sz1)
	[den,fact]=lcm([den1(l,l);den2])
	num1(l,l)=[num1(l,l),-num2]*fact
	den1(l,l)=den
      end
      [num1,den1]=simp(num1,den1),
      f=rlist(num1,den1,s1.dt)
    end
  end
endfunction
