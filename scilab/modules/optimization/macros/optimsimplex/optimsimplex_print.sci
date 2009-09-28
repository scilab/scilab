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
    str = optimsimplex_tostring ( this );
    for k = 1:this.nbve
      mprintf("%s\n" , str(k) );
    end
  end
endfunction

