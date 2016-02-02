// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
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
// neldermead_defaultoutput --
//   The default output function.
//
function stop = neldermead_defaultoutput(state, data)
    if ( state == "init" ) then
        mprintf ( "Initialization\n");
    elseif ( state == "done" ) then
        mprintf ( "End of Optimization\n");
    end
    mprintf ( "Iter. #%s, Feval #%s, Fval = %s -- %s\n", ..
    string(data.iteration), string(data.funccount), string(data.fval), data.step);
    stop = %f
endfunction
