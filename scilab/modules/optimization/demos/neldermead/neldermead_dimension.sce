// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

mprintf("Illustrates the sensitivity to dimension of the Nelder-Mead algorithm\n");

function [ f , index ] = quadracticn ( x , index )
  f = sum(x.^2);
endfunction
//
// solvepb --
//   Find the solution for the given number of dimensions
//
function [nbfevals , niter , rho] = solvepb ( n )
  rand("seed",0)
  nm = neldermead_new ();
  nm = neldermead_configure(nm,"-numberofvariables",n);
  nm = neldermead_configure(nm,"-function",quadracticn);
  nm = neldermead_configure(nm,"-x0",zeros(n,1));
  nm = neldermead_configure(nm,"-maxiter",2000);
  nm = neldermead_configure(nm,"-maxfunevals",2000);
  nm = neldermead_configure(nm,"-tolxmethod",%f);
  nm = neldermead_configure(nm,"-tolsimplexizerelative",0.0);
  nm = neldermead_configure(nm,"-tolsimplexizeabsolute",1.e-8);
  nm = neldermead_configure(nm,"-simplex0method","given");
  coords (1,1:n) = zeros(1,n);
  for i = 2:n+1
    coords (i,1:n) = 2.0 * rand(1,n) - 1.0;
  end
  nm = neldermead_configure(nm,"-coords0",coords);
  //
  // Perform optimization
  //
  nm = neldermead_search(nm);
  //neldermead_display(nm);
  si0 = neldermead_get ( nm , "-simplex0" );
  sigma0 = optimsimplex_size ( si0 , "sigmaplus" );
  siopt = neldermead_get ( nm , "-simplexopt" );
  sigmaopt = optimsimplex_size ( siopt , "sigmaplus" );
  niter = neldermead_get ( nm , "-iterations" );
  rho = (sigmaopt/sigma0)^(1.0/niter);
  nbfevals = neldermead_get ( nm , "-funevals" );
  mprintf ( "%d %d %d %f\n", n , nbfevals , niter , rho );
  nm = neldermead_destroy(nm);
endfunction


for n = 1:20
  [nbfevals niter rho] = solvepb ( n );
  array_rho(n) = rho;
  array_nbfevals(n) = nbfevals;
  array_niter(n) = niter;
end
// Plot rate of convergence
hh = scf();
plot(1:20,array_rho)
hh.children.x_label.text = "Number of parameters"
hh.children.y_label.text = "Rate of convergence"
hh.children.children.children.mark_mode = "on";
hh.children.children.children.mark_style = 9;
hh.children.children.children.mark_size = 10;

// Plot number of function evaluations
hh = scf();
plot(1:20,array_nbfevals)
hh.children.x_label.text = "Number of parameters"
hh.children.y_label.text = "Number of function evaluations"
hh.children.children.children.mark_mode = "on";
hh.children.children.children.mark_style = 9;
hh.children.children.children.mark_size = 10;
mprintf("End of demo.\n");

//
// Load this script into the editor
//
filename = 'neldermead_dimension.sce';
dname = get_absolute_file_path(filename);
editor ( dname + filename );

