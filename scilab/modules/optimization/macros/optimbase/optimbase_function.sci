// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
//     if index > 0, everything went fine
//     if index == 0, interrupts the optimization
//     if index < 0, f cannot be evaluated
// Syntaxes:
//   [ this , f , index ] = optimbase_function ( this , x , index )
//   [ this , f , g , index ] = optimbase_function ( this , x , index )
//   [ this , f , c , index ] = optimbase_function ( this , x , index )
//   [ this , f , g , c , gc , index ] = optimbase_function ( this , x , index )
// Not Authorized syntaxes of the optimbase_function:
//   Rejected because there is no index in input :
//   [ this , f , index ] = optimbase_function ( this , x )
//   [ this , f ] = optimbase_function ( this , x )
//   Rejected because there is no index in output :
//   [ this , f ] = optimbase_function ( this , x , index )
//   [ this , f , g ] = optimbase_function ( this , x , index )
//   [ this , f , c ] = optimbase_function ( this , x , index )
//   [ this , f , g , c , gc ] = optimbase_function ( this , x , index )
//
// Syntaxes of the cost function:
//   > Without additional data
//   [ f , index ] = costf ( x , index )
//   [ f , g , index ] = costf ( x , index )
//   [ f , c , index ] = costf ( x , index )
//   [ f , g , c , gc , index ] = costf ( x , index )
//   > With additional data
//   [ f , index ] = costf ( x , index , a1, a2, ... )
//   [ f , g , index ] = costf ( x , index , a1, a2, ... )
//   [ f , c , index ] = costf ( x , index , a1, a2, ... )
//   [ f , g , c , gc , index ] = costf ( x , index , a1, a2, ... )
// Not Authorized syntaxes of the cost function:
//   Rejected because there is no index in input :
//   [ f ] = costf ( this , x )
//   [ f , index ] = costf ( this , x )
//   [ f ] = costf ( this , x , a1, a2, ... )
//   [ f , index ] = costf ( this , x , a1, a2, ... )
//   Rejected because there is no index in output :
//   [ f ] = costf ( this , x , index )
//   [ f , g ] = costf ( this , x , index )
//   [ f , c ] = costf ( this , x , index )
//   [ f , g , c ] = costf ( this , x , index )
//   [ f ] = costf ( this , x , index , a1, a2, ... )
//   [ f , g ] = costf ( this , x , index , a1, a2, ... )
//   [ f , g , c ] = costf ( this , x , index , a1, a2, ... )
//   [ f , c ] = costf ( this , x , index , a1, a2, ... )
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
        msg = sprintf ( "Function Evaluation #%d, index=%d, x= [%s]" , ...
        this.funevals , index, strcat(string(x)," "))
        this = optimbase_log ( this , msg )
    end
    //
    // Setup the callback and its arguments
    //
    funtype = typeof(this.fun)
    if ( funtype == "function" ) then
        __optimbase_f__ = this.fun
        //
        // Backward-compatibility: process the costfargument field
        //
        if ( typeof(this.costfargument) == "string" ) then
            __optimbase_args__ = list()
        else
            __optimbase_args__ = list(this.costfargument)
        end
    else
        __optimbase_f__ = this.fun(1)
        __optimbase_args__ = list(this.fun(2:$))
    end
    if ( this.withderivatives ) then
        if ( this.nbineqconst == 0 ) then
            // [ f , g , index ] = costf ( x , index )
            // [ f , g , index ] = costf ( x , index , a1, a2, ... )
            // [ this , f , g , index ] = optimbase_function ( this , x , index )
            [ f , g , index ] = __optimbase_f__ ( x , index , __optimbase_args__(1:$) );
            if type(f) <> 1 | ~isreal(f) then
                errmsg = msprintf(_("%s: Wrong type for user function output argument #%d: A real matrix expected."), "optimbase_function", 1);
                error(errmsg)
            end
            if type(g) <> 1 | ~isreal(g) then
                errmsg = msprintf(_("%s: Wrong type for user function output argument #%d: A real matrix expected."), "optimbase_function", 2);
                error(errmsg)
            end
            if type(index) <> 1 | ~isreal(index) then
                errmsg = msprintf(_("%s: Wrong type for user function output argument #%d: A real matrix expected."), "optimbase_function", 3);
                error(errmsg)
            end
            varargout(1) = this
            varargout(2) = f
            varargout(3) = g
            varargout(4) = index
        else
            // [ f , g , c , gc , index ] = costf ( x , index )
            // [ f , g , c , gc , index ] = costf ( x , index , a1, a2, ... )
            // [ this , f , g , c , gc , index ] = optimbase_function ( this , x , index )
            [ f , g , c , gc , index ] = __optimbase_f__ ( x , index , __optimbase_args__(1:$) );
            if type(f) <> 1 | ~isreal(f) then
                errmsg = msprintf(_("%s: Wrong type for user function output argument #%d: A real matrix expected."), "optimbase_function", 1);
                error(errmsg)
            end
            if type(g) <> 1 | ~isreal(g) then
                errmsg = msprintf(_("%s: Wrong type for user function output argument #%d: A real matrix expected."), "optimbase_function", 2);
                error(errmsg)
            end
            if type(c) <> 1 | ~isreal(c) then
                errmsg = msprintf(_("%s: Wrong type for user function output argument #%d: A real matrix expected."), "optimbase_function", 3);
                error(errmsg)
            end
            if type(gc) <> 1 | ~isreal(gc) then
                errmsg = msprintf(_("%s: Wrong type for user function output argument #%d: A real matrix expected."), "optimbase_function", 4);
                error(errmsg)
            end
            if type(index) <> 1 | ~isreal(index) then
                errmsg = msprintf(_("%s: Wrong type for user function output argument #%d: A real matrix expected."), "optimbase_function", 5);
                error(errmsg)
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
            // [ f , index ] = costf ( x , index , a1, a2, ... )
            // [ this , f , index ] = optimbase_function ( this , x , index )
            [ f , index ] = __optimbase_f__ ( x , index , __optimbase_args__(1:$) );
            if type(f) <> 1 | ~isreal(f) then
                errmsg = msprintf(_("%s: Wrong type for user function output argument #%d: A real matrix expected."), "optimbase_function", 1);
                error(errmsg)
            end
            if type(index) <> 1 | ~isreal(index) then
                errmsg = msprintf(_("%s: Wrong type for user function output argument #%d: A real matrix expected."), "optimbase_function", 2);
                error(errmsg)
            end
            varargout(1) = this
            varargout(2) = f
            varargout(3) = index
        else
            // [ f , c , index ] = costf ( x , index )
            // [ f , c , index ] = costf ( x , index , a1, a2, ... )
            // [ this , f , c , index ] = optimbase_function ( this , x , index )
            [ f , c , index ] = __optimbase_f__ ( x , index , __optimbase_args__(1:$) );
            if type(f) <> 1 | ~isreal(f) then
                errmsg = msprintf(_("%s: Wrong type for user function output argument #%d: A real matrix expected."), "optimbase_function", 1);
                error(errmsg)
            end
            if type(c) <> 1 | ~isreal(c) then
                errmsg = msprintf(_("%s: Wrong type for user function output argument #%d: A real matrix expected."), "optimbase_function", 2);
                error(errmsg)
            end
            if type(index) <> 1 | ~isreal(index) then
                errmsg = msprintf(_("%s: Wrong type for user function output argument #%d: A real matrix expected."), "optimbase_function", 3);
                error(errmsg)
            end
            varargout(1) = this
            varargout(2) = f
            varargout(3) = c
            varargout(4) = index
        end
    end
endfunction

