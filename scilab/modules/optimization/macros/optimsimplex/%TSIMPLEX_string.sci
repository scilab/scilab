// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// %TSIMPLEX_string --
//   Returns the string containing the Optim Simplex component.
//
function str = %TSIMPLEX_string ( this )
  str = []
  k = 1
  str(k) = sprintf("Optim Simplex Object:\n")
  k = k + 1
  str(k) = sprintf("=====================")
  if this.n == 0 then
    k = k + 1
    str(k) = sprintf("Empty simplex (zero dimension)\n");
  elseif this.nbve == 0 then
    k = k + 1
    str(k) = sprintf("Empty simplex (zero vertices)\n");
  elseif this.x == [] then
    k = k + 1
    str(k) = sprintf("Empty simplex (zero coordinates)\n");
  elseif this.fv == [] then
    k = k + 1
    str(k) = sprintf("Empty simplex (zero function values)\n");
  else
    for ive = 1:this.nbve
      // Compute a string for x
      ss = sprintf("%e", this.x(ive,1));
      for i = 2:this.n
        ss = ss + " " + sprintf("%e", this.x(ive,i));
      end
      k = k + 1
      str(k) = sprintf("Vertex #%d/%d : fv=%e, x=%s\n" , ive , this.nbve , this.fv(ive), ss );
    end
  end
endfunction

