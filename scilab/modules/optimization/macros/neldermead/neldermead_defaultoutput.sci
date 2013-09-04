// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


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
