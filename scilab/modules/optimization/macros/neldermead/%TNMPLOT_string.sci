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
// %TNMPLOT_string --
//   Returns the string containing the Nelder-Mead plot component.
//
function str = %TNMPLOT_string ( this )
    str = []
    k = 1
    str(k) = sprintf("Nelder-Mead Plot Object:\n")
    k = k + 1
    str(k) = sprintf("======================")
    k = k + 1
    str(k) = ""
    k = k + 1
    str(k) = sprintf("Simplex data file : %s\n", this.simplexfn);
    k = k + 1
    str(k) = sprintf("Fbar data file : %s\n", this.fbarfn);
    k = k + 1
    str(k) = sprintf("Fopt data file : %s\n", this.foptfn);
    k = k + 1
    str(k) = sprintf("Sigma data file : %s\n", this.sigmafn);
    /////////////////////////////////////////////////////
    // Nelder Mead Object
    k = k + 1
    str(k) = "";
    k = k + 1
    str(k) = sprintf("nmbase: <%s Object>\n", typeof(this.nmbase));
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



