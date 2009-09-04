// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimbase_function --
//   Call the cost function and return the value.
//   If a cost function argument is defined in current object, 
//   pass it to the function.
//   If an index is defined as input argument, pass it to the 
//   function, always as second argument.
// Arguments
//   x : the point where the function is to be evaluated.
//   index : a flag to pass to the cost function (default = 1)
//   result : the result of the cost function
//     This result may be the value of the cost function, the 
//     values are the inequality constraints, the values of the 
//     gradient of f or of the constraints, etc...
// Note
//  The following protocol is used
//  * if index=1, or no index, returns the value of the cost 
//    function (default case)
//  * if index=2, returns the value of the nonlinear inequality 
//    constraints, as a row array
//  * if index=3, returns an array which contains
//    at index #0, the value of the cost function  
//    at index #1 to the end is the list of the values of the nonlinear 
//    constraints
//  The inequality constraints are expected to be positive.
//
function [ this , result ] = optimbase_function ( this , x , index )
  if this.fun == "" then
    errmsg = sprintf("Empty function (use -fun option).")
    error(errmsg)
  end
  if typeof(this.costfargument)=="string" then
    if (~isdef('index','local')) then
      result = this.fun ( x );
    else
      result = this.fun ( x , index );
    end
  else
    if (~isdef('index','local')) then
      result = this.fun ( x , this.costfargument );
    else
      result = this.fun ( x , index , this.costfargument );
    end
  end
  this.funevals = this.funevals + 1;
  optimbase_log ( this , sprintf ( "Function Evaluation #%d is [%s] at [%s]" , ...
    this.funevals , strcat(string(result)," ") , strcat(string(x)," ") ))
endfunction

