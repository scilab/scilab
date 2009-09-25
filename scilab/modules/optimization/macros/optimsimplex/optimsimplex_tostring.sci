// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// optimsimplex_tostring --
//   Returns the current simplex as a string.
// Arguments
//   <no arg>
//
function str = optimsimplex_tostring ( this )
  if this.n == 0 then
    str = sprintf("Empty simplex (zero dimension)\n");
  elseif this.nbve == 0 then
    str = sprintf("Empty simplex (zero vertices)\n");
  elseif this.x == [] then
    str = sprintf("Empty simplex (zero coordinates)\n");
  elseif this.fv == [] then
    str = sprintf("Empty simplex (zero function values)\n");
  else
  str = []
  for k = 1:this.nbve
    // Compute a string for x
    ss = sprintf("%e", this.x(k,1));
    for i = 2:this.n
      ss = ss + " " + sprintf("%e", this.x(k,i));
    end
    str(k) = sprintf("Vertex #%d/%d : fv=%e, x=%s\n" , k , this.nbve , this.fv(k), ss );
  end
  end
endfunction
