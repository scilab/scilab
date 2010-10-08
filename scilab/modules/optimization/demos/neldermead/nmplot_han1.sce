// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function demo_nmplot_1()

  mprintf(_("Illustrates the 1st counter example given by Han et al.\n"));

  //
  // han1 --
  //   Compute the cost function from the Han Phd Thesis
  //   which exhibits a failure of the NM method.
  // Reference 
  //   Algorithms in Unconstrained Optimization
  //   Han, Lixing
  //   2000
  //   Ph.D., The University of Connecticut
  //
  mprintf(_("Defining Han function...\n"));
  
  function [ f , index ] = han1 ( x , index )
    f = x(1)^2 + x(2) * (x(2) + 2.0) * (x(2) - 0.5) * (x(2) - 2.0);
  endfunction


  mprintf(_("Defining initial simplex coordinates...\n"));
  coords0 = [
    0.  -1.  
    0.   1.  
    1.   0.  
  ];


  mprintf(_("Creating nmplot object...\n"));
  nm = nmplot_new ();
  mprintf(_("Configuring nmplot object...\n"));
  nm = nmplot_configure(nm, "-numberofvariables", 2);
  nm = nmplot_configure(nm, "-function", han1);
  nm = nmplot_configure(nm, "-x0", [1.0 1.0]');
  nm = nmplot_configure(nm, "-maxiter", 50);
  nm = nmplot_configure(nm, "-maxfunevals", 300);
  nm = nmplot_configure(nm, "-tolfunrelative", 10*%eps);
  nm = nmplot_configure(nm, "-tolxrelative", 10*%eps);
  nm = nmplot_configure(nm, "-simplex0method", "given");
  nm = nmplot_configure(nm, "-coords0", coords0);
  
  //
  // Setup output files
  //
  mprintf(_("Setup output files...\n"));
  simplexfn = TMPDIR + filesep() + "history.simplex.txt";
  fbarfn = TMPDIR + filesep() + "history.fbar.txt";
  foptfn = TMPDIR + filesep() + "history.fopt.txt";
  sigmafn = TMPDIR + filesep() + "history.sigma.txt";
  nm = nmplot_configure(nm, "-simplexfn", simplexfn);
  nm = nmplot_configure(nm, "-fbarfn", fbarfn);
  nm = nmplot_configure(nm, "-foptfn", foptfn);
  nm = nmplot_configure(nm, "-sigmafn", sigmafn);
  
  //
  // Perform optimization
  //
  mprintf(_("Searching (please wait)...\n"));
  nm = nmplot_search(nm);
  nmplot_display(nm);
  
  //
  // Plot the history of the simplex
  //
  mprintf(_("Plotting contour (please wait)...\n"));
  [nm , xdata , ydata , zdata ] = nmplot_contour ( nm , xmin = -0.2 , xmax = 1.2 , ymin = -2.0 , ymax = 2.0 , nx = 50 , ny = 50 );
  my_handle = scf(100001);
  clf(my_handle,"reset");
  drawlater();
  contour ( xdata , ydata , zdata , [-5 -4 -2 -1 0 1 1.5] )
  nmplot_simplexhistory ( nm );
  drawnow();
  
  //
  // Clean-up
  //
  deletefile(simplexfn);
  deletefile(fbarfn);
  deletefile(foptfn);
  deletefile(sigmafn);
  nm = nmplot_destroy(nm);
  mprintf(_("End of demo.\n"));
  
  //
  // Load this script into the editor
  //
  filename = 'nmplot_han1.sce';
  dname = get_absolute_file_path(filename);
  editor ( dname + filename, "readonly" );

endfunction

demo_nmplot_1();
clear demo_nmplot_1;
