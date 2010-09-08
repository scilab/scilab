// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x=mtlb_full(y)
// Emulation function for full() Matlab function

if type(y)==1 then 
  x=full(y)
elseif type(y)==10 then 
  tmp = asciimat(y)
  tmp=full(tmp)
  x=[];
  for k=1:size(tmp,1)
    x=[x;ascii(tmp(k,:))]
  end
elseif type(y)==4 then 
  x=full(bool2s(y))
end
endfunction


