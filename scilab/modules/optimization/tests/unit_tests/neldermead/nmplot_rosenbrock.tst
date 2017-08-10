// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
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



function [ y , index ] = rosenbrock ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

// Use 20 points in X and 20 points in Y
NP = 20;

//
// Check behaviour with default settings.
//
nm = nmplot_new ();
nm = nmplot_configure(nm,"-numberofvariables",2);
nm = nmplot_configure(nm,"-function",rosenbrock);
nm = nmplot_configure(nm,"-x0",[-1.2 1.0].');
nm = nmplot_configure(nm,"-maxiter",10);
nm = nmplot_configure(nm,"-maxfunevals",300);
nm = nmplot_configure(nm,"-tolfunrelative",10*%eps);
nm = nmplot_configure(nm,"-tolxrelative",10*%eps);
nm = nmplot_configure(nm,"-simplex0method","axes");
nm = nmplot_configure(nm,"-simplex0length",1.0);
nm = nmplot_configure(nm,"-method","variable");
//
// Setup output files
//
simplexfn = fullfile(TMPDIR,"simplex.txt");
fbarfn = fullfile(TMPDIR,"fbar.txt");
foptfn = fullfile(TMPDIR,"fopt.txt");
sigmafn = fullfile(TMPDIR,"sigma.txt");
nm = nmplot_configure(nm,"-simplexfn",simplexfn);
nm = nmplot_configure(nm,"-fbarfn",fbarfn);
nm = nmplot_configure(nm,"-foptfn",foptfn);
nm = nmplot_configure(nm,"-sigmafn",sigmafn);
//
// Perform optimization
//
nm = nmplot_search(nm);
// Plot the contours of the cost function and the simplex history
xmin = -2.0; 
xmax = 2.0 ; 
ymin = -2.0 ; 
ymax = 2.0 ; 
nx = NP ; 
ny = NP;
[nm , xdata , ydata , zdata ] = nmplot_contour ( nm , ...
  xmin , xmax , ymin , ymax , nx , ny );
f = scf();
drawlater();
contour ( xdata , ydata , zdata , 20 )
nmplot_simplexhistory ( nm );
drawnow();
close(f);
f = scf();
mytitle = "Function Value Average" ; 
myxlabel = "Iterations";
nmplot_historyplot ( nm , fbarfn , mytitle , myxlabel);
close(f);
f = scf();
mytitle = "Minimum Function Value" ; 
myxlabel = "Iterations";
nmplot_historyplot ( nm , foptfn , mytitle , myxlabel);
close(f);
f = scf();
mytitle = "Maximum Oriented length" ; 
myxlabel = "Iterations";
nmplot_historyplot ( nm , sigmafn , mytitle , myxlabel);
close(f);
deletefile(simplexfn);
deletefile(fbarfn);
deletefile(foptfn);
deletefile(sigmafn);
nm = nmplot_destroy(nm);



