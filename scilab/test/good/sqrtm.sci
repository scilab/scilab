// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function x=sqrtm(a)
//   sqrtm - computes the matrix square root. 
//%CALLING SEQUENCE
//   x=sqrtm(a)
//%PARAMETERS
//   a   : square hermitian or diagonalizable matrix
//   x   : square  matrix
//!

x=%s_pow(a,0.5);
endfunction
