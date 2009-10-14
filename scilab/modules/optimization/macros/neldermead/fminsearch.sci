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
  defaultoptions = optimset ("fminsearch");
  if rhs==2 then
    // No options on the command line
    // Set default values
    options = defaultoptions;
  elseif rhs==3 then
    // One options struc on the command line : use it !
    options = varargin(3);
  end
  // Compute options from the options struct
  numberofvariables = size(x0,2);
  MaxFunEvals = optimget ( options , "MaxFunEvals" , defaultoptions.MaxFunEvals );
  MaxIter = optimget ( options , "MaxIter" , defaultoptions.MaxIter );
  TolFun = optimget ( options , "TolFun" , defaultoptions.TolFun );
  TolX = optimget ( options , "TolX" , defaultoptions.TolX );
  Display = optimget ( options , "Display" , defaultoptions.Display );
  OutputFcn = optimget ( options , "OutputFcn" , defaultoptions.OutputFcn );
  PlotFcns = optimget ( options , "PlotFcns" , defaultoptions.PlotFcns );
  // If the MaxIter option is a string, we make the assumption that it is the default 200 value.
  // If not, this is the actual value.
  if ( type ( MaxIter ) == 10 ) then
    if ( MaxIter == "200*numberofvariables" ) then
      MaxIter = 200 * numberofvariables;
    else
      errmsg = msprintf(gettext("%s: Unexpected maximum number of iterations %s."), "fminsearch", MaxIter );
      error(errmsg)
    end
  end
  // If the MaxFunEvals option is a string, this is the default 200 value
  // If not, this is the actual value.
  if ( type ( MaxFunEvals ) == 10 ) then
    if ( MaxFunEvals == "200*numberofvariables" ) then
      MaxFunEvals = 200 * numberofvariables;
    else
      errmsg = msprintf(gettext("%s: Unexpected maximum number of function evaluations %s."), "fminsearch", MaxFunEvals );
      error(errmsg)
    end
  end
  if ( Display == "iter" ) then
    mprintf ( "%10s   %10s   %10s %17s\n" , "Iteration", "Func-count" , "min f(x)" , "Procedure" );
  end
  // Prepare the data structure to pass to the output function
  fmsdata = tlist(["T_FMINSEARCH" 
    "Display" 
    "OutputFcn" 
    "PlotFcns" 
    ]);
  fmsdata.Display = Display
  fmsdata.OutputFcn = OutputFcn
  fmsdata.PlotFcns = PlotFcns
  // Prepare the data structure to pass to the cost function
  fmsfundata = tlist(["T_FMINSEARCH" 
    "Fun" 
    ]);
  fmsfundata.Fun = fun
  // Perform Optimization
  nm = neldermead_new ();
  nm = neldermead_configure(nm,"-x0",x0.');
  nm = neldermead_configure(nm,"-numberofvariables",numberofvariables);
  nm = neldermead_configure(nm,"-simplex0method","pfeffer");
  nm = neldermead_configure(nm,"-simplex0deltausual",0.05);
  nm = neldermead_configure(nm,"-simplex0deltazero",0.0075);
  nm = neldermead_configure(nm,"-method","variable");
  nm = neldermead_configure(nm,"-function",fminsearch_function);
  nm = neldermead_configure(nm,"-costfargument",fmsfundata);
  nm = neldermead_configure(nm,"-maxiter",MaxIter);
  nm = neldermead_configure(nm,"-maxfunevals",MaxFunEvals);
  nm = neldermead_configure(nm,"-tolxmethod",%f);
  nm = neldermead_configure(nm,"-tolfunmethod",%f);
  nm = neldermead_configure(nm,"-tolssizedeltafvmethod",%t);
  nm = neldermead_configure(nm,"-tolsimplexizemethod",%f);
  nm = neldermead_configure(nm,"-toldeltafv",TolFun);
  nm = neldermead_configure(nm,"-tolsimplexizeabsolute",TolX);
  nm = neldermead_configure(nm,"-checkcostfunction",%f);
  nm = neldermead_configure(nm,"-outputcommand",fminsearch_outputfun);
  nm = neldermead_configure(nm,"-outputcommandarg",fmsdata);
  //nm = neldermead_configure(nm,"-verbose",1);
  //nm = neldermead_configure(nm,"-verbosetermination",1);
  nm = neldermead_search(nm);
  x = neldermead_get(nm,"-xopt").';
  fval = neldermead_get(nm,"-fopt");
  status = neldermead_get(nm,"-status");
  select status
  case "maxiter" then
    if ( ( Display == "notify" ) | ( Display == "iter" ) | ( Display == "final" ) ) then
      mprintf(gettext("Exiting: Maximum number of iterations has been exceeded\n"))
      mprintf(gettext("         - increase MaxIter option.\n"))
      mprintf(gettext("         Current function value: %s\n") , string(fval) )
    end
    exitflag = 0;
  case "maxfuneval" then
    if ( ( Display == "notify" ) | ( Display == "iter" ) | ( Display == "final" ) ) then
      mprintf(gettext("Exiting: Maximum number of function evaluations has been exceeded\n"))
      mprintf(gettext("         - increase MaxFunEvals option.\n"))
      mprintf(gettext("         Current function value: %s\n") , string(fval) )
    end
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
    " the current x satisfies the termination criteria using OPTIONS.TolX of",...
    TolX,...
    " and F(X) satisfies the convergence criteria using OPTIONS.TolFun of",...
    TolFun);
  if ( ( Display == "final" ) | ( Display == "iter" ) ) then
    if ( ( exitflag == 1 ) ) then
      mprintf( "%s\n" , output.message(1) );
      mprintf( "%s\n" , output.message(2) );
      mprintf( "%s\n" , output.message(3) );
    end
  end
  nm = neldermead_destroy(nm);
endfunction
//
// The output function called back by fminsearch
//
// Arguments
//  state : the current state of the algorithm
//    "init", "iter", "done"
//  data : the data at the current state
//    This is a tlist with the following entries:
//    * x : the optimal vector of parameters
//    * fval : the minimum function value
//    * simplex : the simplex, as a simplex object
//    * iteration : the number of iterations performed
//    * funccount : the number of function evaluations
//    * step : the type of step in the previous iteration
//  fmsdata : this is a tlist which contains specific data of the 
//    fminsearch algorithm
//    * Display : what to display
//    * OutputFcn : the array of output functions
//
function fminsearch_outputfun ( state , data , fmsdata )
  if ( fmsdata.Display == "iter" ) then
    select data.step
    case "init" then
      if ( data.iteration == 0 ) then
        dstep = "";
      else
        dstep = "initial simplex";
      end
    case "done" then
      dstep = ""
    case "reflection" then
      dstep = "reflect"
    case "expansion" then
      dstep = "expand"
    case "insidecontraction" then
      dstep = "contract inside"
    case "outsidecontraction" then
      dstep = "contract outside"
    case "reflectionnext" then
      dstep = "reflectionnext"
    case "shrink" then
      dstep = "shrink"
    else
      errmsg = msprintf(gettext("%s: Unknown step %s"), "fminsearch", data.step)
      error(errmsg)
    end
    if ( data.step <> "done" ) then
    mprintf ( "%6s        %5s     %12s         %-20s\n", ...
      string(data.iteration) , string(data.funccount) , string(data.fval) , dstep )
    else
      mprintf ( "\n" )
    end
  end
  // Process output functions
  optimValues = struct(...
      "funcCount" ,data.funccount , ...
      "fval" ,data.fval , ...
      "iteration" , data.iteration , ...
      "procedure" , data.step ...
      );
  if ( fmsdata.OutputFcn <> [] ) then
    if ( type ( fmsdata.OutputFcn ) == 13 ) then
      // The output function is a macro
      fmsdata.OutputFcn ( data.x , optimValues , state );
    elseif ( type ( fmsdata.OutputFcn ) == 15 ) then
      // The output function is a list of macros
      for i = 1:length(fmsdata.OutputFcn)
        fmsdata.OutputFcn(i) ( data.x , optimValues , state );
      end
    else
      // The user did something wrong...
      errmsg = msprintf(gettext("%s: The output function associated with ''OutputFcn'' option is neither a function nor a list."), "fminsearch")
      error(errmsg)
    end
  end
  // Process plot functions
  if ( fmsdata.PlotFcns <> [] ) then
    if ( type ( fmsdata.PlotFcns ) == 13 ) then
      // The output function is a macro
      fmsdata.PlotFcns ( data.x , optimValues , state );
    elseif ( type ( fmsdata.PlotFcns ) == 15 ) then
      // The output function is a list of macros
      for i = 1:length(fmsdata.PlotFcns)
        fmsdata.PlotFcns(i) ( data.x , optimValues , state );
      end
    else
      // The user did something wrong...
      errmsg = msprintf(gettext("%s: The output function associated with ''PlotFcns'' option is neither a function nor a list."), "fminsearch")
      error(errmsg)
    end
  end
endfunction
//
// fminsearch_function --
//   Calls the cost function and make it match
//   neldermead requirements.
//
function [ f , index , fmsfundata ] = fminsearch_function ( x , index , fmsfundata )
  f = fmsfundata.Fun ( x )
endfunction

