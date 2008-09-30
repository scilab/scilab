// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function x = hilbert(xr)
// Marple, S.L., "Computing the discrete-time analytic signal via FFT,"
// IEEE Transactions on Signal Processing, Vol. 47, No.9 (September
// 1999), pp.2600-2603
// http://ieeexplore.ieee.org/iel5/78/16975/00782222.pdf?arnumber=782222  
    
  if  type(xr)<>1 then
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Array of floating point numbers expected.\n"),"hilbert",1))
  end

  n=size(xr,'*');
  
  if n==0 then x=[],return,end
  if ~isreal(xr,0) then 
    error(msprintf(gettext("%s: Input argument #%d must be real.\n"),'hilbert',1));
  end
  
  no2 = int(n/2);
  
  x = fft(real(xr),-1); 
  
  h  = zeros(xr); 
  
  if ((2*no2) == n) then  // n is even
    h([1,no2+1]) = 1;
    h(2:no2) = 2;
  else // n is odd
    h(1) = 1;
    h(2:(n+1)/2) = 2;
  end
  
  x = fft(x.*h,1);

endfunction

