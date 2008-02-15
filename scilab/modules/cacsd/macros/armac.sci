// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function [ar]=armac(a,b,d,ny,nu,sig)
// just build a tlist for storing armacx coefficients 
//   A(z^-1)y= B(z^-1)u + D(z^-1)sig*e(t)
//   a=<Id,a1,..,a_r>; matrix (ny,r*ny)
//   b=<b0,.....,b_s>; matrix (ny,(s+1)*nu)
//   d=<Id,d1,..,d_p>; matrix (ny,p*ny);
//   ny : dim of observation y 
//   nu : dim of control  u
//   sig : standard deviation  (ny,ny);
//
//!

[na,la]=size(a);
if na<>ny then 
	write(%io(2),"armac: a(:,1) must be of dimension "+string(ny));
	return;
end
[nb,lb]=size(b);
if nb<>0 & nb<>ny then 
 	write(%io(2),"armac: b(:,1) must be of dimension "+string(ny));
	return;
end;
if lb<>0 & nu<>0 then 
 if modulo(lb,nu)<>0 then 
 	write(%io(2),"armac: number of columns of b are incompatible with nu');
	return;
  end;
end      
[nd,ld]=size(d);
if nd<>ny then 
	write(%io(2),"armac: d(:,1) must be of dimension "+string(ny));
	return;
end
ar=tlist(['ar','a','b','d','ny','nu','sig'],a,b,d,ny,nu,sig);
endfunction
