// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Allan CORNET
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function demo_nmplot_qfixed2()

  mprintf(_("Illustrates that the fixed-shape Spendley et al. algorithm performs badly on a badly quadratic test case.\n"));
  mprintf(_("Defining quadratic function...\n"));
  a = 100;
  
  function [ y , index ] = quadratic ( x , index )
    y = a * x(1)^2 + x(2)^2;
  endfunction

  mprintf(_("Creating nmplot object ...\n"));
  nm = nmplot_new ();
  nm = nmplot_configure(nm, "-numberofvariables",2);
  nm = nmplot_configure(nm, "-function",quadratic);
  nm = nmplot_configure(nm, "-x0",[10.0 10.0]');
  nm = nmplot_configure(nm, "-maxiter",400);
  nm = nmplot_configure(nm, "-maxfunevals",400);
  nm = nmplot_configure(nm, "-tolxmethod",%f);
  nm = nmplot_configure(nm, "-tolsimplexizerelative",1.e-8);
  nm = nmplot_configure(nm, "-simplex0method","spendley");
  nm = nmplot_configure(nm, "-method","fixed");
  
  //
  // Setup output files
  //
  simplexfn = TMPDIR + filesep() + "history.simplex.txt";
  fbarfn = TMPDIR + filesep() + "history.fbar.txt";
  foptfn = TMPDIR + filesep() + "history.fopt.txt";
  sigmafn = TMPDIR + filesep() + "history.sigma.txt";
  nm = nmplot_configure(nm, "-simplexfn",simplexfn);
  nm = nmplot_configure(nm, "-fbarfn",fbarfn);
  nm = nmplot_configure(nm, "-foptfn",foptfn);
  nm = nmplot_configure(nm, "-sigmafn",sigmafn);
  
  //
  // Perform optimization
  //
  mprintf(_("Searching (please wait) ...\n"));
  nm = nmplot_search(nm);
  disp(nm);
  // Plot various histories
  mprintf(_("Plotting history of fbar ...\n"));
  f = scf();
  nmplot_historyplot ( nm ,fbarfn, mytitle = _("Function Value Average") , myxlabel = _("Iterations") );
  mprintf(_("Plotting history of fopt ...\n"));
  f = scf();
  nmplot_historyplot ( nm , foptfn, mytitle = _("Logarithm Minimum Function Value") , myxlabel = _("Iterations") );
  f.children.log_flags = "nln";
  newticks = tlist(["ticks","locations","labels"]);
  newticks.labels = ["1.e-20" "1.e-10" "1.e-1"];
  newticks.locations = [1.e-20 1.e-10 1.e-1];
  f.children.y_ticks = newticks;
  f.children.children(1).children.mark_mode = "on";
  f.children.children(1).children.mark_style = 9;
  mprintf(_("Plotting history of sigma ...\n"));
  f = scf();
  nmplot_historyplot ( nm , sigmafn, mytitle = _("Logarithm Maximum Oriented length") , myxlabel = _("Iterations") );
  f.children.log_flags = "nln";
  f.children.y_ticks = newticks;
  f.children.children(1).children.mark_mode = "on";
  f.children.children(1).children.mark_style = 9;
  // Plot the contours of the cost function and the simplex history
  mprintf(_("Plotting contour (please wait) ...\n"));
  nm = nmplot_configure(nm, "-verbose", 0);
  [nm , xdata , ydata , zdata ] = nmplot_contour ( nm , xmin = -5.0 , xmax = 12.0 , ymin = -2.0 , ymax = 12.0 , nx = 50 , ny = 50 );
  f = scf();
  drawlater();
  contour ( xdata , ydata , zdata , [10.0 50 100 1000 2000 5000 10000 20000] )
  nmplot_simplexhistory ( nm );
  drawnow();
  
  // Clean-up
  deletefile(simplexfn);
  deletefile(fbarfn);
  deletefile(foptfn);
  deletefile(sigmafn);
  nm = nmplot_destroy(nm);
  mprintf("End of demo.\n");

  //
  // Load this script into the editor
  //
  filename = 'nmplot_quadratic.fixed2.sce';
  dname = get_absolute_file_path(filename);
  editor ( dname + filename, "readonly" );

endfunction

demo_nmplot_qfixed2();
clear demo_nmplot_qfixed2;


