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

win=xget("window");

  
if rhs == 1
  a=gca();
else
  if (type(handl) == 9)
    if (handl.type == 'Axes')
      a = handl;
    else
      error("Error : handle must be of type Axes");
    end
  else
    error("Error : Incorrect input, second argument must be of type graphic Axes handle");
  end
end
a.data_bounds = [rect(1) rect(2) ; rect(3) rect(4)]
endfunction
