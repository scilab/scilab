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
//   optimbase_function, so that it matches
//   the requirements of simplex methods.
//
function [ f , this ] = neldermead_costf ( x , this )
  [ this.optbase , hascons ] = optimbase_hasnlcons ( this.optbase );
  if ( hascons ) then
    [ this.optbase , f , c , index ] = optimbase_function ( this.optbase , x , 2 );
  else
    [ this.optbase , f , index ] = optimbase_function ( this.optbase , x , 2 );
  end
endfunction

