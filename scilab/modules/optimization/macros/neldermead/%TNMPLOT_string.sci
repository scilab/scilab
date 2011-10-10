// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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



