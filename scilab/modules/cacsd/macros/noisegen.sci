// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function noisegen(pas,Tmax,sig)
// This function returns in the calling env 
// a macro  [b]=Noise(t)
// Noize(t) is a piecewise constant function [k*pas,(k+1)*pas]
//The value on each constant interval are random values from  
// i.i.d  Gaussian variables of standard deviation sig. 
// The function is constant for t<=0 and t>=Tmax.
//!

dua_g=sig*rand(0:pas:Tmax,'n');
[nn1,nn2]=size(dua_g);
deff('[b]=Noise(t)','b=dua_g(min(max((t/'+string(Tmax)+...
   ')*'+string(nn2)+',1),'+string(nn2)+'))');
[dua_g,Noise]=resume(dua_g,Noise);
endfunction
