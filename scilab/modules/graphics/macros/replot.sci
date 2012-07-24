// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function replot(rect,handl)
// replots the graphic window win 
// using rect as x and y bounds 


[lhs,rhs]=argn(0);

if rhs < 1 then
  error(msprintf(gettext("%s: Wrong number of input argument(s): %d or %d expected.\n"), "replot", 1, 2));
end
  
if rhs == 1
  a=gca();
else
  if (type(handl) == 9)
    if (handl.type == 'Axes')
      a = handl;
    else
      error(msprintf(gettext("%s: Input argument #%d must be a handle on an axis.\n"), "replot", 2));
    end
  else
    error(msprintf(gettext("%s: Wrong type for input argument #%d: Graphic handle expected.\n"), "replot", 2));
  end
end
a.data_bounds = [rect(1) rect(2) ; rect(3) rect(4)]
endfunction
