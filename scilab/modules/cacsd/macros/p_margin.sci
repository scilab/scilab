// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [phm,fr]=p_margin(h)



if type(h)<>16 then error(97,1),end
flag=h(1)
select flag(1)
 case 'r' then ,
 case 'lss' then h=ss2tf(h)
 else error(97,1),
end;
//
[n,d]=h(['num','den']);w=poly(0,'w')
// 
niw=horner(n,%i*w);diw=horner(d,%i*w)
w=roots(real(niw*conj(niw)-diw*conj(diw)))
//recherche des racines reelles positives
eps=1.e-7
ws=w(find((abs(imag(w))<eps)&(real(w)>0)))
if ws==[] then phm=[],fr=[],return,end
//
f=freq(n,d,%i*ws);
phm=atan(imag(f),real(f))
phm=180*phm(:)/%pi 
fr=real(ws)/(2*%pi) 
endfunction
