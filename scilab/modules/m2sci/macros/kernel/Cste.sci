// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function const=Cste(value)
// Create a new constant tlist

rhs=argn(2)

if rhs~=1 then
  error(gettext("Wrong number of inputs."));
end

dims=list()

if value=="" then
  dims=list(0,0)
else
  sz=size(value)
  for k=1:size(sz,"*")
    dims($+1)=sz(k)
    if type(value)==String then
      if k==2 then
	dims($)=sum(length(value))
      end
    end
  end
end

if type(value)==String then
  prop=Real
  value=value
else
  if type(value)==1 then
    if isreal(value) then
      prop=Real
    else
      prop=Complex
    end
  else
    prop=Real
  end
end

const=tlist(["cste","value","infer"],value,Infer(dims,Type(type(value),prop)))
endfunction
