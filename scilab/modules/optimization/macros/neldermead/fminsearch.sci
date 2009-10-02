// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// fminsearch --
//   Emulate the fminsearch command of Matlab.
//   Search the minimum with Nelder-Mead algorithm.
//   [x,fval,exitflag,output] = fminsearch(fun,x0,options)
// Arguments, input
//   fun : the function to minimize
//   x0 : a row vector with dimension n where n is the number of parameters
//        to optimize.
//        Initial guess for optimization algorithm.
//  options : an optional struct, as provided by optimset
//
// TODO : implement Implement the following fminsearch features : Display, OutputFcn, PlotFcns
function [x,fval,exitflag,output] = fminsearch ( varargin )
  [lhs,rhs]=argn();
  if rhs<>2 & rhs<>3 then
    errmsg = msprintf(gettext("%s: Unexpected number of input arguments : %d provided while 2 or 3 are expected."), "fminsearch", rhs);
    error(errmsg)
  end
  fun = varargin(1);
  x0 = varargin(2);
  if rhs==2 then
    // No options on the command line
    // Set default values
    options = optimset ("fminsearch");
  elseif rhs==3 then
    // One options struc on the command line : use it !
    options = varargin(3);
  end
  // Compute options from the options struct
  numberofvariables = size(x0,2);
  // If the MaxIter option is a string, this is the default 200 value.
  // If not, this is the actual value.
  if type(options.MaxIter)==10 then
    MaxIter = 200 * numberofvariables;
  else
    MaxIter = options.MaxIter;
  end
  // If the MaxFunEvals option is a string, this is the default 200 value
  // If not, this is the actual value.
  if type(options.MaxFunEvals)==10 then
    MaxFunEvals = 200 * numberofvariables;
  else
    MaxFunEvals = options.MaxFunEvals;
  end
  TolFun = options.TolFun;
  TolX = options.TolX;
  // Perform Optimization
  nm = neldermead_new ();
  nm = neldermead_configure(nm,"-x0",x0.');
  nm = neldermead_configure(nm,"-numberofvariables",numberofvariables);
  nm = neldermead_configure(nm,"-simplex0method","pfeffer");
  nm = neldermead_configure(nm,"-simplex0deltausual",0.05);
  nm = neldermead_configure(nm,"-simplex0deltazero",0.0075);
  nm = neldermead_configure(nm,"-method","variable");
  nm = neldermead_configure(nm,"-function",fun);
  nm = neldermead_configure(nm,"-maxiter",MaxIter);
  nm = neldermead_configure(nm,"-maxfunevals",MaxFunEvals);
  nm = neldermead_configure(nm,"-tolxmethod",%f);
  nm = neldermead_configure(nm,"-tolfunmethod",%f);
  nm = neldermead_configure(nm,"-tolssizedeltafvmethod",%t);
  nm = neldermead_configure(nm,"-tolsimplexizemethod",%f);
  nm = neldermead_configure(nm,"-toldeltafv",TolFun);
  nm = neldermead_configure(nm,"-tolsimplexizeabsolute",TolX);
  nm = neldermead_configure(nm,"-checkcostfunction",%f);
//nm = neldermead_configure(nm,"-verbose",1);
//nm = neldermead_configure(nm,"-verbosetermination",1);
  nm = neldermead_search(nm);
  x = neldermead_get(nm,"-xopt").';
  fval = neldermead_get(nm,"-fopt");
  status = neldermead_get(nm,"-status");
  select status
  case "maxiter" then
    exitflag = -1;
  case "maxfuneval" then
    exitflag = 0;
  case "tolsizedeltafv" then
    exitflag = 1;
  else
    errmsg = msprintf(gettext("%s: Unknown status %s"), "fminsearch", status)
    error(errmsg)
  end
  output = struct(...
      "algorithm" ,[],...
      "funcCount" ,[],...
      "iterations" ,[],...
      "message" , []);
  output.algorithm = 'Nelder-Mead simplex direct search';
  output.funcCount = neldermead_get(nm,"-funevals");
  output.iterations = neldermead_get(nm,"-iterations");
  output.message = sprintf("%s\n%s %e\n%s %e", "Optimization terminated:",...
    "the current x satisfies the termination criteria using OPTIONS.TolX of",...
    TolX,...
    "and F(X) satisfies the convergence criteria using OPTIONS.TolFun of",...
    TolFun);
  nm = neldermead_destroy(nm);
  clear nm;
endfunction

