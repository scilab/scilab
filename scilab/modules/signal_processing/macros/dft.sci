// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function xf=dft(x,flag);
//xf=dft(x,flag);
//macro which computes dft of vector x
// x    :input vector
// flag :indicates dft or idft
// xf   :output vector
//!
warnobsolete('fft', '5.4.2');
n=max(size(x));
arg=(0:n-1);
am=-2*%pi*%i*arg'*arg/n;
if flag==1 then,
  am=-am;
end,
xf=exp(am)*matrix(x,n,1);
if flag==1 then,
  xf=xf/n;
end
endfunction
