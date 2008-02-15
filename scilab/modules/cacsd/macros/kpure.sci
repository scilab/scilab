// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [y,R]=kpure(n)
//!

y=[],R=[]
if type(n)<>16 then error(92,1),end;
flag=n(1);
if flag(1)=='lss' then n=ss2tf(n),end
if flag(1)<>'r' then  error(90,1),end;
if n('dt')<>'c' then error('System must be continuous'),end
if size(n('num'),'*') > 1 then error(95,1),end

r=routh_t(n,poly(0,'k')),
[n,d]=n(['num','den'])
[s,t]=size(r);
for i=1:s,
  coe=coeff(r(i,:)),
  if coe==0*ones(1,t) then error('---> infinite solution'),end
end,

z=0;u=0;eps=1e-7;

for i=1:s,
  t=prod(size(r(i,:)));
  gd=r(i,1);
  for j=2:t, 
    [gd,u]=bezout(gd,r(i,j)), 
  end
  k=roots(gd)
  h=prod(size(k)),
  if h>0 then
    for j=1:h,
      if and(real(k(j))<>y) then
	wr=roots(k(j)*n+d)
	[w,ki]=mini(abs(real(wr)))
	if w<eps then 
	  y=[y real(k(j))],
	  R=[R wr(ki)]
	end
      end
    end,
  end;
end;

[y,k]=sort(y)
R=R(k)
endfunction
