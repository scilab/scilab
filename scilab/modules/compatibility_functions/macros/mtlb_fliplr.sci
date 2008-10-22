// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x=mtlb_fliplr(y)
// Emulation function for fliplr() Matlab function

if type(y)==1 then 
  x=y(:,$:-1:1)
elseif type(y)==10 then 
  tmp = asciimat(y)
  tmp=tmp(:,$:-1:1)
  x=[];
  for k=1:size(tmp,1)
    x=[x;ascii(tmp(k,:))]
  end
elseif type(y)==4 then 
  tmp = bool2s(y) 
  x=tmp(:,$:-1:1)
end
endfunction


