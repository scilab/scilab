// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// References
//   Sequential Application of Simplex Designs in Optimisation and Evolutionary Operation
//   Spendley, W. and Hext, G. R. and Himsworth, F. R.
//   1962
//
//   Convergence Properties of the Nelder-Mead Simplex Method in Low Dimensions
//   Jeffrey C. Lagarias and James A. Reeds nd Margaret H. Wright and Paul E. Wright
//   SIAM Journal of Optimization, 1998, volume 9, pp. 112--147
//
//   A Simplex Method for Function Minimization
//   Nelder, J. A.  and Mead, R.
//   1965
//
//   Iterative Methods for Optimization
//   C. T. Kelley,
//   SIAM Frontiers in Applied Mathematics
//   1999
//
//   Detection and Remediation of Stagnation in the Nelder--Mead Algorithm Using a Sufficient Decrease Condition
//   Kelley C. T.
//   SIAM J. on Optimization},
//   1999
//

//
// neldermead_search --
//   Search the minimum with Nelder-Mead algorithm.
//
function this = neldermead_search ( this )
  if ( this.startupflag == 0) then
    this = neldermead_startup ( this );
    this.startupflag = 1;
  end
  neldermead_outputcmd ( this, "init" , this.simplex0 )
  if this.restartflag == 1 then
    this = neldermead_autorestart ( this )
  else
    this = neldermead_algo ( this );
  end
  neldermead_outputcmd ( this, "done" , this.simplexopt )
endfunction
//
// neldermead_algo --
//   Performs an optimization without restart
//
function this = neldermead_algo ( this )
    select this.method
    case "fixed" then
      this = neldermead_fixed (this);
    case "variable" then
      this = neldermead_variable (this);
    case "box" then
      this = neldermead_box (this);
    else
      errmsg = msprintf(gettext("%s: Unknown -method %s"), "neldermead_algo", this.method)
      error(errmsg)
    end
endfunction
//
// neldermead_autorestart --
//   Performs an optimization with automatic restart
//
function this = neldermead_autorestart ( this )
  for irestart = 1: this.restartmax
    this = neldermead_log (this,sprintf("Restart #%d/%d", irestart,this.restartmax));
    this = neldermead_algo ( this );
    [ this , istorestart ] = neldermead_istorestart ( this );
    if istorestart==0 then
      this = neldermead_log (this,"Must not restart");
      this.restartnb  = irestart
      break
    else
      this = neldermead_log (this,"Must restart");
    end
    if ( irestart == this.restartmax ) then
      this = neldermead_log (this,"Stopping after all restarts performed");
      this.restartnb  = irestart
      this.optbase = optimbase_set ( this.optbase , "-status" , "maxrestart" );
    else
      this = neldermead_updatesimp ( this );
    end
  end
endfunction


//
// neldermead_variable --
//   The original Nelder-Mead algorithm, with variable-size simplex.
//
function this = neldermead_variable ( this )
  //
  // Order the vertices for the first time
  //
  simplex = this.simplex0;
  n = optimbase_cget ( this.optbase , "-numberofvariables" );
  if (n==0) then
    errmsg = msprintf(gettext("%s: The number of variable is zero."), "neldermead_variable")
    error(errmsg)
  end
  fvinitial = optimbase_get ( this.optbase , "-fx0" );
  // Sort function values and x points by increasing function value order
  this = neldermead_log (this,"Step #1 : order");
  simplex = optimsimplex_sort ( simplex );
  currentcenter = optimsimplex_center ( simplex );
  currentxopt = optimbase_cget ( this.optbase , "-x0" );
  newfvmean = optimsimplex_fvmean ( simplex );
  //
  // Initialize
  //
  terminate = 0;
  iter = 0;
  //
  // Nelder-Mead Loop
  //
  while ( terminate == 0 )
    this.optbase = optimbase_incriter ( this.optbase );
    iter = iter + 1;
    xlow = optimsimplex_getx ( simplex , 1 )
    flow = optimsimplex_getfv ( simplex , 1 )
    xhigh = optimsimplex_getx ( simplex , n+1 )
    fhigh = optimsimplex_getfv ( simplex , n+1 )
    xn = optimsimplex_getx ( simplex , n )
    fn = optimsimplex_getfv ( simplex , n )
    //
    // Store history
    //
    xcoords = optimsimplex_getallx ( simplex )
    this = neldermead_storehistory ( this , n , flow , xlow , xcoords );
    deltafv = abs(optimsimplex_deltafvmax ( simplex ));
    currentfopt = flow;
    previousxopt = currentxopt;
    currentxopt = xlow;
    previouscenter = currentcenter;
    currentcenter = optimsimplex_center ( simplex );
    oldfvmean = newfvmean;
    newfvmean = optimsimplex_fvmean ( simplex );
    totaliter = optimbase_get ( this.optbase , "-iterations" );
    funevals = optimbase_get ( this.optbase , "-funevals" );
    ssize = optimsimplex_size ( simplex )
    this = neldermead_log (this,sprintf("================================================================="));
    this = neldermead_log (this,sprintf("Iteration #%d (total = %d)",iter,totaliter));
    this = neldermead_log (this,sprintf("Function Eval #%d",funevals));
    this = neldermead_log (this,sprintf("Xopt : %s",strcat(string(xlow)," ")));
    this = neldermead_log (this,sprintf("Fopt : %e",flow));
    this = neldermead_log (this,sprintf("DeltaFv : %e",deltafv));
    this = neldermead_log (this,sprintf("Center : %s",strcat(string(currentcenter)," ")));
    this = neldermead_log (this,sprintf("Size : %e",ssize));
    str = optimsimplex_tostring ( simplex )
    for i = 1:n+1
      this = neldermead_log (this,str(i));
    end
    this.optbase = optimbase_set ( this.optbase , "-xopt" , xlow );
    this.optbase = optimbase_set ( this.optbase , "-fopt" , flow );
    neldermead_outputcmd ( this, "iter" , simplex )

    //
    // Update termination flag
    //
    if ( iter > 1 ) then
      [this , terminate , status] = neldermead_termination (this , ...
        fvinitial , oldfvmean , newfvmean , previouscenter , currentcenter , simplex );
      if (terminate==1) then
        this = neldermead_log (this,sprintf("Terminate with status : %s",status));
        break
      end
    end
    //
    // Compute xbar, center of better vertices
    //
    this = neldermead_log (this,sprintf("Reflect"));
    xbar   = optimsimplex_xbar ( simplex );
    this = neldermead_log (this,sprintf("xbar="+strcat(string(xbar)," ")+""));
    //
    // Reflect the worst point with respect to center
    //
    xr = neldermead_interpolate ( xbar , xhigh , this.rho );
    [ this , fr ] = neldermead_function ( this , xr );
    this = neldermead_log (this,sprintf("xr=["+strcat(string(xr)," ")+"], f(xr)=%f",fr));
    if ( fr >= flow & fr < fn ) then
      this = neldermead_log (this,sprintf("  > Perform reflection"));
      simplex = optimsimplex_setve ( simplex , n+1 , fr , xr )
    elseif ( fr < flow ) then
      // Expand
      this = neldermead_log (this,sprintf("Expand"));
      xe = neldermead_interpolate ( xbar , xhigh , this.rho*this.chi );
      [ this ,fe] = neldermead_function ( this ,xe);
      this = neldermead_log (this,sprintf("xe="+strcat(string(xe)," ")+", f(xe)=%f",fe));
      if (fe < fr) then
        this = neldermead_log (this,sprintf("  > Perform Expansion"));
        simplex = optimsimplex_setve ( simplex , n+1 , fe , xe )
      else
        this = neldermead_log (this,sprintf("  > Perform reflection"));
        simplex = optimsimplex_setve ( simplex , n+1 , fr , xr )
      end
    elseif ( fr >= fn & fr < fhigh ) then
      // Outside contraction
      this = neldermead_log (this,sprintf("Contract - outside"));
      xc = neldermead_interpolate ( xbar , xhigh , this.rho*this.gamma );
      [ this ,fc] = neldermead_function ( this ,xc);
      this = neldermead_log (this,sprintf("xc="+strcat(string(xc)," ")+", f(xc)=%f",fc));
      if ( fc <= fr ) then
        this = neldermead_log (this,sprintf("  > Perform Outside Contraction"));
        simplex = optimsimplex_setve ( simplex , n+1 , fc , xc )
      else
        //  Shrink
        this = neldermead_log (this,sprintf("  > Perform Shrink"));
        [ simplex , this ] = optimsimplex_shrink ( simplex , neldermead_costf , this.sigma , this );
      end
    else
      // ( fr >= fn & fr >= fhigh )  
      // Inside contraction
      this = neldermead_log (this,sprintf("Contract - inside"));
      xc = neldermead_interpolate ( xbar , xhigh , -this.gamma );
      [ this ,fc] = neldermead_function ( this ,xc);
      this = neldermead_log (this,sprintf("xc="+strcat(string(xc)," ")+", f(xc)=%f",fc));
      if ( fc < fhigh ) then
        this = neldermead_log (this,sprintf("  > Perform Inside Contraction"));
        simplex = optimsimplex_setve ( simplex , n+1 , fc , xc )
      else
        //  Shrink
        this = neldermead_log (this,sprintf("  > Perform Shrink"));
        [ simplex , this ] = optimsimplex_shrink ( simplex , neldermead_costf , this.sigma , this )
      end
    end
    //
    // Sort simplex
    //
    this = neldermead_log (this,sprintf("Sort"));
    simplex  = optimsimplex_sort ( simplex );
  end
  this.optbase = optimbase_set ( this.optbase , "-xopt" , xlow );
  this.optbase = optimbase_set ( this.optbase , "-fopt" , flow );
  this.optbase = optimbase_set ( this.optbase , "-status" , status );
  this.simplexopt = simplex;
endfunction

//
// neldermead_fixed --
//   The simplex algorithm with fixed size simplex.
//
function this = neldermead_fixed (this)
  //
  // Order the vertices for the first time
  //
  simplex = this.simplex0;
  n = optimbase_cget ( this.optbase , "-numberofvariables" );
  fvinitial = optimbase_get ( this.optbase , "-fx0" );
  // Sort function values and x points by increasing function value order
  this = neldermead_log (this,sprintf("Sort"));
  simplex = optimsimplex_sort ( simplex );
  neldermead_outputcmd ( this, "init" , simplex )
  //
  // Compute center of simplex
  //
  currentcenter = optimsimplex_center ( simplex );
  newfvmean = optimsimplex_fvmean ( simplex );
  currentxopt = optimbase_cget ( this.optbase , "-x0" );
  //
  // Initialize
  //
  terminate = 0;
  iter = 0;
  //
  // main N-M loop
  //
  while (terminate == 0)
    this.optbase = optimbase_incriter ( this.optbase );
    iter = iter + 1;
    xlow = optimsimplex_getx ( simplex , 1 )
    flow = optimsimplex_getfv ( simplex , 1 )
    xhigh = optimsimplex_getx ( simplex , n+1 )
    fhigh = optimsimplex_getfv ( simplex , n+1 )
    //
    // Store history
    //
    xcoords = optimsimplex_getallx ( simplex )
    this = neldermead_storehistory ( this , n , flow , xlow , xcoords );
    deltafv = abs(optimsimplex_deltafvmax ( simplex ));
    currentfopt = flow;
    previousxopt = currentxopt;
    currentxopt = xlow;
    previouscenter = currentcenter;
    currentcenter = optimsimplex_center ( simplex );
    oldfvmean = newfvmean;
    newfvmean = optimsimplex_fvmean ( simplex );
    totaliter = optimbase_get ( this.optbase , "-iterations" );
    funevals = optimbase_get ( this.optbase , "-funevals" );
    ssize = optimsimplex_size ( simplex )
    this = neldermead_log (this,sprintf("================================================================="));
    this = neldermead_log (this,sprintf("Iteration #%d (total = %d)",iter,totaliter));
    this = neldermead_log (this,sprintf("Function Eval #%d",funevals));
    this = neldermead_log (this,sprintf("Xopt : %s",strcat(string(xlow)," ")));
    this = neldermead_log (this,sprintf("Fopt : %e",flow));
    this = neldermead_log (this,sprintf("DeltaFv : %e",deltafv));
    this = neldermead_log (this,sprintf("Center : %s",strcat(string(currentcenter)," ")));
    this = neldermead_log (this,sprintf("Size : %e",ssize));
    str = optimsimplex_tostring ( simplex )
    for i = 1:n+1
      this = neldermead_log (this,str(i));
    end
    this.optbase = optimbase_set ( this.optbase , "-xopt" , xlow );
    this.optbase = optimbase_set ( this.optbase , "-fopt" , flow );
    neldermead_outputcmd ( this, "iter" , simplex )
    //
    // Update termination flag
    //
    if ( iter > 1 ) then
      [this , terminate , status] = neldermead_termination (this , ...
        fvinitial , oldfvmean , newfvmean , previouscenter , currentcenter , simplex );
      if (terminate==1) then
        this = neldermead_log (this,sprintf("Terminate with status : %s",status));
        break;
      end
    end
    //
    // Compute xbar, center of better vertices
    //
    this = neldermead_log (this,sprintf("Reflect"));
    xbar = optimsimplex_xbar ( simplex );
    this = neldermead_log (this,sprintf("xbar="+strcat(string(xbar)," ")+""));
    //
    // Reflect the worst point with respect to center
    //
    xr = neldermead_interpolate ( xbar , xhigh , this.rho );
    [ this ,fr] = neldermead_function ( this ,xr);
    this = neldermead_log (this,sprintf("xr="+strcat(string(xr)," ")+", f(xr)=%f",fr));
    //
    // Replace worst point by xr if it is better
    //
    if ( fr < fhigh ) then
      this = neldermead_log (this,sprintf("  > Perform reflect"));
      simplex = optimsimplex_setve ( simplex , n+1 , fr , xr )
    else
      //  Shrink
      this = neldermead_log (this,sprintf("  > Perform Shrink"));
      [ simplex , this ] = optimsimplex_shrink ( simplex , neldermead_costf , this.sigma , this )
    end
    //
    // Sort simplex
    //
    simplex = optimsimplex_sort ( simplex );
  end
  this.optbase = optimbase_set ( this.optbase , "-xopt" , xlow );
  this.optbase = optimbase_set ( this.optbase , "-fopt" , flow );
  this.optbase = optimbase_set ( this.optbase , "-status" , status );
  this.simplexopt = simplex;
endfunction
//
// neldermead_interpolate --
//   Computes xi as an interpolation between x1 and x2, with factor as :
//     xi = (1+fac)x1 - fac x2
//
function xi = neldermead_interpolate ( x1 , x2 , fac )
  xi = (1 + fac)*x1 - fac*x2;
endfunction

//
// neldermead_termination --
//   Returns 1 if the algorithm terminates.
//   Returns 0 if the algorithm must continue.
// Arguments
//   this : the current object
//   fvinitial : initial function value
//   newfvmean, oldfvmean : the old and new function value average on the simplex
//   previousxopt : the previous value of x
//   currentxopt : the current value of x
//   simplex : the simplex
//     The best point in the simplex is expected to be stored at 1
//     The worst point in the simplex is expected to be stored at n+1
//   terminate : 1 if the algorithm terminates, 0 if the algorithm must continue.
//   this.status : termination status
//     status = "continue"
//     status = "maxiter"
//     status = "maxfuneval"
//     status = "tolf"
//     status = "tolx"
//     status = "tolfstdev"
//     status = "tolsize"
//     status = "tolsizedeltafv"
// Notes
//   Use the function average on the simplex instead of the best function value.
//   This is because the function average changes at each iteration.
//   Instead, the best function value as a step-by-step evolution and may not
//   change in 2 iterations, leading to astop of the algorithm.
// TODO : set the fvinitial, oldfvmean, newfvmean.
//
function [this , terminate , status ] = neldermead_termination (this , ...
  fvinitial , oldfvmean , newfvmean , previousxopt , currentxopt , ...
  simplex )
  terminate = 0;
  status = "continue";
  //
  // Termination Criteria from parent optimization class
  //
  [ this.optbase , terminate , status] = optimbase_terminate ( this.optbase , ...
    fvinitial , newfvmean , previousxopt , currentxopt );
  //
  // Criteria #5 : standard deviation of function values
  //
  if (terminate == 0) then
    if this.tolfstdeviationmethod == "enabled" then
      fv = optimsimplex_getallfv ( simplex )
      sd = st_deviation(fv);
      this.optbase = optimbase_stoplog  ( this.optbase,sprintf("  > st_deviation(fv)=%e < tolfstdeviation=%e",...
        sd, this.tolfstdeviation));
      if sd < this.tolfstdeviation then
        terminate = 1;
        status = "tolfstdev";
      end
    end
  end
  //
  // Criteria #6 : simplex absolute + relative size
  //
  if (terminate == 0) then
    if this.tolsimplexizemethod == "enabled" then
      ssize = optimsimplex_size ( simplex , "sigmaplus" );
      this.optbase = optimbase_stoplog  ( this.optbase,sprintf("  > simplex size=%e < %e + %e * %e",...
        ssize, this.tolsimplexizeabsolute , this.tolsimplexizerelative , this.simplexsize0 ));
      if ssize < this.tolsimplexizeabsolute + this.tolsimplexizerelative * this.simplexsize0 then
        terminate = 1;
        status = "tolsize";
      end
    end
  end
  //
  // Criteria #7 : simplex absolute size + difference in function values (Matlab-like)
  //
  if (terminate == 0) then
    if this.tolssizedeltafvmethod == "enabled" then
      ssize = optimsimplex_size ( simplex , "sigmaplus" );
      this.optbase = optimbase_stoplog  ( this.optbase,sprintf("  > simplex size=%e < %e",...
        ssize, this.tolsimplexizeabsolute));
      shiftfv = abs(optimsimplex_deltafvmax( simplex ))
      this.optbase = optimbase_stoplog  ( this.optbase,sprintf("  > abs(fv(n+1) - fv(1))=%e < toldeltafv=%e",...
        shiftfv, this.toldeltafv));
      if ssize < this.tolsimplexizeabsolute & shiftfv < this.toldeltafv then
        terminate = 1;
        status = "tolsizedeltafv";
      end
    end
  end
  //
  // Criteria #8 : Kelley stagnation, based on
  // a sufficient decrease condition
  //
  if ( terminate == 0 ) then
    if ( this.kelleystagnationflag==1 ) then
      [ sg , this ] = optimsimplex_gradientfv ( simplex , neldermead_costf , "forward" , this );
      nsg = sg' * sg;
      sgstr = strcat(string(sg)," ");
      this.optbase = optimbase_stoplog ( this.optbase , sprintf ( "Test Stagnation : nsg = %e, sg = "+sgstr, nsg) );
      this.optbase = optimbase_stoplog ( this.optbase , ...
        sprintf ( "Test Stagnation : newfvmean=%e >= oldfvmean=%e - %e * %e" , newfvmean, oldfvmean , this.kelleyalpha , nsg ) );
      if ( newfvmean >= oldfvmean - this.kelleyalpha * nsg ) then
        terminate = 1;
        status = "kelleystagnation";
      end
    end
  end
endfunction
  

//
// neldermead_outputcmd --
//   Calls back user's output command
// Arguments
//   this : the current object
//   state : the state of the algorithm,
//     "init", "done", "iter"
//   simplex : the current simplex
//
function  neldermead_outputcmd ( this, ...
   state , simplex )
  outputcmd = optimbase_cget ( this.optbase , "-outputcommand" );
  if typeof(outputcmd) <> "string" then
    brutedata = optimbase_outstruct ( this.optbase );
    data = tlist(["T_NMDATA",...
      "x","fval","iteration","funccount",...
      "simplex"]);
    data.x = brutedata.x;
    data.fval = brutedata.fval;
    data.iteration = brutedata.iteration;
    data.funccount = brutedata.funccount;
    data.simplex = simplex;
    optimbase_outputcmd ( this.optbase , state , data );
  end
endfunction
//
// neldermead_storehistory --
//   Stores the history into the data structure.
// Arguments, input
//   this : current object
//   n : the number of unknown parameters
//   fopt : the current value of the function at optimum
//   xopt : arrary with size n, current optimum
//   xcoords : array with size n x n+1, coordinates of the n+1 vertices
//
function this = neldermead_storehistory ( this , n , fopt , xopt , xcoords )
  storehistory = optimbase_cget ( this.optbase , "-storehistory" );
  iterations = optimbase_get ( this.optbase , "-iterations" );
  if storehistory == 1 then
    this.optbase = optimbase_histset ( this.optbase , iterations , "-fopt" , fopt );
    this.optbase = optimbase_histset ( this.optbase , iterations , "-xopt" , xopt(1:n)' );
    this.historysimplex ( iterations , 1:n+1,1:n) = xcoords(1:n,1:n+1)';
  end
endfunction

//
// neldermead_istorestart --
//   Returns 1 if the optimization is to restart.
// Arguments
//   istorestart : 1 of the the optimization is to restart.
//
function [ this , istorestart ] = neldermead_istorestart ( this )
  select this.restartdetection
  case "oneill"
    [ this , istorestart ] = neldermead_isroneill ( this )
  case "kelley"
    [ this , istorestart ] = neldermead_isrkelley ( this )
  else
    errmsg = msprintf(gettext("%s: Unknown restart method %s"),"neldermead_istorestart", this.restartmethod)
    error(errmsg)
  end
endfunction
//
// neldermead_isrkelley--
//   Returns 1 if the optimization is to restart.
//   Use kelleystagnation as a criteria for restart.
// Arguments
//   istorestart : 1 of the the optimization is to restart.
//
function [ this , istorestart ] = neldermead_isrkelley ( this )
  istorestart = 0
  if ( this.kelleystagnationflag==1 ) then
    status = optimbase_get ( this.optbase , "-status" );
    if ( status =="kelleystagnation" ) then
       istorestart = 1
    end
  end
endfunction
//
// neldermead_isroneill --
//   Returns 1 if the optimization is to restart.
//   Use O'Neill method as a criteria for restart.
//   It is a axis by axis search for optimality.
// Arguments
//   xopt : the optimum, as a st of n values
//   fopt : function value at optimum
//   eps : a small value
//   step : a list of n values, representing
//     the "size" of each parameter
//   istorestart : 1 of the the optimization is to restart.
//
function [ this , istorestart ] = neldermead_isroneill ( this )
  n = optimbase_cget ( this.optbase , "-numberofvariables" );
  //
  // If required, make a vector step from the scalar step
  //
  defaultstep = this.restartstep;
  stepn = length ( defaultstep );
  if ( stepn <> n ) then
    step = defaultstep * ones(n,1);
  else
    step = defaultstep;
  end

  xopt = optimbase_get ( this.optbase , "-xopt" );
  fopt = optimbase_get ( this.optbase , "-fopt" );

    istorestart = 0
    for ix = 1:n
      stepix = step ( ix )
      del = stepix * this.restarteps
      if ( del==0.0 ) then
         del = eps
      end
      xoptix =  xopt ( ix )
      xopt ( ix ) = xoptix + del
      [ this , fv ] = neldermead_function ( this , xopt )
      if ( fv < fopt ) then
        istorestart = 1
        break
      end
      xopt ( ix ) = xoptix - del
      [ this , fv ] = neldermead_function ( this , xopt )
      if ( fv < fopt ) then
        istorestart = 1
        break
      end
      xopt ( ix ) = xoptix
    end
endfunction

//
// neldermead_startup --
//   Startup the algorithm.
//   Computes the initial simplex, depending on the -simplex0method.
//
function this = neldermead_startup (this)
  [ this.optbase , hasbounds ] = optimbase_hasbounds ( this.optbase );
  if ( hasbounds ) then
    [ this.optbase , isok ] = optimbase_checkbounds ( this.optbase );
    if ( ~isok ) then
      error ( msprintf(gettext("%s: Bounds are not consistent."), "neldermead_startup" ))
    end
  end
  x0 = optimbase_cget ( this.optbase , "-x0" );
  select this.simplex0method
  case "given" then
    [ simplex0 , this ] = optimsimplex_new ( this.coords0 , ...
      neldermead_costf , this );
  case "axes" then
    simplex0 = optimsimplex_new ( );
    [ simplex0 , this ] = optimsimplex_axes ( simplex0 , ...
      x0' , neldermead_costf , this.simplex0length , this );
  case "spendley" then
    simplex0 = optimsimplex_new ( );
    [ simplex0 , this ] = optimsimplex_spendley ( simplex0 , ...
      x0' , neldermead_costf , this.simplex0length , this );
  case "pfeffer" then
    simplex0 = optimsimplex_new ( );
    [ simplex0 , this ] = optimsimplex_pfeffer ( simplex0 , ...
      x0' , neldermead_costf , this.simplex0deltausual , ...
      this.simplex0deltazero , this );
  case "randbounds" then
    //
    // Initialize the random number generator, so that the results are always the
    // same.
    //
    rand("seed" , 0)
    simplex0 = optimsimplex_new ( );
    if ( this.boxnbpoints == "2n" ) then
      this.boxnbpointseff = 2 * this.optbase.numberofvariables;
    else
      this.boxnbpointseff = this.boxnbpoints;
    end
    if ( ~hasbounds ) then
      error ( msprintf(gettext("%s: Randomized bounds initial simplex is not available without bounds." ), "neldermead_startup"))
    end
    [ simplex0 , this ] = optimsimplex_randbounds ( simplex0 , x0' , ...
      neldermead_costf , this.optbase.boundsmin , this.optbase.boundsmax , ...
      this.boxnbpointseff  , this );
  else
    errmsg = msprintf(gettext("%s: Unknown -simplex0method : %s"), "neldermead_startup", this.simplex0method);
    error(errmsg);
  end
  //
  // Scale the simplex into the bounds and the nonlinear inequality constraints, if any
  //
  if ( hasbounds | this.optbase.nbineqconst > 0 ) then
    this = neldermead_log (this,sprintf("Scaling initial simplex into nonlinear inequality constraints..."));
    nbve = optimsimplex_getnbve ( simplex0 );
    for ive = 1 : nbve
      x = optimsimplex_getx ( simplex0 , ive );
      this = neldermead_log (this,sprintf("Scaling vertex #%d/%d at ["+...
        strcat(string(x)," ")+"]... " , ...
        ive , nbve ));
      [ this , status , xp ] = _scaleinconstraints ( this , x , x0 );
      if ( ~status ) then
        errmsg = msprintf(gettext("%s: Impossible to scale the vertex #%d/%d at [%s] into inequality constraints"), ...
          "neldermead_startup", ive , nbve , strcat(string(x)," "));
        error(errmsg);
      end
      if ( or ( x <> xp ) ) then
        [ this , fv ] = neldermead_function ( this , xp );
        simplex0 = optimsimplex_setve ( simplex0 , ive , fv , xp );
      end
    end
  end
  //
  // Store the simplex
  //
  this.simplex0 = optimsimplex_destroy ( this.simplex0 );
  this.simplex0 = simplex0;
  this.simplexsize0 = optimsimplex_size ( simplex0 );
  fx0 = optimsimplex_getfv ( this.simplex0 , 1 );
  this.optbase = optimbase_set ( this.optbase , "-fx0" , fx0 );
  this.optbase = optimbase_set ( this.optbase , "-xopt" , x0 );
  this.optbase = optimbase_set ( this.optbase , "-fopt" , fx0 );
  this.optbase = optimbase_set ( this.optbase , "-iterations" , 0 );
  if ( this.kelleystagnationflag == 1 ) then
    this = neldermead_kelleystag ( this );
  end
endfunction

//
// neldermead_kelleystag --
//   Initialize Kelley's stagnation detection system when normalization is required,
//   by computing kelleyalpha.
//   If the simplex gradient is zero, then
//   use alpha0 as alpha.
// Arguments
//   status : the status after the failing
//     optimization process
//   simplex : the simplex computed at the end of the failing
//     optimization process
//
function this = neldermead_kelleystag ( this )
    if this.kelleystagnationflag == 1 then
      if this.kelleynormalizationflag == 0 then
        this.kelleyalpha = this.kelleystagnationalpha0
      else
        [sg,this] = optimsimplex_gradientfv ( this.simplex0 , neldermead_costf , "forward" , this )
        nsg = sg' * sg
        sigma0 = optimsimplex_size ( this.simplex0 , "sigmaplus" )
        if nsg==0.0 then 
          this.kelleyalpha = this.kelleystagnationalpha0
        else
          this.kelleyalpha = this.kelleystagnationalpha0 * sigma0 / nsg
        end
      end
      this = neldermead_log (this,sprintf("Test Stagnation Kelley : alpha0 = %e", this.kelleyalpha));
    end
endfunction
  //
  // _scaleinconstraints --
  //   Given a point to scale and a reference point which satisfies the constraints, 
  //   scale the point towards the reference point until it satisfies all the constraints.
  //   Returns a list of key values with the following
  //   keys : -status 0/1 -x x, where status
  //   is 0 if the procedure has failed after -boxnbnlloops
  //   iterations.
  // Arguments
  //   x : the point to scale
  //   xref : the reference point
  //   status : %T or %F
  //   p : scaled point
  //
function [ this , status , p ] = _scaleinconstraints ( this , x , xref )
  p = x
  //
  // Project the point into the bounds
  //
  [ this.optbase , hasbounds ] = optimbase_hasbounds ( this.optbase );
  if ( hasbounds ) then
    [ this.optbase , p ] = optimbase_proj2bnds ( this.optbase ,  p );
    this = neldermead_log (this,sprintf(" > After projection into bounds p = [%s]" , ...
      strcat(string(p)," ")));
  end
  //
  // Adjust point to satisfy nonlinear inequality constraints
  //
  nbnlc = optimbase_cget ( this.optbase , "-nbineqconst" )
  if ( nbnlc == 0 ) then
    scaled = %T
  else
    scaled = %F
    //
    // Try the current point and see if the constraints are satisfied.
    // If not, move the point "halfway" to the centroid,
    // which should satisfy the constraints, if
    // the constraints are convex.
    // Perform this loop until the constraints are satisfied.
    // If all loops have been performed without success, the scaling
    // has failed.
    //
    for i = 1 : this.nbineqloops
      [ this , constlist ] = neldermead_function ( this , p , index=2 );
      feasible = %T
      for ic = 1 : this.optbase.nbineqconst;
        const = constlist ( ic );
        if ( const < 0.0 ) then
          this = neldermead_log (this,sprintf("Constraint #%d/%d is not satisfied", ...
            ic , this.optbase.nbineqconst ));
          feasible = %F;
          break;
        end
      end
      if ( feasible ) then
        scaled = %T;
        break;
      else
        this = neldermead_log (this,sprintf("Scaling inequality constraint at loop #%d/%d", ...
          i , this.nbineqloops));
        p = ( xref + p ) * this.ineqscaling;
      end
    end
    this = neldermead_log (this,sprintf(" > After scaling into inequality constraints p = [%s]" , ...
      strcat(string(p)," ") ) );
  end
  if ( scaled ) then
    status = %T
  else
    status = %F
    this = neldermead_log (this,sprintf(" > Impossible to scale into constraints after %d loops" , ...
      this.optbase.nbineqconst ));
  end
endfunction
//
// neldermead_box --
//   The Nelder-Mead algorithm, with variable-size simplex
//   and modifications by Box for bounds and
//   inequality constraints.
//
function this = neldermead_box ( this )
  //
  // Order the vertices for the first time
  //
  simplex = this.simplex0;
  n = optimbase_cget ( this.optbase , "-numberofvariables" );
  fvinitial = optimbase_get ( this.optbase , "-fx0" );
  // Sort function values and x points by increasing function value order
  this = neldermead_log (this,"Step #1 : order");
  simplex = optimsimplex_sort ( simplex );
  currentcenter = optimsimplex_center ( simplex );
  currentxopt = optimbase_cget ( this.optbase , "-x0" );
  newfvmean = optimsimplex_fvmean ( simplex );
  nbve = optimsimplex_getnbve ( simplex );
  ihigh = nbve;
  inext = ihigh - 1
  ilow = 1
  [ this.optbase , hasbounds ] = optimbase_hasbounds ( this.optbase );
  nbnlc = optimbase_cget ( this.optbase , "-nbineqconst" )
  //
  // Initialize
  //
  terminate = 0;
  iter = 0;
  //
  // Nelder-Mead Loop
  //
  while ( terminate == 0 )
    this.optbase = optimbase_incriter ( this.optbase );
    iter = iter + 1;
    xlow = optimsimplex_getx ( simplex , ilow )
    flow = optimsimplex_getfv ( simplex , ilow )
    xhigh = optimsimplex_getx ( simplex , ihigh )
    fhigh = optimsimplex_getfv ( simplex , ihigh )
    xn = optimsimplex_getx ( simplex , inext )
    fn = optimsimplex_getfv ( simplex , inext )
    //
    // Store history
    //
    xcoords = optimsimplex_getallx ( simplex )
    this = neldermead_storehistory ( this , n , flow , xlow , xcoords );
    deltafv = abs(optimsimplex_deltafvmax ( simplex ));
    currentfopt = flow;
    previousxopt = currentxopt;
    currentxopt = xlow;
    previouscenter = currentcenter;
    currentcenter = optimsimplex_center ( simplex );
    oldfvmean = newfvmean;
    newfvmean = optimsimplex_fvmean ( simplex );
    totaliter = optimbase_get ( this.optbase , "-iterations" );
    funevals = optimbase_get ( this.optbase , "-funevals" );
    ssize = optimsimplex_size ( simplex )
    this = neldermead_log (this,sprintf("================================================================="));
    this = neldermead_log (this,sprintf("Iteration #%d (total = %d)",iter,totaliter));
    this = neldermead_log (this,sprintf("Function Eval #%d",funevals));
    this = neldermead_log (this,sprintf("Xopt : [%s]",strcat(string(xlow)," ")));
    this = neldermead_log (this,sprintf("Fopt : %e",flow));
    this = neldermead_log (this,sprintf("DeltaFv : %e",deltafv));
    this = neldermead_log (this,sprintf("Center : [%s]",strcat(string(currentcenter)," ")));
    this = neldermead_log (this,sprintf("Size : %e",ssize));
    str = optimsimplex_tostring ( simplex )
    for i = 1:nbve
      this = neldermead_log (this,str(i));
    end
    neldermead_outputcmd ( this, "iter" , simplex )

    //
    // Update termination flag
    //
    if ( iter > 1 ) then
      [this , terminate , status] = neldermead_termination (this , ...
        fvinitial , oldfvmean , newfvmean , previouscenter , currentcenter , simplex );
      if (terminate==1) then
        this = neldermead_log (this,sprintf("Terminate with status : %s",status));
        break
      end
    end
    //
    // Compute xbar, center of better vertices
    //
    this = neldermead_log (this,sprintf("Reflect"));
    xbar = optimsimplex_xbar ( simplex );
    this = neldermead_log (this,sprintf("xbar=[%s]" , strcat(string(xbar)," ")));
    //
    // Reflect the worst point with respect to center
    //
    xr = neldermead_interpolate ( xbar , xhigh , this.rho );
    // Adjust point to satisfy bounds and nonlinear inequality constraints
    if ( hasbounds | nbnlc > 0 ) then
      [ this , status , xr ] = _scaleinconstraints ( this , xr , xbar )
      if ( ~status ) then
        status = "impossibleconstr"
        break
      end
    end
    [ this , fr ] = neldermead_function ( this , xr );
    this = neldermead_log (this,sprintf("xr=[%s], f(xr)=%f", strcat(string(xr)," ") , fr));
    if ( fr >= flow & fr < fn ) then
      this = neldermead_log (this,sprintf("  > Perform reflection"));
      simplex = optimsimplex_setve ( simplex , ihigh , fr , xr )
    elseif ( fr < flow ) then
      // Expand
      this = neldermead_log (this,sprintf("Expand"));
      xe = neldermead_interpolate ( xbar , xhigh , this.rho*this.chi );
      // Adjust point to satisfy bounds and nonlinear inequality constraints
      if ( hasbounds | nbnlc > 0 ) then
        [ this , status , xe ] = _scaleinconstraints ( this , xe , xbar )
        if ( ~status ) then
          status = "impossibleconstr"
          break
        end
      end
      [ this ,fe] = neldermead_function ( this ,xe);
      this = neldermead_log (this,sprintf("xe=[%s], f(xe)=%f", strcat(string(xe)," ") , fe ));
      if (fe < fr) then
        this = neldermead_log (this,sprintf("  > Perform Expansion"));
        simplex = optimsimplex_setve ( simplex , ihigh , fe , xe )
      else
        this = neldermead_log (this,sprintf("  > Perform reflection"));
        simplex = optimsimplex_setve ( simplex , ihigh , fr , xr )
      end
    elseif ( fr >= fn & fr < fhigh ) then
      // Outside contraction
      this = neldermead_log (this,sprintf("Contract - outside"));
      xc = neldermead_interpolate ( xbar , xhigh , this.rho*this.gamma );
      // Adjust point to satisfy bounds and nonlinear inequality constraints
      if ( hasbounds | nbnlc > 0 ) then
        [ this , status , xc ] = _scaleinconstraints ( this , xc , xbar )
        if ( ~status ) then
          status = "impossibleconstr"
          break
        end
      end
      [ this ,fc] = neldermead_function ( this ,xc);
      this = neldermead_log (this,sprintf("xc=[%s], f(xc)=%f", strcat(string(xc)," ") , fc));
      if ( fc <= fr ) then
        this = neldermead_log (this,sprintf("  > Perform Outside Contraction"));
        simplex = optimsimplex_setve ( simplex , ihigh , fc , xc )
      else
        //  Shrink
        this = neldermead_log (this,sprintf("  > Perform Shrink"));
        [ simplex , this ] = optimsimplex_shrink ( simplex , neldermead_costf , this.sigma , this );
      end
    else
      // ( fr >= fn & fr >= fhigh )  
      // Inside contraction
      this = neldermead_log (this,sprintf("Contract - inside"));
      xc = neldermead_interpolate ( xbar , xhigh , -this.gamma );
      // Adjust point to satisfy bounds and nonlinear inequality constraints
      if ( hasbounds | nbnlc > 0 ) then
        [ this , status , xc ] = _scaleinconstraints ( this , xc , xbar )
        if ( ~status ) then
          status = "impossibleconstr"
          break
        end
      end
      [ this ,fc] = neldermead_function ( this ,xc);
      this = neldermead_log (this,sprintf("xc=[%s], f(xc)=%f", strcat(string(xc)," ") , fc));
      if ( fc < fhigh ) then
        this = neldermead_log (this,sprintf("  > Perform Inside Contraction"));
        simplex = optimsimplex_setve ( simplex , ihigh , fc , xc )
      else
        //  Shrink
        this = neldermead_log (this,sprintf("  > Perform Shrink"));
        [ simplex , this ] = optimsimplex_shrink ( simplex , neldermead_costf , this.sigma , this )
      end
    end
    //
    // Sort simplex
    //
    this = neldermead_log (this,sprintf("Sort"));
    simplex  = optimsimplex_sort ( simplex );
  end
  this.optbase = optimbase_set ( this.optbase , "-xopt" , xlow );
  this.optbase = optimbase_set ( this.optbase , "-fopt" , flow );
  this.optbase = optimbase_set ( this.optbase , "-status" , status );
  this.simplexopt = simplex;
endfunction

