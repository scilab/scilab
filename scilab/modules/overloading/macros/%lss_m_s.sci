// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Serge Steer , F.D
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function s=%lss_m_s(s1,D2)
// s=s1*gain
// SISO case FD
[A1,B1,C1,D1,x1,dom1]=s1(2:7);
[n2,m2]=size(D2);
if prod(size(s1))==1 then
 if n2==1 then 
   D=D1*D2; [A1,B1*D2]; 
   s=tlist(['lss','A','B','C','D','X0','dt'],A1,B1*D2,C1,D,x1,dom1);
   return;
 end
 if m2==1 then 
   s=tlist(['lss','A','B','C','D','X0','dt'],A1,B1,D2*C1,D2*D1,x1,dom1);
   return;
 end   
 [Q,M]=fullrf(D2);[n2,mq]=size(Q);
 if mq==1 then 
   s=Q*tlist(['lss','A','B','C','D','X0','dt'],A1,B1*M,C1,D1*M,x1,dom1);
   return;
 end  
 w=s1; 
 for k=2:mq, w=sysdiag(w,s1);end
 s=w*M;s=Q*s;
 return;
end
D=D1*D2; 
s=tlist(['lss','A','B','C','D','X0','dt'],A1,B1*D2,C1,D,x1,dom1);
endfunction
