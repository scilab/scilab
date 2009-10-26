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
  if this.nbve<> 0 & nx1 <> this.nbve then
    error(msprintf(gettext("%s: Number of rows of x is %d is different from number of vertices = %d."),"optimsimplex_check" , nx1 , this.nbve ));
  end
  if this.n<> 0 & nx2 <> this.n then
    error(msprintf(gettext("%s: Number of columns of x is %d is different from dimension = %d."),"optimsimplex_check" , nx2 , this.n ));
  end
  nf1 = size(this.fv,1)
  nf2 = size(this.fv,2)
  if this.n<> 0 & nf1 <> this.nbve then
    error(msprintf(gettext("%s: Number of rows of fv is %d is different from number of vertices = %d."),"optimsimplex_check" , nf1 , this.nbve ));
  end
  if this.nbve<> 0 & nf2 <> 1 then
    error(msprintf(gettext("%s: Number of columns of fv is %d is different from 1."),"optimsimplex_check" , nf2 ));
  end
endfunction

