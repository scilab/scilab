// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
// Copyright (C) DIGITEO - 2010 - Allan CORENT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function _h = eval(z)
  // Syntax :  H = eval(Z)
  // returns the evaluation of the matrix of character strings Z.
  // Example: a=1; b=2; Z=['a','b'] ; eval(Z) returns the matrix [1,2];
  //!

  _h = [];
  [mz, nz] = size(z);
  if mz * nz > 1 then
    str = [];
    for l = 1:mz, for k = 1:nz,
        str = [str; "%_h(" + string(l) + ", " + string(k) + ")=" + z(l,k)];
      end
    end
  else
    str = "%_h = " + z;
  end
  %_h = [];
  deff("[%_h] = %eval();", str);
  _h = %eval();
endfunction
