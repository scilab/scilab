// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
function x=ifft(a)
  if type(a)==1 then 
    x=fft(a,1)
  elseif typeof(a)=='hypermat' then
    x=%hm_fft(a,1)
  end
endfunction
