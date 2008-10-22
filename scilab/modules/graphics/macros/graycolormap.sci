// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function cmap = graycolormap(n)
//graycmap   linear gray-scale color map.
//r=g=b

if size(n,'*')<>1|or(n<1) then
  error('graycolormap : n must be a positive integer')
end
cmap=((0:n-1)'/(n-1))*[1 1 1];
endfunction
