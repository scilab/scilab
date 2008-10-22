// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function r=%h_e(i,h)
h=h
  if type(i)==10 then
    r=get(h,i)
  elseif type(i)==15 then
    n=lstsize(i)
    
    for k=1:n
      p=i(k)
      if type(p)==10 then
	if type(h)==9 then
	  h=get(h,p),
	else
	  h=h(p)
	end
      elseif or(type(p)==[1 2 4 8 129]) then
	h=h(p)
      elseif type(p)==15 then
	h=h(p(:))
      else
	error('Invalid path')
      end
    end
    r=h
  elseif type(i)==1 then
    r=h(i)
  else
    error('Invalid path')
  end
  if type(r)==10 then r=stripblanks(r),end
endfunction
