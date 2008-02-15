// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=not_empty(A)
// M2SCI function
// Checks if no A dimension can be 0
// Input: A = a M2SCI tlist
// Output: r = boolean value (true if A can not be an empty matrix)

n=size(A.dims)
nb=0
for k=1:n
  if A.dims(k)>0 | A.dims(k)==SupToOne then
    nb=nb+1
  end
end

r=nb>0
endfunction
