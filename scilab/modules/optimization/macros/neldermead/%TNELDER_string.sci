// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
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
// %TNELDER_string --
//   Returns the string containing the Nelder-Mead component.
//
function str = %TNELDER_string ( this )
    str = []
    k = 1
    str(k) = sprintf("Nelder-Mead Object:\n")
    k = k + 1
    str(k) = sprintf("======================")
    k = k + 1
    str(k) = ""
    k = k + 1
    str(k) = sprintf("Simplex0 Method : %s\n", string(this.simplex0method));
    k = k + 1
    str(k) = sprintf("Simplex0 Length : %s\n", string(this.simplex0length));
    k = k + 1
    str(k) = sprintf("Simplex0, Pfeffer, Delta Usual : %s\n", string(this.simplex0deltausual));
    k = k + 1
    str(k) = sprintf("Simplex0, Pfeffer, Delta Zero : %s\n", string(this.simplex0deltazero));
    k = k + 1
    str(k) = sprintf("Simplex0, Given, Coords :\n");
    if ( this.coords0 == [] ) then
        k = k + 1
        str(k) = sprintf("[]\n");
    else
        nbve = size(this.coords0,"r")
        for i = 1 : nbve
            k = k + 1
            str(k) = sprintf("%s\n" , _tostring(this.coords0(i,:)));
        end
    end
    /////////////////////////////////////////////////////
    // Termination
    k = k + 1
    str(k) = sprintf("\n");
    k = k + 1
    str(k) = sprintf("Termination parameters\n");
    k = k + 1
    str(k) = sprintf("Termination on simplex size : %s\n", string(this.tolsimplexizemethod));
    k = k + 1
    str(k) = sprintf("Termination on simplex size, Absolute Tolerance : %s\n", string(this.tolsimplexizeabsolute));
    k = k + 1
    str(k) = sprintf("Termination on simplex size, Relative Tolerance : %s\n", string(this.tolsimplexizerelative));
    k = k + 1
    str(k) = sprintf("Termination on simplex size, Initial Simplex Size : %s\n", string(this.simplexsize0));
    k = k + 1
    str(k) = sprintf("Termination on simplex size + Delta of function value : %s\n", string(this.tolssizedeltafvmethod));
    k = k + 1
    str(k) = sprintf("Termination on simplex size + Delta of function value, Absolute Tolerance on Delta F : %s\n", string(this.toldeltafv));
    k = k + 1
    str(k) = sprintf("Termination on Kelley''s Stagnation : %s\n", string(this.kelleystagnationflag));
    k = k + 1
    str(k) = sprintf("Termination on Kelley''s Stagnation, Normalization : %s\n", string(this.kelleynormalizationflag));
    k = k + 1
    str(k) = sprintf("Termination on Kelley''s Stagnation, Alpha0 : %s\n", string(this.kelleystagnationalpha0));
    k = k + 1
    str(k) = sprintf("Termination on Kelley''s Stagnation, Alpha : %s\n", string(this.kelleyalpha));
    k = k + 1
    str(k) = sprintf("Termination by Box : %s\n", string(this.boxtermination));
    k = k + 1
    str(k) = sprintf("Termination by Box, Absolute Tolerance on Function: %s\n", string(this.boxtolf));
    k = k + 1
    str(k) = sprintf("Termination by Box, Maximum Number of Consecutive Match : %s\n", string(this.boxnbmatch));
    k = k + 1
    str(k) = sprintf("Termination by Box, Current Number of Consecutive Match : %s\n", string(this.boxkount));
    k = k + 1
    str(k) = sprintf("Termination on Variance : %s\n", string(this.tolvarianceflag));
    k = k + 1
    str(k) = sprintf("Termination on Variance, Absolute Tolerance : %s\n", string(this.tolabsolutevariance));
    k = k + 1
    str(k) = sprintf("Termination on Variance, Relative Tolerance : %s\n", string(this.tolrelativevariance));
    k = k + 1
    str(k) = sprintf("Termination on Variance, Variance of Initial Simplex : %s\n", string(this.variancesimplex0));
    /////////////////////////////////////////////////////
    // Algorithm parameters
    k = k + 1
    str(k) = sprintf("\n");
    k = k + 1
    str(k) = sprintf("Algorithms parameters\n");
    k = k + 1
    str(k) = sprintf("Method : %s\n", string(this.method));
    k = k + 1
    str(k) = sprintf("Reflection Factor (rho) : %s\n", string(this.rho));
    k = k + 1
    str(k) = sprintf("Expansion Factor (chi) : %s\n", string(this.chi));
    k = k + 1
    str(k) = sprintf("Contraction Factor (gamma) : %s\n", string(this.gamma));
    k = k + 1
    str(k) = sprintf("Shrinkage Factor (sigma) : %s\n", string(this.sigma));
    k = k + 1
    str(k) = sprintf("Kelley Stagnation : %s\n", string(this.kelleystagnationflag));
    k = k + 1
    str(k) = sprintf("Restart Epsilon : %s\n", string(this.restarteps));
    k = k + 1
    str(k) = sprintf("Restart Step : %s\n", _strvec(this.restartstep));
    k = k + 1
    str(k) = sprintf("Restart Maximum : %d\n", this.restartmax);
    k = k + 1
    str(k) = sprintf("Restart Simplex Method : %s\n", string(this.restartsimplexmethod));
    k = k + 1
    str(k) = sprintf("Restart Flag : %s\n", string(this.restartflag));
    k = k + 1
    str(k) = sprintf("Restart Number : %s\n", string(this.restartnb));
    k = k + 1
    str(k) = sprintf("Restart Detection Method : %s\n", string(this.restartdetection));
    k = k + 1
    str(k) = sprintf("Startup Flag : %s\n", string(this.startupflag));
    k = k + 1
    str(k) = sprintf("Automatic Checking of Cost Function : %s\n", string(this.checkcostfunction));
    k = k + 1
    str(k) = sprintf("Box, Number of Points  : %s\n", string(this.boxnbpoints));
    k = k + 1
    str(k) = sprintf("Box, Current Number of Points  : %s\n", string(this.boxnbpointseff));
    k = k + 1
    str(k) = sprintf("Box, Scaling, Factor  : %s\n", string(this.boxineqscaling));
    k = k + 1
    str(k) = sprintf("Box, Scaling, Method  : %s\n", string(this.scalingsimplex0));
    k = k + 1
    str(k) = sprintf("Box, Scaling, Minimum : %s\n", string(this.guinalphamin));
    k = k + 1
    str(k) = sprintf("Box, Bounds Parameter: %s\n", string(this.boxboundsalpha));
    k = k + 1
    str(k) = sprintf("Box, Reflection Coefficient : %s\n", string(this.boxreflect));
    /////////////////////////////////////////////////////
    // Sub-objects
    k = k + 1
    str(k) = "";
    k = k + 1
    str(k) = sprintf("optbase: <%s Object>\n", typeof(this.optbase));
    k = k + 1
    str(k) = sprintf("simplex0: <%s Object>\n", typeof(this.simplex0));
    k = k + 1
    str(k) = sprintf("simplexopt: <%s Object>\n", typeof(this.simplexopt));
endfunction

//
// _strvec --
//  Returns a string for the given vector.
//
function str = _strvec ( x )
    str = strcat(string(x)," ")
endfunction
function s = _tostring ( x )
    if ( x==[] ) then
        s = "[]"
    else
        n = size ( x , "*" )
        if ( n == 1 ) then
            s = string(x)
        else
            s = "["+strcat(string(x)," ")+"]"
        end
    end
endfunction



