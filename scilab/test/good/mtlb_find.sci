// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [i,j,v]=mtlb_find(b)
// Translation function for find() Matlab function

[lhs,rhs]=argn(0)

// Because find does not work on complex values
// And isreal does not word with Booleans
c=b
if type(b)<>4 & type(b)<>6 then
  if ~isreal(b) then
    c=abs(b)
  end
end

// One output arg
if lhs==1 then
  i=find(c)
  if size(b,1)<>1 then 
    i=i'
  end
// Two output args
elseif lhs==2 then
  [i,j]=find(c)
  if size(b,1)<>1 then 
    i=i'
    j=j'
  end
// Three output args
else
  [i,j]=find(c)
  if i<>[] then 
    v = b(i+size(b,1)*(j-1))
  else 
    v = []
  end
  if size(b,1)<>1 then 
    i=i'
    j=j'
  end
end
endfunction
