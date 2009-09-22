// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// optimsimplex_print --
//   Display the current simplex, with coordinates and function values.
// Arguments
//   <no arg>
//
function optimsimplex_print ( this )
  if this.n == 0 then
    mprintf("Empty simplex (zero dimension)\n");
  elseif this.nbve == 0 then
    mprintf("Empty simplex (zero vertices)\n");
  elseif this.x == [] then
    mprintf("Empty simplex (zero coordinates)\n");
  elseif this.fv == [] then
    mprintf("Empty simplex (zero function values)\n");
  else
  mprintf("Dimension : %d\n" , this.n );
  mprintf("Number of vertices : %d\n" , this.nbve );
  for k = 1:this.nbve
    // Compute a string for x
    ss = sprintf("%f", this.x(k,1));
    for i = 2:this.n
      ss = ss + " " + sprintf("%f", this.x(k,i));
    end
    mprintf("Vertex #%d/%d : fv=%f, x=%s\n" , k , this.n+1 , this.fv(k), ss );
  end
  end
endfunction

