// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007 - INRIA - Jean-Baptiste SILVY
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function resMat = resize_matrix(mat,nbRow,nbCol,typeOfMat)

// Author Jean-Baptiste Silvy
// Create a matrix of size nbRow x nbCol whose elements (i,j) are mat(i,j)
// if (i,j) is in the range of the matrix. Otherwise elements (i,j) are 0 for real
// or integer matrix, %f for boolean matrix and an empty string for string matrix.
// The type of the matrix may be modified by specifying the typeOfMat variable

if ( nbRow <= 0 | nbCol <= 0 ) then
  resMat = [] ;
  return ;
end

matSize = size(mat) ;
minRow = min( nbRow, matSize(1) ) ;
minCol = min( nbCol, matSize(2) ) ;

[lhs,rhs] = argn(0) ;
if ( rhs < 4 ) then
  typeOfMat = typeof(mat) ;
end

select typeOfMat
case 'constant' then
  // real matrix
  resMat = zeros( nbRow, nbCol ) ;
case 'boolean' then
  // boolean matrix
  // intialized it to false
  resMat = ( zeros( nbRow, nbCol ) == 1 ) ;
case 'int8' then
  resMat = int8( zeros( nbRow, nbCol ) ) ;
case 'int16' then
  resMat = int16( zeros( nbRow, nbCol ) ) ;
case 'int32' then
  resMat = int32( zeros( nbRow, nbCol ) ) ;
case 'uint8' then
  resMat = uint8( zeros( nbRow, nbCol ) ) ;
case 'uint16' then
  resMat = uint16( zeros( nbRow, nbCol ) ) ;
case 'uint32' then
  resMat = uint32( zeros( nbRow, nbCol ) ) ;
case 'string' then
  // string matrix
  resMat = emptystr( nbRow, nbCol ) ;
end

resMat(1:minRow,1:minCol) = mat(1:minRow,1:minCol) ;

endfunction
