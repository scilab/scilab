// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt



//
// assert_close --
//   Returns 1 if the two real matrices computed and expected are close,
//   i.e. if the relative distance between computed and expected is lesser than epsilon.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_close ( computed, expected, epsilon )
  if expected==0.0 then
    shift = norm(computed-expected);
  else
    shift = norm(computed-expected)/norm(expected);
  end
  if shift < epsilon then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction
//
// assert_equal --
//   Returns 1 if the two real matrices computed and expected are equal.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_equal ( computed , expected )
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

//
//  Reference:
//
//    An extension of the simplex method to constrained
//    nonlinear optimization
//    M.B. Subrahmanyam
//    Journal of optimization theory and applications
//    Vol. 62, August 1989
//
//    Gould F.J.
//    Nonlinear Tolerance Programming
//    Numerical methods for Nonlinear optimization
//    Edited by F.A. Lootsma, pp 349-366, 1972

//
// optimtestcase --
//   Non linear inequality constraints are positive.
//    
// Arguments
//   x: the point where to compute the function
//   index : the stuff to compute
// Note
//  The following protocol is used
//  * if index=1, or no index, returns the value of the cost 
//    function (default case)
//  * if index=2, returns the value of the nonlinear inequality 
//    constraints, as a row array
//  * if index=3, returns an array which contains
//    at index #0, the value of the cost function  
//    at index #1 to the end is the list of the values of the nonlinear 
//    constraints
//  The inequality constraints are expected to be positive.
//
function result = optimtestcase ( x , index )
  if (~isdef('index','local')) then
    index = 1
  end
  if ( index == 1 | index == 3 ) then
    f = x(1)^2 + x(2)^2 + 2.0 * x(3)^2 + x(4)^2 ...
      - 5.0 * x(1) - 5.0 * x(2) - 21.0 * x(3) + 7.0 * x(4)
  end
  if ( index == 2 | index == 3 ) then
    c1 = - x(1)^2 - x(2)^2 - x(3)^2 - x(4)^2 ...
              - x(1) + x(2) - x(3) + x(4) + 8
    c2 = - x(1)^2 - 2.0 * x(2)^2 - x(3)^2 - 2.0 * x(4)^2 ...
              + x(1) + x(4) + 10.0
    c3 = - 2.0 * x(1)^2 - x(2)^2 - x(3)^2 - 2.0 * x(1) ...
              + x(2) + x(4) + 5.0
  end
  select index
  case 1 then
    result = f
  case 2 then
    result = [c1 c2 c3]
  case 3 then
    result = [f c1 c2 c3]
  else
    errmsg = sprintf("Unexpected index %d" , index);
    error(errmsg);
  end
endfunction
//
// neldermead_constraints --
//   The Nelder-Mead algorithm, with variable-size simplex
//   and modifications for bounds and
//   inequality constraints.
//
function this = neldermead_constraints ( this )
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
  terminate = %f;
  iter = 0;
  step = "init";
  //
  // Nelder-Mead Loop
  //
  while ( ~terminate )
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
    this = neldermead_log (this,sprintf("Xopt : [%s]",_strvec(xlow)));
    this = neldermead_log (this,sprintf("Fopt : %e",flow));
    this = neldermead_log (this,sprintf("DeltaFv : %e",deltafv));
    this = neldermead_log (this,sprintf("Center : [%s]",_strvec(currentcenter)));
    this = neldermead_log (this,sprintf("Size : %e",ssize));
    str = optimsimplex_tostring ( simplex )
    for i = 1:nbve
      this = neldermead_log (this,str(i));
    end
    neldermead_outputcmd ( this, "iter" , simplex , step )

    //
    // Update termination flag
    //
    if ( iter > 1 ) then
      [this , terminate , status] = neldermead_termination (this , ...
        fvinitial , oldfvmean , newfvmean , previouscenter , currentcenter , simplex );
      if ( terminate ) then
        this = neldermead_log (this,sprintf("Terminate with status : %s",status));
        break
      end
    end
    //
    // Compute xbar, center of better vertices
    //
    this = neldermead_log (this,sprintf("Reflect"));
    xbar = optimsimplex_xbar ( simplex );
    this = neldermead_log (this,sprintf("xbar=[%s]" , _strvec(xbar)));
    //
    // Reflect the worst point with respect to center
    //
    xr = neldermead_interpolate ( xbar , xhigh , this.rho );
    this = neldermead_log (this,sprintf("xr=[%s]" , _strvec(xr)));
    // Adjust point to satisfy bounds and nonlinear inequality constraints
    if ( hasbounds | nbnlc > 0 ) then
      [ this , status , xr ] = _scaleinboundsandcons ( this , xr , xbar );
      if ( ~status ) then
        status = "impossibleconstr"
        break
      end
    end
    [ this , fr ] = neldermead_function ( this , xr );
    this = neldermead_log (this,sprintf("xr=[%s], f(xr)=%f", _strvec(xr) , fr));
    if ( fr >= flow & fr < fn ) then
      this = neldermead_log (this,sprintf("  > Perform reflection"));
      simplex = optimsimplex_setve ( simplex , ihigh , fr , xr )
      step = "reflection";
    elseif ( fr < flow ) then
      // Expand
      this = neldermead_log (this,sprintf("Expand"));
      xe = neldermead_interpolate ( xbar , xhigh , this.rho*this.chi );
      // Adjust point to satisfy bounds and nonlinear inequality constraints
      if ( hasbounds | nbnlc > 0 ) then
        [ this , status , xe ] = _scaleinboundsandcons ( this , xe , xbar );
        if ( ~status ) then
          status = "impossibleconstr"
          break
        end
      end
      [ this , fe ] = neldermead_function ( this , xe );
      this = neldermead_log (this,sprintf("xe=[%s], f(xe)=%f", strcat(string(xe)," ") , fe ));
      if (fe < fr) then
        this = neldermead_log (this,sprintf("  > Perform Expansion"));
        simplex = optimsimplex_setve ( simplex , ihigh , fe , xe )
        step = "expansion";
      else
        this = neldermead_log (this,sprintf("  > Perform reflection"));
        simplex = optimsimplex_setve ( simplex , ihigh , fr , xr )
        step = "reflection";
      end
    elseif ( fr >= fn & fr < fhigh ) then
      // Outside contraction
      this = neldermead_log (this,sprintf("Contract - outside"));
      xc = neldermead_interpolate ( xbar , xhigh , this.rho*this.gamma );
      // Adjust point to satisfy bounds and nonlinear inequality constraints
      if ( hasbounds | nbnlc > 0 ) then
        [ this , status , xc ] = _scaleinboundsandcons ( this , xc , xbar );
        if ( ~status ) then
          status = "impossibleconstr"
          break
        end
      end
      [ this , fc ] = neldermead_function ( this , xc );
      this = neldermead_log (this,sprintf("xc=[%s], f(xc)=%f", strcat(string(xc)," ") , fc));
      if ( fc <= fr ) then
        this = neldermead_log (this,sprintf("  > Perform Outside Contraction"));
        simplex = optimsimplex_setve ( simplex , ihigh , fc , xc )
        step = "outsidecontraction";
      else
        //  Shrink
        this = neldermead_log (this,sprintf("  > Perform Shrink"));
        [ simplex , this ] = optimsimplex_shrink ( simplex , neldermead_costf , this.sigma , this );
        step = "shrink";
      end
    else
      // ( fr >= fn & fr >= fhigh )  
      // Inside contraction
      this = neldermead_log (this,sprintf("Contract - inside"));
      xc = neldermead_interpolate ( xbar , xhigh , -this.gamma );
      // Adjust point to satisfy bounds and nonlinear inequality constraints
      if ( hasbounds | nbnlc > 0 ) then
        [ this , status , xc ] = _scaleinboundsandcons ( this , xc , xbar );
        if ( ~status ) then
          status = "impossibleconstr"
          break
        end
      end
      [ this , fc ] = neldermead_function ( this , xc );
      this = neldermead_log (this,sprintf("xc=[%s], f(xc)=%f", strcat(string(xc)," ") , fc));
      if ( fc < fhigh ) then
        this = neldermead_log (this,sprintf("  > Perform Inside Contraction"));
        simplex = optimsimplex_setve ( simplex , ihigh , fc , xc )
        step = "insidecontraction";
      else
        //  Shrink
        this = neldermead_log (this,sprintf("  > Perform Shrink"));
        [ simplex , this ] = optimsimplex_shrink ( simplex , neldermead_costf , this.sigma , this )
        step = "shrink";
      end
    end
    //
    // Sort simplex
    //
    this = neldermead_log (this,sprintf("Sort"));
    simplex  = optimsimplex_sort ( simplex );
  end
  this.optbase = optimbase_set ( this.optbase , "-xopt" , xlow.' );
  this.optbase = optimbase_set ( this.optbase , "-fopt" , flow );
  this.optbase = optimbase_set ( this.optbase , "-status" , status );
  this.simplexopt = simplex;
endfunction

//
// Test with my own algorithm,
// the "Mega Super Ultra Modified Simplex Method" !!!
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase);
nm = neldermead_configure(nm,"-x0",[0.0 0.0 0.0 0.0]');
nm = neldermead_configure(nm,"-maxiter",200);
nm = neldermead_configure(nm,"-maxfunevals",400);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-3);
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-nbineqconst",3);
//nm = neldermead_configure(nm,"-verbose",1);
nm = neldermead_configure(nm,"-verbosetermination",1);
nm = neldermead_configure(nm,"-method","mine");
nm = neldermead_configure(nm,"-mymethod",neldermead_constraints);
nm = neldermead_search(nm);
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_close ( xopt , [0.0 1.0 2.0 -1.0]', 1e-3 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_close ( fopt , -44.0 , 1e-5 );
// Check status
status = neldermead_get(nm,"-status");
assert_equal ( status , "tolsize" );
nm = neldermead_destroy(nm);

