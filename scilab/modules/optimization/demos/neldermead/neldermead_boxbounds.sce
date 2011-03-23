// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// neldermeadBounds.sce --
//   Show a simple neldermead session with bounds.
//

function demo_boxbounds()

  mprintf(_("Illustrates Box'' algorithm on a simply bounds-constrained optimization problem.\n"));

  // A simple quadratic function
  function [ f , index ] = myquad ( x , index )
    f = x(1)^2 + x(2)^2
  endfunction

  //
  // Initialize the random number generator, so that the results are always the
  // same.
  //
  rand("seed" , 0)

  x0 = [1.2 1.9].';
  // Compute f(x0) : should be close to -2351244.0
  [ fx0 , index ] = myquad ( x0 , 2 );
  mprintf("Computed fx0 = %e (expected = %e)\n",fx0 , 5.05 );

  xopt = [1 1].';
  // Compute f(xopt) : should be 2
  [ fopt , index ] = myquad ( xopt , 2 );
  mprintf("Computed fopt = %e (expected = %e)\n", fopt , 2 );

  nm = nmplot_new ();
  nm = nmplot_configure(nm,"-numberofvariables",2);
  nm = nmplot_configure(nm,"-function",myquad);
  nm = nmplot_configure(nm,"-x0",x0);
  nm = nmplot_configure(nm,"-method","box");
  nm = nmplot_configure(nm,"-boundsmin",[1 1]);
  nm = nmplot_configure(nm,"-boundsmax",[2 2]);

  //
  // Check that the cost function is correctly connected.
  //
  [ nm , f ] = nmplot_function ( nm , x0 );
  nm = nmplot_configure(nm,"-simplexfn",TMPDIR + "/history.simplex.txt");
  
  //
  // Perform optimization
  //
  mprintf(_("Searching (please wait) ...\n"));
  nm = nmplot_search(nm);
  mprintf(_("...Done\n"));
  nmplot_display(nm);
  mprintf("==========================\n");
  xcomp = nmplot_get(nm,"-xopt");
  mprintf("x computed = %s\n",strcat(string(xcomp), " "));
  mprintf("x expected = %s\n",strcat(string(xopt), " "));
  shift = norm(xcomp-xopt)/norm(xopt);
  mprintf("Shift = %f\n",shift);
  fcomp = nmplot_get(nm,"-fopt");
  mprintf("f computed = %f\n",fcomp);
  mprintf("f expected = %f\n",fopt);
  shift = abs(fcomp-fopt)/abs(fopt);
  mprintf("Shift =%f\n",shift);
  
  //
  // Plot
  //
  mprintf(_("Plot contour (please wait)...\n"));
  [nm , xdata , ydata , zdata ] = nmplot_contour ( nm , xmin = 0.5 , xmax = 2.1 , ymin = 0.5 , ymax = 2.1 , nx = 50 , ny = 50 );
  
  my_handle = scf(100001);
  clf(my_handle,"reset");
  xset("fpf"," ")
  drawlater();
  contour ( xdata , ydata , zdata , linspace(min(zdata),max(zdata),10) )
  nmplot_simplexhistory ( nm );
  drawnow();

  nm = nmplot_destroy(nm);
  mprintf(_("End of demo.\n"));

endfunction

demo_boxbounds();
clear demo_boxbounds;
