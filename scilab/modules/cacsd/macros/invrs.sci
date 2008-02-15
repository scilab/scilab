// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [Sli]=invrs(Sl,alfa);
// Sli=invrs(Sl,alfa) computes Sli, the PSSD
// inverse of PSSD Sl.
//!

D=Sl(5);
if type(D)==2 then 
  s=poly(0,varn(D));
  Sl(5)=horner(Sl(5),s+alfa);
end
Sl(2)=Sl(2)-alfa*eye(Sl(2)); //Slnew(s)=Slold(s+alfa)

[Sreg,Wss]=rowregul(Sl,0,0);
if rcond(Sreg(5)) >1.d-6 then
  Sli=invsyslin(Sreg)*Wss;
else
  error('square but singular system');
end
[Q,M]=pbig(Sli(2),0.001,'d');
Sli=projsl(Sli,Q,M);//Remove poles at zero.

if Sli(2)~=[] then Sli(2)=Sli(2)+alfa*eye();end
if type(Sli(5))==2 then 
  Sli(5)=horner(Sli(5),s-alfa);
end
endfunction
