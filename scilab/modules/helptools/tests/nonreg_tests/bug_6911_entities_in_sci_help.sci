// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
function  [s, v] = entities_in_sci_help(r,t)
    // Tests if entities like &#969; are supported & kept unconverted.
    // Calling Sequence
    //   [surface, volume] = entities_in_sci_help(r,t)
    //
    // Parameters
    //  r: vector of pulsations &#969;
    //  t: vector of theta angles of the mesh
    //
    // Description
    // Direct UTF-8 characters like "âäéèêë" & "ìïôöòûüùñ" are supported.
    // Literal HTML entities starting with & + letters + ; are not accepted.
    //
    // Examples
    //// This is an omega &#969; & here is a &eacute;
    //    r = linspace(1,2,30); t = linspace(0,2*%pi,100);
    //
    // See also
    //  help_from_sci
    //
    // Authors
    //  Samuel GOUGEON
    //
    s = 1;
    v = 2;
endfunction
