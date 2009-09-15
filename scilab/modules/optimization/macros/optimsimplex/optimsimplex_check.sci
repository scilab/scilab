// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// optimsimplex_check --
//   Check the consistency of the internal data.
//   Generates an error if necessary.
// Arguments
//   <no arg>
//
function optimsimplex_check ( this )
  nx1 = size(this.x,1)
  nx2 = size(this.x,2)
  nf1 = size(this.fv,1)
  nf2 = size(this.fv,2)
  if this.n<> 0 & nx1 <> this.n then
    error(sprintf("Dimension #1 of x is %d is different from dimension = %d\n" , nx1 , this.n ));
  end
  if this.nbve<> 0 & nx2 <> this.nbve then
    error(sprintf("Dimension #2 of x is %d is different from dimension + 1 = %d\n" , nx2 , this.n + 1));
  end
  if this.nbve<> 0 & nf1 <> this.nbve then
    error(sprintf("Dimension #1 of fv is %d is different from dimension = %d\n" , nf1 , this.n + 1));
  end
  if this.n<> 0 & nf2 <> 1 then
    error(sprintf("Dimension #2 of fv is %d is different from 1\n" , nf2 ));
  end
endfunction

