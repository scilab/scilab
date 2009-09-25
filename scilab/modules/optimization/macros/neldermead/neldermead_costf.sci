// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// neldermead_costf --
//   Call the cost function and return the value.
// Note
//   This function is given to the simplex class as 
//   a callback.
//   Input/Output arguments are swapped w.r.t. 
//   neldermead_function, so that it matches
//   the requirements of simplex methods.
//
function [ f , this ] = neldermead_costf ( x , this )
  nbnlc = optimbase_cget ( this.optbase , "-nbineqconst" )
  if ( nbnlc == 0 ) then
    [this.optbase , f] = optimbase_function ( this.optbase , x );
  else
    // Set the index, so that, if an additionnal cost function argument is provided,
    // it can be appended at the end.
    index = 1;
    [this.optbase , f] = optimbase_function ( this.optbase , x , index );
  end
endfunction

