// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2007 - INRIA - Eric Dubois
// Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function matin = squeeze(matin)

// PURPOSE: Remove singleton dimensions, that is any dimension 
// for which the size of the input hypermatrix is 1; if the 
// input is a matrix, it is unaffected
// ------------------------------------------------------------
// INPUT:
// * matin = a hypermatrix or a matrix of constant type
// ------------------------------------------------------------
// OUTPUT: 
// * matout = a hypermatrix or a matrix of constant type
// ------------------------------------------------------------



typein=typeof(matin)

select typein
case 'hypermat' then
  newDims = matin.dims( find( matin.dims <> 1 ) ) ;
  if newDims == [] then
    matin.dims = 1 ;
  else
    matin.dims = newDims ;
  end
case 'constant'
  // it is a standard matrix nothing to do
else 
  error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),'squeeze',1))
end

endfunction
