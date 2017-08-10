// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
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
// optimplotx --
//   Emulate the optimplotfx command of Matlab.
//   Plot the function value during an optimization process.
// Arguments, input
//   x : the current point
//   optimValues : a tlist which contains the following fields
//     funcCount" : the number of function evaluations
//     fval : the current function value
//     iteration : the current iteration
//     procedure : a string containing the current type of step
//  state : the current state of the algorithm
//    "init", "iter", "done"
// Notes:
//   The algorithm is the following.
//   At initialization of the algorithm, create an empty
//   graphic plot, retrieve the handle, and store a special
//   key "optimplotfx" in the user_data field of the handle.
//   When the plot is to update, this key is searched so that the
//   correct plot can be update (and not another).
//
function optimplotx ( x , optimValues , state )
    if ( state == "init" ) then
        // Initialize
        opfvh = scf();
        nbvar = length(x)
        bar ( 1:nbvar , x );
        gg = gce();
        gg.children.background = 9;
        opfvh.user_data = "optimplotx";
        opfvh.children.x_label.text = msprintf ( "Number of variables: %d" , nbvar );
        opfvh.children.y_label.text = "Current point";
        opfvh.children.title.text = msprintf ( "Current Point" );
    else
        opfvh = findobj ( "user_data" , "optimplotx" );
        nbvar = length(x)
        gg = opfvh.children.children;
        for ivar = 1:nbvar
            gg.children.data(ivar,2) = x(ivar);
        end
        xmin = 0;
        ymin = min(x);
        xmax = nbvar+1;
        ymax = max(x);
        // A small trick, for a nicer display
        if ( ( ymin > 0.0 ) & ( ymax > 0.0 ) ) then
            ymin = 0.0
        elseif ( ( ymin < 0.0 ) & ( ymax < 0.0 ) ) then
            ymax = 0.0
        end
        if ( ymax > 0 ) then
            ymax = ymax * 1.1
        end
        if ( ymin < 0 ) then
            ymin = ymin * 1.1
        end
        opfvh.children.data_bounds = [
        xmin ymin
        xmax ymax
        ];
    end
endfunction

