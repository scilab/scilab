// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//
// %mps_string --
//   Returns the string containing the MPS component.
//
function str = %mps_string ( this )
    str = []
    k = 1
    str(k) = sprintf("MPS Object:\n")
    k = k + 1
    str(k) = sprintf("===========")
    k = k + 1
    str(k) = ""
    k = k + 1
    str(k) = sprintf("irobj: %s\n", _tostring(this.irobj));
    k = k + 1
    str(k) = sprintf("namec: %s\n", _tostring(this.namec));
    k = k + 1
    str(k) = sprintf("nameb: %s\n", _tostring(this.nameb));
    k = k + 1
    str(k) = sprintf("namran: %s\n", _tostring(this.namran));
    k = k + 1
    str(k) = sprintf("nambnd: %s\n", _tostring(this.nambnd));
    k = k + 1
    str(k) = sprintf("name: %s\n", _tostring(this.name));
    k = k + 1
    str(k) = sprintf("rownames: %s\n", _tostring(this.rownames));
    k = k + 1
    str(k) = sprintf("colnames: %s\n", _tostring(this.colnames));
    k = k + 1
    str(k) = sprintf("rowstat: %s\n", _tostring(this.rowstat));
    k = k + 1
    str(k) = sprintf("rowcode: %s\n", _tostring(this.rowcode));
    k = k + 1
    str(k) = sprintf("colcode: %s\n", _tostring(this.colcode));
    k = k + 1
    str(k) = sprintf("rownmbs: %s\n", _tostring(this.rownmbs));
    k = k + 1
    str(k) = sprintf("colpnts: %s\n", _tostring(this.colpnts));
    k = k + 1
    str(k) = sprintf("acoeff: %s\n", _tostring(this.acoeff));
    k = k + 1
    str(k) = sprintf("rhs: %s\n", _tostring(this.rhs));
    k = k + 1
    str(k) = sprintf("ranges: %s\n", _tostring(this.ranges));
    k = k + 1
    str(k) = sprintf("bounds: %s\n", _tostring(this.bounds));
    k = k + 1
    str(k) = sprintf("stavar: %s\n", _tostring(this.stavar));
    k = k + 1
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
            [nr,nc] = size(x)
            tx = typeof(x)
            s = msprintf("%d-by-%d %s matrix",nr,nc,tx)
        end
    end
endfunction



