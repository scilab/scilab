function noisegen(pas,Tmax,sig)
// This function returns in the calling env 
// a macro  [b]=Noise(t)
// Noize(t) is a piecewise constant function [k*pas,(k+1)*pas]
//The value on each constant interval are random values from  
// i.i.d  Gaussian variables of standard deviation sig. 
// The function is constant for t<=0 and t>=Tmax.
//!
// Copyright INRIA
dua_g=sig*rand(0:pas:Tmax,'n');
[nn1,nn2]=size(dua_g);
deff('[b]=Noise(t)','b=dua_g(mini(maxi((t/'+string(Tmax)+...
   ')*'+string(nn2)+',1),'+string(nn2)+'))');
[dua_g,Noise]=resume(dua_g,Noise);
endfunction
