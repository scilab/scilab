// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 2007 - Jean-Baptiste SILVY
// Copyright (C) DIGITEO - 2011-2012 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function resMat = resize_matrix(mat, nbRow, nbCol, typeOfMat)

  // Author Jean-Baptiste Silvy
  // Create a matrix of size nbRow x nbCol whose elements (i,j) are mat(i,j)
  // if (i,j) is in the range of the matrix. Otherwise elements (i,j) are 0 for real
  // or integer matrix, %f for boolean matrix and an empty string for string matrix.
  // The type of the matrix may be modified by specifying the typeOfMat variable

  [lhs, rhs] = argn(0);
  if rhs <> 3 & rhs <> 4 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected.\n"),"resize_matrix", 3, 4));
  end

  if ((nbRow <= 0) | (nbCol <= 0)) then
    resMat = [];
    return
  end

  matSize = size(mat) ;
  minRow = min(nbRow, matSize(1));
  minCol = min(nbCol, matSize(2));

  [lhs,rhs] = argn(0) ;
  if (rhs < 4) then
    typeOfMat = typeof(mat) ;
  end

  z = zeros(nbRow, nbCol);

  select typeOfMat
  case 'constant' then
    // real matrix
    resMat = z;
  case 'boolean' then
    // boolean matrix
    // intialized it to false
    resMat = (z == 1);
    resMat(1:minRow, 1:minCol) = (mat(1:minRow, 1:minCol) == 1);
    return
  case 'int8' then
    mat = int8(mat);
    resMat = int8(z);
  case 'int16' then
    mat = int16(mat);
    resMat = int16(z);
  case 'int32' then
    mat = int32(mat);
    resMat = int32(z);
  case 'uint8' then
    mat = uint8(mat);
    resMat = uint8(z);
  case 'uint16' then
    mat = uint16(mat);
    resMat = uint16(z);
  case 'uint32' then
    mat = uint32(mat);
    resMat = uint32(z);
  case 'string' then
    // string matrix
    mat = string(mat);
    resMat = emptystr(nbRow, nbCol);
  else
    error(msprintf(gettext("%s: Wrong value for input argument #%d.\n"), "resize_matrix", 1));
  end

  resMat(1:minRow, 1:minCol) = mat(1:minRow, 1:minCol);

endfunction
