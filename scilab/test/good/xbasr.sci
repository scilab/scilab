// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) 2007 - INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function []=xbasr(win_num)
// This function is used to redraw the content of the graphic 
// window win_num.
//!

[lhs,rhs]=argn(0);
if rhs==0,
  draw(gcf()) ;
else 
   draw(scf(win_num)) ;
end 
endfunction
