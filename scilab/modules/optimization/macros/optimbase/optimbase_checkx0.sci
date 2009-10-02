// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimbase_checkx0 --
//   Returns %T if the initial guess is consistent with the bounds
//   and the non linear inequality constraints, if any.
// Arguments
//   flag : %T or %F
//
function [ this , isok ] = optimbase_checkx0 ( this )
    this = optimbase_log ( this , sprintf ( "Checking initial guess..." ) )
    [ this , isfeasible ] = optimbase_isfeasible ( this , this.x0 )
    isok = ( isfeasible == 1 )
    if ( isok ) then
      this = optimbase_log ( this , sprintf ( "... initial guess is feasible." ) )
    else
      this = optimbase_log ( this , sprintf ( "... initial guess is not feasible." ) )
    end
endfunction

