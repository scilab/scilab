// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
    scf(opfvh);
    nbvar = length(x)
    gg = gce();
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
    hh = gcf();
    hh.children.data_bounds = [
      xmin ymin
      xmax ymax
    ];
  end
endfunction

