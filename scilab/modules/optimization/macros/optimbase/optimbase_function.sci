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
//   index, input : a flag to pass to the cost function
//      if index=1, output a message, and compute whatever is required
//      if index=2, compute f
//      if index=3, compute g
//      if index=4, compute f and g
//      if index=5, returns c
//      if index=6, returns f and c
//      if index=7, returns f, g, c and gc
//    where
//      f : the cost function
//      g : the gradient of the cost function
//      c : the nonlinear, positive constraints
//      gc : the gradient of the nonlinear, positive constraints
//    Other values of index might be used in the future,
//    for example, when an derivative-based optimizer with 
//    non linear constraints is required.
//   index, output : 
//     if index > 0, everything went find
//     if index == 0, interrupts the optimization
//     if index < 0, f cannot be evaluated
// Calling sequences :
//   [ this , f , index ] = optimbase_function ( this , x , index )
//   [ this , f , g , index ] = optimbase_function ( this , x , index )
//   [ this , f , c , index ] = optimbase_function ( this , x , index )
//   [ this , f , g , c , gc , index ] = optimbase_function ( this , x , index )
// Not Authorized Calling sequences of the optimbase_function:
//   Rejected because there is no index in input :
//   [ this , f , index ] = optimbase_function ( this , x )
//   [ this , f ] = optimbase_function ( this , x )
//   Rejected because there is no index in output :
//   [ this , f ] = optimbase_function ( this , x , index )
//   [ this , f , g ] = optimbase_function ( this , x , index )
//   [ this , f , c ] = optimbase_function ( this , x , index )
//   [ this , f , g , c , gc ] = optimbase_function ( this , x , index )
//
// Calling sequences of the cost function:
//   > Without additionnal data
//   [ f , index ] = costf ( x , index )
//   [ f , g , index ] = costf ( x , index )
//   [ f , c , index ] = costf ( x , index )
//   [ f , g , c , gc , index ] = costf ( x , index )
//   > With additionnal data
//   [ f , index , data ] = costf ( x , index , data )
//   [ f , g , index , data ] = costf ( x , index , data )
//   [ f , c , index , data ] = costf ( x , index , data )
//   [ f , g , c , gc , index , data ] = costf ( x , index , data )
// Not Authorized Calling sequences of the cost function:
//   Rejected because there is no index in input :
//   [ f ] = costf ( this , x )
//   [ f , index ] = costf ( this , x )
//   [ f , data ] = costf ( this , x , data )
//   [ f , index , data ] = costf ( this , x , data )
//   Rejected because there is no index in output :
//   [ f ] = costf ( this , x , index )
//   [ f , g ] = costf ( this , x , index )
//   [ f , c ] = costf ( this , x , index )
//   [ f , g , c ] = costf ( this , x , index )
//   [ f , data ] = costf ( this , x , index , data )
//   [ f , g , data ] = costf ( this , x , index , data )
//   [ f , g , c , data ] = costf ( this , x , index , data )
//   [ f , c , data ] = costf ( this , x , index , data )
//
function varargout = optimbase_function ( this , x , index )
  [lhs,rhs]=argn();
  if ( rhs <> 3 ) then
    errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while 2 are expected."), "optimbase_function", rhs);
    error(errmsg)
  end
  if ( ( lhs < 3 ) | ( lhs > 5 ) ) then
    errmsg = msprintf(gettext("%s: Unexpected number of output arguments : %d provided while 3 to 5 are expected."), "optimbase_function", lhs);
    error(errmsg)
  end
  if this.fun == "" then
    errmsg = msprintf(gettext("%s: Empty function (use -function option)."), "optimbase_function")
    error(errmsg)
  end
  this.funevals = this.funevals + 1;
  if this.verbose == 1 then
    msg = sprintf ( "Function Evaluation #%d at [%s]" , ...
      this.funevals , strcat(string(x)," ") )
    this = optimbase_log ( this , msg )
  end
    if ( this.withderivatives ) then 
      if ( this.nbineqconst == 0 ) then
        // [ f , g , index ] = costf ( x , index )
        // [ f , g , index , data ] = costf ( x , index , data )
        // [ this , f , g , index ] = optimbase_function ( this , x , index )
        if ( typeof(this.costfargument) == "string" ) then
          [ f , g , index ] = this.fun ( x , index );
        else
          [ f , g , index , this.costfargument ] = this.fun ( x , index , this.costfargument );
        end
        varargout(1) = this
        varargout(2) = f
        varargout(3) = g
        varargout(4) = index
      else
        // [ f , g , c , gc , index ] = costf ( x , index )
        // [ f , g , c , gc , index , data ] = costf ( x , index , data )
        // [ this , f , g , c , gc , index ] = optimbase_function ( this , x , index )
        if ( typeof(this.costfargument) == "string" ) then
          [ f , g , c , gc , index ] = this.fun ( x , index );
        else
          [ f , g , c , gc , index , this.costfargument ] = this.fun ( x , index , this.costfargument );
        end
        varargout(1) = this
        varargout(2) = f
        varargout(3) = g
        varargout(4) = c
        varargout(5) = gc
        varargout(6) = index
      end
    else
      if ( this.nbineqconst == 0 ) then
        // [ f , index ] = costf ( x , index )
        // [ f , index , data ] = costf ( x , index , data )
        // [ this , f , index ] = optimbase_function ( this , x , index )
        if ( typeof(this.costfargument) == "string" ) then
          [ f , index ] = this.fun ( x , index );
        else
          [ f , index , this.costfargument ] = this.fun ( x , index , this.costfargument );
        end
        varargout(1) = this
        varargout(2) = f
        varargout(3) = index
      else
        // [ f , c , index ] = costf ( x , index )
        // [ f , c , index , data ] = costf ( x , index , data )
        // [ this , f , c , index ] = optimbase_function ( this , x , index )
        if ( typeof(this.costfargument) == "string" ) then
          [ f , c , index ] = this.fun ( x , index );
        else
          [ f , c , index , this.costfargument ] = this.fun ( x , index , this.costfargument );
        end
        varargout(1) = this
        varargout(2) = f
        varargout(3) = c
        varargout(4) = index
      end
    end
endfunction

