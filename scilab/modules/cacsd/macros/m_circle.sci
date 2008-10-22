// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function []=m_circle(gain)
// Copyright INRIA
[lhr,rhs]=argn(0)
//
d36=11;
if rhs<1 then 
 gain=[-12 -8 -6 -5 -4 -3 -2 -1.4 -1 -.5 0.25 0.5 0.7 1 1.4 ..
                2 2.3 3 4 5 6 8 12]
else
 gain=matrix(gain,1,prod(size(gain)))
end
//
titre='isogain contours for y/(1+y)'
l10=log(10);
//
lambda=exp(l10*gain/20)
rayon=lambda./(lambda.*lambda-ones(lambda))
centre=-lambda.*rayon
rayon=abs(rayon)
rect=[mini(centre-rayon),mini(-rayon),maxi(centre+rayon),maxi(rayon)];
//
drawlater()
ax=gca();
llrect=xstringl(0,0,'1')
hx=llrect(3);
//
for i=1:prod(size(gain))
  sgn=1;if 2*int(i/2)==i then sgn=-1,end
  g=string(gain(i)),ng=length(g)
  if gain(i)<0 then 
    w=0:0.03:%pi;
    xx=centre(i)+rayon(i)*sin(%pi/3)+hx
    xy=sgn*cos(%pi/3)*rayon(i)
  else 
    xx=centre(i)-sin(%pi/3)*rayon(i)-hx*ng
    xy=sgn*cos(-%pi/3)*rayon(i)
    w=-%pi:0.03:0;
  end;
  n=prod(size(w))
  rf=centre(i)*ones(w)+rayon(i)*exp(%i*w);
  xpoly([real(rf) real(rf($:-1:1))],[imag(rf)  -imag(rf($:-1:1))])
  e=gce();e.foreground=3;e.clip_state='clipgrf'
//  xstring(xx,xy,g)
drawnow();
end;
endfunction
