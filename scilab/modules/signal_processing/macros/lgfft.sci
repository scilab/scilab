function [y]=lgfft(x)
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
[o,i]=argn(0);
   if i < 1 then error(58); end;
   if type(x) <> 1 then error(53,1); end;
//
   z=testvec(x,'l');
   nz=maxi(size(z));
   pnz=int(log(nz)/log(2));
   rz=nz-2**pnz;
   if rz == 0 then y=nz; else y=2**(pnz+1); end;
endfunction
