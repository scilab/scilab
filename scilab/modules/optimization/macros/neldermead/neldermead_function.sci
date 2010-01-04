// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// neldermead_function --
//   Call the cost function and return the value of f.
//   This is a simple way to get the value of the cost function
//   from outside.
// Arguments
//   x : the point where the function is to be evaluated.
//   index : a flag to pass to the cost function (default = 1)
//   f : the cost function
//
function [ this , f ] = neldermead_function ( this , x )
  index = 2;
  [ this.optbase , hasnlcons ] = optimbase_hasnlcons ( this.optbase );
  if ( hasnlcons ) then
    [ this.optbase , f , c , index ] = optimbase_function ( this.optbase , x , index );
  else
    [ this.optbase , f , index ] = optimbase_function ( this.optbase , x , index );
  end
  // Ignores the value of index at output.
endfunction

