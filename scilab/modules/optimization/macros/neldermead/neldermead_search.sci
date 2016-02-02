// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
function this = neldermead_search ( this , warn)

    rhs = argn(2);

    warn_mode = warning("query"); // Saving current warning mode to revert to it at the end
    if rhs == 2 then
        if warn == 0 | warn == "off" then
            warning("off"); // Turn off warnings
        elseif warn == 1 | warn == "on" then
            warning("on"); // Turn on warnings
        else
            msg = _("%s: Wrong value for input argument #%d: ""%s"", ""%s"", %d or %d expected.\n");
            error(msprintf(msg, "neldermead_search", 2, "off", "on", 0, 1))
        end
    end

    withderivatives = optimbase_cget ( this.optbase , "-withderivatives" );
    if ( withderivatives ) then
        errmsg = msprintf(gettext("%s: The -withderivatives option is true but all algorithms in neldermead are derivative-free."), "neldermead_search")
        error(errmsg)
    end
    if ( ~this.startupflag ) then
        this = neldermead_startup ( this );
        this.startupflag = %t;
    end
    stop = neldermead_outputcmd ( this, "init" , this.simplex0 , "init" )
    if ( stop ) then
        this.optbase = optimbase_set ( this.optbase , "-status" , "userstop" );
        verbose = optimbase_cget ( this.optbase , "-verbose" )
        if ( verbose == 1 ) then
            this = neldermead_log (this,sprintf("Terminate by user''s request"));
        end
        return
    end

    if ( this.restartflag ) then
        this = neldermead_autorestart ( this )
    else
        this = neldermead_algo ( this );
    end
    stop = neldermead_outputcmd ( this, "done" , this.simplexopt , "done" )
    if ( stop ) then
        this.optbase = optimbase_set ( this.optbase , "-status" , "userstop" );
        verbose = optimbase_cget ( this.optbase , "-verbose" )
        if ( verbose == 1 ) then
            this = neldermead_log (this,sprintf("Terminate by user''s request"));
        end
    end

    warning(warn_mode); // Revert to the warning mode that was active before the call

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
    case "mine" then
        this = this.mymethod ( this );
    else
        errmsg = msprintf(gettext("%s: Unknown -method %s"), "neldermead_algo", this.method)
        error(errmsg)
    end
endfunction
//
// neldermead_autorestart --
//   Performs an optimization with automatic restart
// NOTE
//   The loop processes for i = 1 to restartmax PLUS 1
//   This is because a RE-start is performed after one simulation
//   has been performed, hence the "RE".
//   Hence,
//     * if restartmax = 0, the number of performed loops is 1.
//     * if restartmax = 1, the number of performed loops is 2.
//     * etc...
// Example #1
//   Sample session with restart max = 3 (the default) and process pass.
//   restartnb = 0
//   Try #1/4
//     Run
//     "Must restart"
//     restartnb = 1
//   Try #2/4
//     Run
//     "Must restart"
//     restartnb = 2
//   Try #3/4
//     Run
//     "Must not restart"
//   "Convergence reached after 2 restarts."
//   reached = %t
//   status = depending on the triggered termination criteria
//   restartnb = 2 (restarts are Tries #2 and #3)
// Example #2
//   Sample session with restart max = 3 (the default) and process fails.
//   restartnb = 0
//   Try #1/4
//     Run
//     "Must restart"
//     restartnb = 1
//   Try #2/4
//     Run
//     "Must restart"
//     restartnb = 2
//   Try #3/4
//     Run
//     "Must restart"
//     restartnb = 3
//   Try #4/4
//     Run
//     Must restart
//   "Convergence not reached after maximum 3 restarts."
//   reached = %f
//   status = "maxrestart"
//   restartnb = 3 (restarts are Tries #2, #3 and #4)
//
function this = neldermead_autorestart ( this )
    restartmax = this.restartmax;
    reached = %f;
    for iloop = 1: restartmax + 1
        this = neldermead_log (this,sprintf("*****************************************************************"));
        this = neldermead_log (this,sprintf("Try #%d/%d.", iloop , restartmax + 1 ));
        //
        // Run algorithm
        this = neldermead_algo ( this );
        //
        // Must we restart ?
        [ this , istorestart ] = neldermead_istorestart ( this );
        if ( istorestart ) then
            this = neldermead_log (this,"Must restart.");
        else
            this = neldermead_log (this,"Must not restart.");
        end
        if ( ~istorestart ) then
            reached = %t;
            break
        end
        if ( iloop < restartmax + 1 ) then
            // We are going to perform a restart
            this.restartnb = this.restartnb + 1;
            this = neldermead_log (this,"Updating simplex.");
            this = neldermead_updatesimp ( this );
        end
    end
    if ( reached ) then
        this = neldermead_log (this, sprintf ( "Convergence reached after %d restarts." , this.restartnb ) );
    else
        this = neldermead_log (this, sprintf ( "Convergence not reached after maximum %d restarts." , this.restartnb ) );
        this.optbase = optimbase_set ( this.optbase , "-status" , "maxrestart" );
    end
endfunction


//
// neldermead_variable --
//   The original Nelder-Mead algorithm, with variable-size simplex.
//
function this = neldermead_variable ( this )
    // Check settings correspond to algo
    [ this.optbase , hascons ] = optimbase_hasconstraints ( this.optbase );
    if ( hascons ) then
        errmsg = msprintf(gettext("%s: Problem has constraints, but variable algorithm ignores them."), "neldermead_variable")
        error(errmsg)
    end
    verbose = optimbase_cget ( this.optbase , "-verbose" )
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
    // Transpose, because optimsimplex returns row vectors
    currentcenter = optimsimplex_center ( simplex ).';
    currentxopt = optimbase_cget ( this.optbase , "-x0" );
    newfvmean = optimsimplex_fvmean ( simplex );
    greedy = this.greedy;
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
        // Transpose, because optimsimplex returns row vectors
        xlow = optimsimplex_getx ( simplex , 1 ).'
        flow = optimsimplex_getfv ( simplex , 1 )
        xhigh = optimsimplex_getx ( simplex , n+1 ).'
        fhigh = optimsimplex_getfv ( simplex , n+1 )
        xn = optimsimplex_getx ( simplex , n ).'
        fn = optimsimplex_getfv ( simplex , n )
        //
        // Store history





        //
        xcoords = optimsimplex_getallx ( simplex )
        this = neldermead_storehistory ( this , n , flow , xlow , xcoords );
        currentfopt = flow;
        previousxopt = currentxopt;
        currentxopt = xlow;
        previouscenter = currentcenter;
        currentcenter = optimsimplex_center ( simplex ).';
        oldfvmean = newfvmean;
        newfvmean = optimsimplex_fvmean ( simplex );
        if ( verbose == 1 ) then
            this = neldermead_logsummary ( this, iter,xlow,flow,currentcenter,simplex )
        end
        this.optbase = optimbase_set ( this.optbase , "-xopt" , xlow );
        this.optbase = optimbase_set ( this.optbase , "-fopt" , flow );
        stop = neldermead_outputcmd ( this, "iter" , simplex , step )
        if ( stop ) then
            status = "userstop"
            if ( verbose == 1 ) then
                this = neldermead_log (this,sprintf("Terminate by user''s request"));
            end
            break
        end

        //
        // Update termination flag
        //
        if ( iter > 1 ) then
            [ this , terminate , status ] = neldermead_termination (this , ..
            fvinitial , oldfvmean , newfvmean , previouscenter , currentcenter , simplex );
            if ( terminate ) then
                this = neldermead_log (this,sprintf("Terminate with status : %s",status));
                break
            end
        end
        //
        // Compute xbar, center of better vertices
        //
        if ( verbose == 1 ) then
            this = neldermead_log (this,sprintf("Reflect"));
        end
        // Transpose, because optimsimplex returns row vectors
        xbar   = optimsimplex_xbar ( simplex ).';
        if ( verbose == 1 ) then
            this = neldermead_log (this,sprintf("xbar="+_strvec(xbar)+""));
        end
        //
        // Reflect the worst point with respect to center
        //
        xr = neldermead_interpolate ( xbar , xhigh , this.rho );
        [ this.optbase , fr , index ] = optimbase_function ( this.optbase , xr , 2 );
        if ( verbose == 1 ) then
            this = neldermead_log (this,sprintf("xr=["+_strvec(xr)+"], f(xr)=%f",fr));
        end
        if ( fr >= flow & fr < fn ) then
            if ( verbose == 1 ) then
                this = neldermead_log (this,sprintf("  > Perform reflection"));
            end
            simplex = optimsimplex_setve ( simplex , n+1 , fr , xr.' )
            step = "reflection";
        elseif ( fr < flow ) then
            // Expand
            if ( verbose == 1 ) then
                this = neldermead_log (this,sprintf("Expand"));
            end
            xe = neldermead_interpolate ( xbar , xhigh , this.rho*this.chi );
            [ this.optbase , fe , index ] = optimbase_function ( this.optbase , xe , 2 );
            if ( verbose == 1 ) then
                this = neldermead_log (this,sprintf("xe="+_strvec(xe)+", f(xe)=%f",fe));
            end
            if ( greedy ) then
                if ( fe < flow ) then
                    if ( verbose == 1 ) then
                        this = neldermead_log (this,sprintf("  > Perform Greedy Expansion"));
                    end
                    simplex = optimsimplex_setve ( simplex , n+1 , fe , xe.' )
                    step = "expansion";
                else
                    if ( verbose == 1 ) then
                        this = neldermead_log (this,sprintf("  > Perform Greedy Reflection"));
                    end
                    simplex = optimsimplex_setve ( simplex , n+1 , fr , xr.' )
                    step = "reflection";
                end
            else
                if ( fe < fr ) then
                    if ( verbose == 1 ) then
                        this = neldermead_log (this,sprintf("  > Perform Expansion"));
                    end
                    simplex = optimsimplex_setve ( simplex , n+1 , fe , xe.' )
                    step = "expansion";
                else
                    if ( verbose == 1 ) then
                        this = neldermead_log (this,sprintf("  > Perform Reflection"));
                    end
                    simplex = optimsimplex_setve ( simplex , n+1 , fr , xr.' )
                    step = "reflection";
                end
            end
        elseif ( fr >= fn & fr < fhigh ) then
            // Outside contraction
            if ( verbose == 1 ) then
                this = neldermead_log (this,sprintf("Contract - outside"));
            end
            xc = neldermead_interpolate ( xbar , xhigh , this.rho*this.gamma );
            [ this.optbase , fc , index ] = optimbase_function ( this.optbase , xc , 2 );
            if ( verbose == 1 ) then
                this = neldermead_log (this,sprintf("xc="+_strvec(xc)+", f(xc)=%f",fc));
            end
            if ( fc <= fr ) then
                if ( verbose == 1 ) then
                    this = neldermead_log (this,sprintf("  > Perform Outside Contraction"));
                end
                simplex = optimsimplex_setve ( simplex , n+1 , fc , xc.' )
                step = "outsidecontraction";
            else
                //  Shrink
                if ( verbose == 1 ) then
                    this = neldermead_log (this,sprintf("  > Perform Shrink"));
                end
                [ simplex , this ] = optimsimplex_shrink ( simplex , costf_transposex , this.sigma , this );
                step = "shrink";
            end
        else
            // ( fr >= fn & fr >= fhigh )
            // Inside contraction
            if ( verbose == 1 ) then
                this = neldermead_log (this,sprintf("Contract - inside"));
            end
            xc = neldermead_interpolate ( xbar , xhigh , -this.gamma );
            [ this.optbase , fc , index ] = optimbase_function ( this.optbase , xc , 2 );
            if ( verbose == 1 ) then
                this = neldermead_log (this,sprintf("xc="+_strvec(xc)+", f(xc)=%f",fc));
            end
            if ( fc < fhigh ) then
                if ( verbose == 1 ) then
                    this = neldermead_log (this,sprintf("  > Perform Inside Contraction"));
                end
                simplex = optimsimplex_setve ( simplex , n+1 , fc , xc.' )
                step = "insidecontraction";
            else
                //  Shrink
                if ( verbose == 1 ) then
                    this = neldermead_log (this,sprintf("  > Perform Shrink"));
                end
                [ simplex , this ] = optimsimplex_shrink ( simplex , costf_transposex , this.sigma , this )
                step = "shrink";
            end
        end
        //
        // Sort simplex
        //
        if ( verbose == 1 ) then
            this = neldermead_log (this,sprintf("Sort"));
        end
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
// Implementation note:
//   We implement the following "rules" of the Spendley et al.
//   method.
//   Rule 1 is strictly applied, but the reflection is done
//   by reflection the high point, since we minimize a function
//   instead of maximizing it, like Spendley.
//   The Rule 2 is NOT implemented, as we expect that the
//   function evaluation is not subject to errors.
//   The Rule 3 is applied, ie reflection with respect
//   to next to high point.
//   A shrink step is included, with shrinkage factor sigma.
//
//   "Rule 1. Ascertain the lowest reading y, of yi ... Yk+1
//   Complete a new simplex Sp by excluding the point Vp corresponding to
//   y, and replacing it by V* defined as above."
//   "Rule 2. If a result has occurred in (k + 1) successive simplexes, and is not
//   then eliminated by application of Rule 1, do not move in the direction
//   indicated by Rule 1, or at all, but discard the result and replace it by a new
//   observation at the same point."
//   "Rule 3. If y is the lowest reading in So , and if the next observation made,
//   y* , is the lowest reading in the new simplex S , do not apply Rule 1 and
//   return to So from Sp . Move out of S, by rejecting the second lowest reading
//   (which is also the second lowest reading in So)."
//
function this = neldermead_fixed (this)
    // Check settings correspond to algo
    [ this.optbase , hascons ] = optimbase_hasnlcons ( this.optbase );
    if ( hascons ) then
        errmsg = msprintf(gettext("%s: Problem has constraints, but fixed algorithm ignores them."), "neldermead_fixed")
        error(errmsg)
    end
    verbose = optimbase_cget ( this.optbase , "-verbose" )
    //
    // Order the vertices for the first time
    //
    simplex = this.simplex0;
    n = optimbase_cget ( this.optbase , "-numberofvariables" );
    fvinitial = optimbase_get ( this.optbase , "-fx0" );
    // Sort function values and x points by increasing function value order
    this = neldermead_log (this,sprintf("Sort"));
    simplex = optimsimplex_sort ( simplex );
    //
    // Compute center of simplex
    //
    // Transpose, because optimsimplex returns row vectors
    currentcenter = optimsimplex_center ( simplex ).';
    newfvmean = optimsimplex_fvmean ( simplex );
    currentxopt = optimbase_cget ( this.optbase , "-x0" );
    //
    // Set indices for "clarity"
    //
    ilow = 1
    ihigh = n + 1
    inext = n
    //
    // Initialize
    //
    terminate = %f;
    iter = 0;
    step = "init";
    //
    // main N-M loop
    //
    while ( ~terminate )
        this.optbase = optimbase_incriter ( this.optbase );
        iter = iter + 1;
        xlow = optimsimplex_getx ( simplex , ilow ).'
        flow = optimsimplex_getfv ( simplex , ilow )
        xhigh = optimsimplex_getx ( simplex , ihigh ).'
        fhigh = optimsimplex_getfv ( simplex , ihigh )
        //
        // Store history
        //
        xcoords = optimsimplex_getallx ( simplex )
        this = neldermead_storehistory ( this , n , flow , xlow , xcoords );
        currentfopt = flow;
        previousxopt = currentxopt;
        currentxopt = xlow;
        previouscenter = currentcenter;
        currentcenter = optimsimplex_center ( simplex ).';
        oldfvmean = newfvmean;
        newfvmean = optimsimplex_fvmean ( simplex );
        if ( verbose == 1 ) then
            this = neldermead_logsummary ( this, iter,xlow,flow,currentcenter,simplex )
        end
        this.optbase = optimbase_set ( this.optbase , "-xopt" , xlow );
        this.optbase = optimbase_set ( this.optbase , "-fopt" , flow );
        stop = neldermead_outputcmd ( this, "iter" , simplex , step )
        if ( stop ) then
            status = "userstop"
            if ( verbose == 1 ) then
                this = neldermead_log (this,sprintf("Terminate by user''s request"));
            end
            break
        end
        //
        // Update termination flag
        //
        if ( iter > 1 ) then
            [ this , terminate , status] = neldermead_termination (this , ..
            fvinitial , oldfvmean , newfvmean , previouscenter , currentcenter , simplex );
            if ( terminate ) then
                if ( verbose == 1 ) then
                    this = neldermead_log (this,sprintf("Terminate with status : %s",status));
                end
                break;
            end
        end
        //
        // Compute xbar, center of better vertices
        //
        if ( verbose == 1 ) then
            this = neldermead_log (this,sprintf("Reflect"));
        end
        xbar = optimsimplex_xbar ( simplex ).';
        if ( verbose == 1 ) then
            this = neldermead_log (this,sprintf("xbar="+_strvec(xbar)+""));
        end
        //
        // Reflect the worst point with respect to center
        //
        xr = neldermead_interpolate ( xbar , xhigh , this.rho );
        [ this.optbase , fr , index ] = optimbase_function ( this.optbase , xr , 2 );
        if ( verbose == 1 ) then
            this = neldermead_log (this,sprintf("xr="+_strvec(xr)+", f(xr)=%f",fr));
        end
        //
        // Replace worst point by xr if it is better
        //
        if ( fr < fhigh ) then
            if ( verbose == 1 ) then
                this = neldermead_log (this,sprintf("  > Perform reflect"));
            end
            simplex = optimsimplex_setve ( simplex , ihigh , fr , xr.' )
            step = "reflection";
        else
            // Reflect / xnext
            xnext = optimsimplex_getx ( simplex , inext ).';
            fnext = optimsimplex_getfv ( simplex , inext );
            xbar2 = optimsimplex_xbar ( simplex , inext ).';
            if ( verbose == 1 ) then
                this = neldermead_log (this,sprintf("xbar2="+_strvec(xbar2)+""));
            end
            xr2 = neldermead_interpolate ( xbar2 , xnext , this.rho );
            [ this.optbase , fr2 , index ] = optimbase_function ( this.optbase , xr2 , 2 );
            if ( verbose == 1 ) then
                this = neldermead_log (this,sprintf("xr2="+_strvec(xr2)+", f(xr2)=%f",fr2));
            end
            if ( fr2 < fnext ) then
                if ( verbose == 1 ) then
                    this = neldermead_log (this,sprintf("  > Perform reflect / next"));
                end
                simplex = optimsimplex_setve ( simplex , inext , fr2 , xr2.' )
                step = "reflectionnext";
            else
                //  Shrink
                if ( verbose == 1 ) then
                    this = neldermead_log (this,sprintf("  > Perform Shrink"));
                end
                [ simplex , this ] = optimsimplex_shrink ( simplex , costf_transposex , this.sigma , this )
                step = "shrink";
            end
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
//   terminate : %t if the algorithm terminates, %f if the algorithm must continue.
//   this.status : termination status
//     "continue"
//     "maxiter"
//     "maxfuneval"
//     "tolf"
//     "tolx"
//     "tolsize"
//     "tolsizedeltafv"
//     "kelleystagnation"
//     "tolboxf"
//     "tolvariance"
// Notes
//   Use the function average on the simplex instead of the best function value.
//   This is because the function average changes at each iteration.
//   Instead, the best function value as a step-by-step evolution and may not
//   change in 2 iterations, leading to astop of the algorithm.
// TODO : set the fvinitial, oldfvmean, newfvmean.
//
function [ this , terminate , status ] = neldermead_termination (this , ..
    fvinitial , oldfvmean , newfvmean , previousxopt , currentxopt , ..
    simplex )
    terminate = %f;
    status = "continue";
    verbose = optimbase_cget ( this.optbase , "-verbose" )
    //
    // Termination Criteria from parent optimization class
    //
    [ this.optbase , terminate , status ] = optimbase_terminate ( this.optbase , ..
    fvinitial , newfvmean , previousxopt , currentxopt );
    //
    // Criteria #6 : simplex absolute + relative size
    //
    if ( ~terminate ) then
        if ( this.tolsimplexizemethod ) then
            ssize = optimsimplex_size ( simplex , "sigmaplus" );
            tolsa = this.tolsimplexizeabsolute;
            tolsr = this.tolsimplexizerelative;
            ssize0 = this.simplexsize0;
            if ( verbose == 1 ) then
                this.optbase = optimbase_stoplog  ( this.optbase,sprintf("  > simplex size=%s < %s + %s * %s",..
                string(ssize), string(tolsa) , string(tolsr) , string(ssize0) ));
            end
            if ( ssize < tolsa + tolsr * ssize0 ) then
                terminate = %t;
                status = "tolsize";
            end
        end
    end
    //
    // Criteria #7 : simplex absolute size + difference in function values (Matlab-like)
    //
    if ( ~terminate ) then
        if ( this.tolssizedeltafvmethod ) then
            ssize = optimsimplex_size ( simplex , "sigmaplus" );
            if ( verbose == 1 ) then
                this.optbase = optimbase_stoplog  ( this.optbase,sprintf("  > simplex size=%s < %s",..
                string(ssize), string(this.tolsimplexizeabsolute)));
            end
            shiftfv = abs(optimsimplex_deltafvmax( simplex ))
            if ( verbose == 1 ) then
                this.optbase = optimbase_stoplog  ( this.optbase,sprintf("  > abs(fv(n+1) - fv(1))=%s < toldeltafv=%s",..
                string(shiftfv), string(this.toldeltafv)));
            end
            if ( ( ssize < this.tolsimplexizeabsolute ) & ( shiftfv < this.toldeltafv ) ) then
                terminate = %t;
                status = "tolsizedeltafv";
            end
        end
    end
    //
    // Criteria #8 : Kelley stagnation, based on
    // a sufficient decrease condition
    //
    if ( ~terminate ) then
        if ( this.kelleystagnationflag ) then
            [ sg , this ] = optimsimplex_gradientfv ( simplex , neldermead_costf , "forward" , this );
            nsg = sg.' * sg;
            if ( verbose == 1 ) then
                sgstr = _strvec(sg);
                this.optbase = optimbase_stoplog ( this.optbase , sprintf ( "Test Stagnation : nsg = %s, sg = [%s]", ..
                string(nsg) , sgstr ) );
                this.optbase = optimbase_stoplog ( this.optbase , ..
                sprintf ( "Test Stagnation : newfvmean=%s >= oldfvmean=%s - %s * %s" , ..
                string(newfvmean), string(oldfvmean) , string(this.kelleyalpha) , string(nsg) ) );
            end
            if ( newfvmean >= oldfvmean - this.kelleyalpha * nsg ) then
                terminate = %t;
                status = "kelleystagnation";
            end
        end
    end
    //
    // Criteria #9 : Box termination criteria
    // The number of consecutive time that an absolute tolerance on
    // function value is met.
    // From Algorithm 454, the tolerance is the difference between the
    // max and the min function values in the simplex.
    //
    if ( ~terminate ) then
        if ( this.boxtermination ) then
            shiftfv = abs(optimsimplex_deltafvmax( simplex ))
            if ( verbose == 1 ) then
                this.optbase = optimbase_stoplog ( this.optbase , ..
                sprintf ( "Test Box : shiftfv=%s < boxtolf=%s" , ..
                string(shiftfv) , string(this.boxtolf) ) );
            end
            if ( shiftfv < this.boxtolf ) then
                this.boxkount = this.boxkount + 1
                if ( verbose == 1 ) then
                    this.optbase = optimbase_stoplog ( this.optbase , ..
                    sprintf ( "Test Box : boxkount=%d == boxnbmatch=%d" , ..
                    this.boxkount , this.boxnbmatch ) );
                end
                if ( this.boxkount == this.boxnbmatch ) then
                    terminate = %t
                    status = "tolboxf"
                end
            else
                this.boxkount = 0
            end
        end
    end
    //
    // Obsolete option: maintain for backward compatibility
    // Criteria #10 : variance of function values
    //
    if ( ~terminate ) then
        if ( this.tolvarianceflag ) then
            var = optimsimplex_fvvariance ( simplex )
            if ( verbose == 1 ) then
                this.optbase = optimbase_stoplog ( this.optbase , ..
                sprintf ( "Test tolvariance : %s < %s" , ..
                string(var) , string(this.tolabsolutevariance) ) );
            end
            if ( var < this.tolrelativevariance * this.variancesimplex0 + this.tolabsolutevariance ) then
                terminate = %t
                status = "tolvariance"
            end
        end
    end
    //
    // Obsolete option: maintain for backward compatibility
    // Criteria #11 : user-defined criteria
    //
    if ( ~terminate ) then
        if ( this.myterminateflag ) then
            [ this , term , stat ] = this.myterminate ( this , simplex )
            if ( term ) then
                terminate = term
                status = stat
            end
        end
    end
    //
    // A verbose message
    //
    if ( verbose == 1 ) then
        this.optbase = optimbase_stoplog (this.optbase,sprintf("  > Terminate = %s, status = %s",..
        string(terminate) , status ));
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
//   step : the type of step performed during the iteration
//     "init", "done", "reflection", "expansion", "insidecontraction", "outsidecontraction"
//     "reflectionnext", "shrink"
//   stop : set to true to stop algorithm
//
function stop = neldermead_outputcmd ( this, ..
    state , simplex , step )
    outputcmd = optimbase_cget ( this.optbase , "-outputcommand" );
    if typeof(outputcmd) <> "string" then
        brutedata = optimbase_outstruct ( this.optbase );
        data = tlist(["T_NMDATA",..
        "x","fval","iteration","funccount",..
        "simplex" , "step" ]);
        data.x = brutedata.x;
        data.fval = brutedata.fval;
        data.iteration = brutedata.iteration;
        data.funccount = brutedata.funccount;
        data.simplex = simplex;
        data.step = step;
        stop = optimbase_outputcmd ( this.optbase , state , data );
    else
        stop = %f
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
    if ( storehistory ) then
        this.optbase = optimbase_histset ( this.optbase , iterations , "-fopt" , fopt );
        this.optbase = optimbase_histset ( this.optbase , iterations , "-xopt" , xopt(1:n) );
        this.historysimplex ( iterations , 1:n+1,1:n) = xcoords(1:n+1,1:n);
    end
endfunction

//
// neldermead_istorestart --
//   Returns 1 if the optimization is to restart.
// Arguments
//   istorestart : %t of the optimization is to restart.
//
function [ this , istorestart ] = neldermead_istorestart ( this )
    status = optimbase_get ( this.optbase , "-status" );
    if ( status =="maxfuneval" ) then
        istorestart = %f
        return
    end
    select this.restartdetection
    case "oneill"
        [ this , istorestart ] = neldermead_isroneill ( this )
    case "kelley"
        [ this , istorestart ] = neldermead_isrkelley ( this )
    else
        errmsg = msprintf(gettext("%s: Unknown restart detection %s"),"neldermead_istorestart", this.restartdetection)
        error(errmsg)
    end
endfunction
//
// neldermead_isrkelley--
//   Returns 1 if the optimization is to restart.
//   Use kelleystagnation as a criteria for restart.
// Arguments
//   istorestart : %t of the optimization is to restart.
//
function [ this , istorestart ] = neldermead_isrkelley ( this )
    istorestart = %f
    if ( this.kelleystagnationflag ) then
        status = optimbase_get ( this.optbase , "-status" );
        if ( status =="kelleystagnation" ) then
            istorestart = %t
        end
    end
endfunction
//
// neldermead_isroneill --
//   Returns 1 if the optimization is to restart.
//   Use O'Neill method as a criteria for restart.
//   It is an axis by axis search for optimality.
// Arguments
//   xopt : the optimum, as a st of n values
//   fopt : function value at optimum
//   eps : a small value
//   step : a list of n values, representing
//     the "size" of each parameter
//   istorestart : %t if the optimization is to restart.
//
function [ this , istorestart ] = neldermead_isroneill ( this )
    n = optimbase_cget ( this.optbase , "-numberofvariables" );
    //
    // If required, make a vector step from the scalar step
    //
    defaultstep = this.restartstep;
    steprows = size ( defaultstep , "r" );
    if ( steprows == 1 ) then
        step = defaultstep * ones(n,1);
    else
        step = defaultstep;
    end
    restarteps = this.restarteps;

    x = optimbase_get ( this.optbase , "-xopt" );
    fopt = optimbase_get ( this.optbase , "-fopt" );
    verbose = optimbase_cget ( this.optbase , "-verbose" )

    if ( verbose ) then
        this = neldermead_log (this,sprintf("================================================================="));
        this = neldermead_log (this, sprintf ( "O''Neill Restart\n") );
        this = neldermead_log (this, sprintf ( "Using step [%s]" , _strvec(step) ) );
    end

    istorestart = %f
    for ix = 1:n
        stepix = step ( ix )
        del = stepix * restarteps
        xix =  x ( ix )
        x ( ix ) = xix + del
        [ this.optbase , fv , index ] = optimbase_function ( this.optbase , x , 2 )
        if ( fv < fopt ) then
            istorestart = %t
            if ( verbose ) then
                this = neldermead_log (this, sprintf ( "Must restart because fv=%s at [%s] is lower than fopt=%s" , ..
                string(fv) , _strvec(x) , string(fopt)) );
            end
            break
        end
        x ( ix ) = xix - del
        [ this.optbase , fv , index ] = optimbase_function ( this.optbase , x , 2 )
        if ( fv < fopt ) then
            istorestart = %t
            if ( verbose ) then
                this = neldermead_log (this, sprintf( "Must restart because fv=%s at [%s] is lower than fopt=%s" , ..
                string(fv) , _strvec(x) , string(fopt)) );
            end
            break
        end
        x ( ix ) = xix
    end
endfunction

//
// neldermead_startup --
//   Startup the algorithm.
//   Computes the initial simplex, depending on the -simplex0method.
//
function this = neldermead_startup (this)
    //
    // 0. Check that the cost function is correctly connected
    // Note: this call to the cost function is not used, but helps the
    // user while he is tuning his object.
    if ( this.checkcostfunction ) then
        this.optbase = optimbase_checkcostfun ( this.optbase );
    end
    //
    // 1. If the problem has bounds, check that they are consistent
    [ this.optbase , hasbounds ] = optimbase_hasbounds ( this.optbase );
    if ( hasbounds ) then
        this.optbase = optimbase_checkbounds ( this.optbase );
    end
    //
    // 2. Get the initial guess and compute the initial simplex
    x0 = optimbase_cget ( this.optbase , "-x0" );
    select this.simplex0method
    case "given" then
        [ simplex0 , this ] = optimsimplex_new ( this.coords0 , [] , this );
    case "axes" then
        [ simplex0 , this ] = optimsimplex_new ( "axes" , ..
        x0.' , [] , this.simplex0length , this );
    case "spendley" then
        [ simplex0 , this ] = optimsimplex_new ( "spendley" , ..
        x0.' , [] , this.simplex0length , this );
    case "pfeffer" then
        [ simplex0 , this ] = optimsimplex_new ( "pfeffer" , ..
        x0.' , [] , this.simplex0deltausual , ..
        this.simplex0deltazero , this );
    case "randbounds" then
        if ( this.boxnbpoints == "2n" ) then
            this.boxnbpointseff = 2 * this.optbase.numberofvariables;
        else
            this.boxnbpointseff = this.boxnbpoints;
        end
        if ( ~hasbounds ) then
            error ( msprintf(gettext("%s: Randomized bounds initial simplex is not available without bounds." ), "neldermead_startup"))
        end
        [ simplex0 , this ] = optimsimplex_new ( "randbounds" , x0.' , ..
        [] , this.optbase.boundsmin , this.optbase.boundsmax , ..
        this.boxnbpointseff  , this );
    else
        errmsg = msprintf(gettext("%s: Unknown value %s for -simplex0method option"), "neldermead_startup", this.simplex0method);
        error(errmsg);
    end
    //
    // 3. Scale the initial simplex into the bounds and the nonlinear inequality constraints, if any
    [ this.optbase , hasnlcons ] = optimbase_hasnlcons ( this.optbase );
    if ( hasbounds | hasnlcons ) then
        // Check that initial guess is feasible
        [ this.optbase , isfeasible ] = optimbase_isfeasible ( this.optbase , x0 );
        if ( isfeasible <> 1 ) then
            error ( msprintf ( gettext ( "%s: Initial guess [%s] is not feasible." ) , "neldermead_startup" , _strvec ( x0 ) ) )
        end
        this = neldermead_log (this,sprintf("Scaling initial simplex into nonlinear inequality constraints..."));
        select this.scalingsimplex0
        case "tox0" then
            [ this , simplex0 ] = neldermead_scaletox0 ( this , simplex0 );
        case "tocenter" then
            [ this , simplex0 ] = neldermead_scaletocenter ( this , simplex0 );
        else
            errmsg = msprintf(gettext("%s: Unknown value %s for -scalingsimplex0 option"),"neldermead_startup", this.scalingsimplex0 );
            error(errmsg);
        end
    end
    //
    // 4. Compute the function values
    nbve = optimsimplex_getnbve ( simplex0 );
    for ive = 1 : nbve
        // Transpose, because optimsimplex returns row vectors
        x = optimsimplex_getx ( simplex0 , ive ).';
        if ( hasnlcons ) then
            [ this.optbase , fv , c , index ] = optimbase_function ( this.optbase , x , 2 );
        else
            [ this.optbase , fv , index ] = optimbase_function ( this.optbase , x , 2 );
        end
        // Transpose xp, which is a column vector
        simplex0 = optimsimplex_setve ( simplex0 , ive , fv , x.' );
    end
    //
    // 5. Store the simplex
    this.simplex0 = optimsimplex_destroy ( this.simplex0 );
    this.simplex0 = simplex0;
    this.simplexsize0 = optimsimplex_size ( simplex0 );
    //
    // 6. Store initial data into the base optimization component
    fx0 = optimsimplex_getfv ( this.simplex0 , 1 );
    this.optbase = optimbase_set ( this.optbase , "-fx0" , fx0 );
    this.optbase = optimbase_set ( this.optbase , "-xopt" , x0 );
    this.optbase = optimbase_set ( this.optbase , "-fopt" , fx0 );
    this.optbase = optimbase_set ( this.optbase , "-iterations" , 0 );
    //
    // 7. Initialize the termination criteria
    this = neldermead_termstartup ( this );
endfunction
//
// neldermead_scaletox0 --
//   Scale the simplex into the
//   nonlinear inequality constraints, if any.
//   Scale toward x0, which is feasible.
//   Do not update the function values.
// Arguments
//   simplex0 : the initial simplex
//
function [ this , simplex0 ] = neldermead_scaletox0 ( this , simplex0 )
    [ this.optbase , hasnlcons ] = optimbase_hasnlcons ( this.optbase );
    nbve = optimsimplex_getnbve ( simplex0 );
    x0 = optimbase_cget ( this.optbase , "-x0" );
    for ive = 2 : nbve
        // Transpose, because optimsimplex returns row vectors
        x = optimsimplex_getx ( simplex0 , ive ).';
        this = neldermead_log (this,sprintf("Scaling vertex #%d/%d at ["+..
        _strvec(x)+"]... " , ive , nbve ));
        // Transpose x into a row vector
        [ this , status , xp ] = _scaleinconstraints ( this , x , x0 );
        if ( ~status ) then
            lclmsg = gettext("%s: Impossible to scale the vertex #%d/%d at [%s] into inequality constraints")
            errmsg = msprintf(lclmsg, ..
            "neldermead_startup", ive , nbve , _strvec(x));
            error(errmsg);
        end
        if ( or ( x <> xp ) ) then
            // Transpose xp, which is a column vector
            simplex0 = optimsimplex_setx ( simplex0 , ive , xp.' );
        end
    end
endfunction
//
// neldermead_scaletocenter --
//   Scale the simplex into the
//   nonlinear inequality constraints, if any.
//   Scale to the centroid of the points
//   which satisfy the constraints.
//   Do not update the function values.
// Notes
//   This is Box's method for scaling.
//   It is unsure, since the centroid of the points
//   which satisfy the constraints may not be feasible.
// Arguments
//
//
function [ this , simplex0 ] = neldermead_scaletocenter ( this , simplex0 , x0 )
    [ this.optbase , hasnlcons ] = optimbase_hasnlcons ( this.optbase );
    nbve = optimsimplex_getnbve ( simplex0 );
    for ive = 2 : nbve
        xref = optimsimplex_xbar ( simplex0 , ive:nbve ).';
        // Transpose, because optimsimplex returns row vectors
        x = optimsimplex_getx ( simplex0 , ive ).';
        this = neldermead_log (this,sprintf("Scaling vertex #%d/%d at ["+..
        _strvec(x)+"]... " , ive , nbve ));
        // Transpose x into a row vector
        [ this , status , xp ] = _scaleinconstraints ( this , x , xref );
        if ( ~status ) then
            errmsg = msprintf(gettext("%s: Impossible to scale the vertex #%d/%d at [%s] into inequality constraints"), ..
            "neldermead_startup", ive , nbve , _strvec(x));
            error(errmsg);
        end
        if ( or ( x <> xp ) ) then
            // Transpose xp, which is a column vector
            simplex0 = optimsimplex_setx ( simplex0 , ive , xp.' );
        end
    end
endfunction
//
// neldermead_termstartup --
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
function this = neldermead_termstartup ( this )
    //
    // Criteria #8 : Kelley stagnation, based on
    // a sufficient decrease condition
    //
    if ( this.kelleystagnationflag ) then
        if ( ~this.kelleynormalizationflag ) then
            this.kelleyalpha = this.kelleystagnationalpha0
        else
            [sg,this] = optimsimplex_gradientfv ( this.simplex0 , neldermead_costf , "forward" , this )
            nsg = sg.' * sg
            sigma0 = optimsimplex_size ( this.simplex0 , "sigmaplus" )
            if nsg==0.0 then
                this.kelleyalpha = this.kelleystagnationalpha0
            else
                this.kelleyalpha = this.kelleystagnationalpha0 * sigma0 / nsg
            end
        end
        this = neldermead_log (this,sprintf("Test Stagnation Kelley : alpha0 = %s", string(this.kelleyalpha)));
    end
    //
    // Criteria #10 : variance of function values
    //
    if ( this.tolvarianceflag ) then
        this.variancesimplex0 = optimsimplex_fvvariance ( this.simplex0 )
    end
endfunction
//
// _scaleinconstraints --
//   Given a point to scale and a reference point which satisfies the constraints,
//   scale the point towards the reference point until it satisfies all the constraints.
//   Returns isscaled = %T if the procedure has succeded before -boxnbnlloops
//   Returns isscaled = %F if the procedure has failed after -boxnbnlloops
//   iterations.
// Arguments
//   x : the point to scale
//   xref : the reference point
//   isscaled : %T or %F
//   p : scaled point
//
function [ this , isscaled , p ] = _scaleinconstraints ( this , x , xref )
    p = x
    [ this.optbase , hasbounds ] = optimbase_hasbounds ( this.optbase );
    nbnlc = optimbase_cget ( this.optbase , "-nbineqconst" )
    //
    // 1. No bounds, no nonlinear inequality constraints
    // => no problem
    //
    if ( ( hasbounds == %f ) & ( nbnlc == 0 ) ) then
        isscaled = %T
        return;
    end
    //
    // 2. Scale into bounds
    //
    if ( hasbounds ) then
        [ this.optbase , p ] = optimbase_proj2bnds ( this.optbase ,  p );
        this = neldermead_log (this,sprintf(" > After projection into bounds p = [%s]" , ..
        _strvec(p)));
    end
    //
    // 3. Scale into non linear constraints
    // Try the current point and see if the constraints are satisfied.
    // If not, move the point "halfway" to the centroid,
    // which should satisfy the constraints, if
    // the constraints are convex.
    // Perform this loop until the constraints are satisfied.
    // If all loops have been performed without success, the scaling
    // has failed.
    //
    isscaled = %F
    alpha = 1.0
    p0 = p
    while ( alpha > this.guinalphamin )
        [ this.optbase , feasible ] = optimbase_isinnonlincons ( this.optbase , p );
        if ( feasible ) then
            isscaled = %T;
            break;
        end
        alpha = alpha * this.boxineqscaling
        this = neldermead_log (this,sprintf("Scaling inequality constraint with alpha = %s", ..
        string(alpha)));
        p = ( 1.0 - alpha ) * xref + alpha * p0;
    end
    this = neldermead_log (this,sprintf(" > After scaling into inequality constraints p = [%s]" , ..
    _strvec(p) ) );
    if ( ~isscaled ) then
        this = neldermead_log (this,sprintf(" > Impossible to scale into constraints after %d loops" , ..
        this.optbase.nbineqconst ));
    end
endfunction
//
// neldermead_logsummary --
//   At each iteration, prints this iteration summary.
//
function this = neldermead_logsummary ( this, iter,xlow,flow,currentcenter,simplex )
    deltafv = abs(optimsimplex_deltafvmax ( simplex ));
    totaliter = optimbase_get ( this.optbase , "-iterations" );
    funevals = optimbase_get ( this.optbase , "-funevals" );
    ssize = optimsimplex_size ( simplex )
    this = neldermead_log (this,sprintf("================================================================="));
    this = neldermead_log (this,sprintf("Iteration #%d (total = %d)",iter,totaliter));
    this = neldermead_log (this,sprintf("Function Eval #%d",funevals));
    this = neldermead_log (this,sprintf("Xopt : [%s]",_strvec(xlow)));
    this = neldermead_log (this,sprintf("Fopt : %s",string(flow)));
    this = neldermead_log (this,sprintf("DeltaFv : %s",string(deltafv)));
    this = neldermead_log (this,sprintf("Center : [%s]",_strvec(currentcenter)));
    this = neldermead_log (this,sprintf("Size : %s",string(ssize)));
    str = string ( simplex )
    for i = 1:size(str,"*")
        this = neldermead_log (this,str(i));
    end
endfunction

//
// neldermead_box --
//   The Nelder-Mead algorithm, with variable-size simplex
//   and modifications by Box for bounds and
//   inequality constraints.
//
function this = neldermead_box ( this )
    // Check settings correspond to algo
    [ this.optbase , hascons ] = optimbase_hasconstraints ( this.optbase );
    if ( ~hascons ) then
        errmsg = msprintf(gettext("%s: Problem has no constraints, but Box algorithm is designed for them."), "neldermead_box")
        error(errmsg)
    end
    verbose = optimbase_cget ( this.optbase , "-verbose" )
    //
    // Order the vertices for the first time
    //
    simplex = this.simplex0;
    n = optimbase_cget ( this.optbase , "-numberofvariables" );
    fvinitial = optimbase_get ( this.optbase , "-fx0" );
    // Sort function values and x points by increasing function value order
    this = neldermead_log (this,"Step #1 : order");
    simplex = optimsimplex_sort ( simplex );
    // Transpose, because optimsimplex returns row vectors
    currentcenter = optimsimplex_center ( simplex ).';
    currentxopt = optimbase_cget ( this.optbase , "-x0" );
    newfvmean = optimsimplex_fvmean ( simplex );
    nbve = optimsimplex_getnbve ( simplex );
    ihigh = nbve;
    inext = ihigh - 1
    ilow = 1
    [ this.optbase , hasbounds ] = optimbase_hasbounds ( this.optbase );
    nbnlc = optimbase_cget ( this.optbase , "-nbineqconst" )
    rho = this.boxreflect;
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
        xlow = optimsimplex_getx ( simplex , ilow ).'
        flow = optimsimplex_getfv ( simplex , ilow )
        xhigh = optimsimplex_getx ( simplex , ihigh ).'
        fhigh = optimsimplex_getfv ( simplex , ihigh )
        xn = optimsimplex_getx ( simplex , inext ).'
        fn = optimsimplex_getfv ( simplex , inext )
        //
        // Store history
        //
        xcoords = optimsimplex_getallx ( simplex )
        this = neldermead_storehistory ( this , n , flow , xlow , xcoords );
        currentfopt = flow;
        previousxopt = currentxopt;
        currentxopt = xlow;
        previouscenter = currentcenter;
        currentcenter = optimsimplex_center ( simplex ).';
        oldfvmean = newfvmean;
        newfvmean = optimsimplex_fvmean ( simplex );
        if ( verbose == 1 ) then
            this = neldermead_logsummary ( this, iter,xlow,flow,currentcenter,simplex);
        end
        this.optbase = optimbase_set ( this.optbase , "-xopt" , xlow );
        this.optbase = optimbase_set ( this.optbase , "-fopt" , flow );
        stop = neldermead_outputcmd ( this, "iter" , simplex , step )
        if ( stop ) then
            status = "userstop"
            if ( verbose == 1 ) then
                this = neldermead_log (this,sprintf("Terminate by user''s request"));
            end
            break
        end

        //
        // Update termination flag
        //
        if ( iter > 1 ) then
            [ this , terminate , status ] = neldermead_termination (this , ..
            fvinitial , oldfvmean , newfvmean , previouscenter , currentcenter , simplex );
            if ( terminate ) then
                if ( verbose == 1 ) then
                    this = neldermead_log (this,sprintf("Terminate with status : %s",status));
                end
                break
            end
        end
        //
        // Compute xbar, center of better vertices
        //
        if ( verbose == 1 ) then
            this = neldermead_log (this,sprintf("Reflect"));
        end
        xbar = optimsimplex_xbar ( simplex ).';
        if ( verbose == 1 ) then
            this = neldermead_log (this,sprintf("xbar=[%s]" , _strvec(xbar)));
        end
        //
        // Search a point improving cost function
        // and satisfying constraints.
        //
        [ this , scaled , xr , fr ] = _boxlinesearch ( this , n , xbar , xhigh , fhigh , rho );
        if ( scaled == %f ) then
            status = "impossibleimprovement"
            break
        end
        if ( verbose == 1 ) then
            this = neldermead_log (this,sprintf("xr=[%s], f(xr)=%f", _strvec(xr) , fr));
            this = neldermead_log (this,sprintf("  > Perform Reflection"));
        end
        simplex = optimsimplex_setve ( simplex , ihigh , fr , xr.' )
        step = "boxreflection";
        //
        // Sort simplex
        //
        if ( verbose == 1 ) then
            this = neldermead_log (this,sprintf("Sort"));
        end
        simplex  = optimsimplex_sort ( simplex );
    end
    this.optbase = optimbase_set ( this.optbase , "-xopt" , xlow );
    this.optbase = optimbase_set ( this.optbase , "-fopt" , flow );
    this.optbase = optimbase_set ( this.optbase , "-status" , status );
    this.simplexopt = simplex;
endfunction

//
// _strvec --
//  Returns a string for the given vector.
//
function str = _strvec ( x )
    str = strcat(string(x)," ")
endfunction
//
// _boxlinesearch --
//   For Box's method, perform a line search
//   from xbar, on the line (xhigh,xbar) and returns:
//   status : %t if the search is successful,
//            status=%f if the search failed.
//   xr : the reflected, scaled point
//   fr : the function value. fr=f(xr) if the search successful,
//        fr is Nan if the search failed.
//   The reflected point satisfies the following
//   constraints :
//   * fr < fhigh
//   * xr satisfies the bounds constraints
//   * xr satisfies the nonlinear positive inequality constraints
//   * xr satisfies the linear positive inequality constraints
//   The method is based on projection and scaling toward the centroid xbar.
//
// Arguments
//   n : number of variables
//   xbar : the centroid
//   xhigh : the high point
//   fhigh : function value at xhigh
//   rho : reflection factor
//
function [ this , status , xr , fr ] = _boxlinesearch ( this , n , xbar , xhigh , fhigh , rho )
    if ( verbose == 1 ) then
        this = neldermead_log (this,"_boxlinesearch");
        this = neldermead_log (this, sprintf ("> xhigh=[%s], fhigh=%s",_strvec(xhigh),string(fhigh)));
        this = neldermead_log (this, sprintf ("> xbar=[%s]" , _strvec(xbar) ) );
    end
    xr = neldermead_interpolate ( xbar , xhigh , rho );
    if ( verbose == 1 ) then
        this = neldermead_log (this, sprintf ( "> xr = [%s]" , _strvec ( xr ) ) );
    end
    alphamin = this.guinalphamin
    [ this.optbase , hasnlcons ] = optimbase_hasnlcons ( this.optbase );
    //
    // The algorithm has 3 steps:
    // 1. scale for bounds (cannot fail)
    // 2. scale for nonlinear constraints (may fail)
    // 3. scale for function improvement (may fail)
    //
    // 1. Project xr into bounds, with an additional alpha inside the bounds.
    // This algo is always succesful.
    // Note:
    //   If the alpha coefficient was not used, the
    //   projectinbounds method could be used directly.
    //
    [ this.optbase , hasbounds ] = optimbase_hasbounds ( this.optbase );
    if ( hasbounds ) then
        boxboundsalpha = this.boxboundsalpha;
        boundsmax = optimbase_cget ( this.optbase , "-boundsmax" );
        boundsmin = optimbase_cget ( this.optbase , "-boundsmin" );
        for ix = 1:n
            xmin = boundsmin ( ix );
            xmax = boundsmax ( ix );
            xrix = xr ( ix );
            if ( xrix > xmax ) then
                xr ( ix ) = xmax - boxboundsalpha;
            elseif ( xrix < xmin ) then
                xr ( ix ) = xmin + boxboundsalpha;
            end
        end
    end
    //
    // 2. Scale from xr to xbar into nonlinear inequality constraints
    // and update xr.
    // Set status to %f if the process fails, set status=%t if it succeeds.
    //
    nbnlc = optimbase_cget ( this.optbase , "-nbineqconst" );
    if ( nbnlc == 0 ) then
        status = %t
    else
        status = %f;
        alpha = 1.0;
        xr0 = xr;
        while ( alpha > alphamin )
            [ this.optbase , feasible ] = optimbase_isinnonlincons ( this.optbase , xr );
            if ( feasible ) then
                status = %t;
                break
            end
            alpha = alpha * this.boxineqscaling;
            xr = ( 1.0 - alpha ) * xbar + alpha * xr0;
        end
    end
    // If scaling failed, returns immediately
    // (there is no need to update the function value).
    if ( ~status ) then
        fr = %nan
        return
    end
    //
    // 3. Scale from xr toward xbar until fr < fhigh.
    //
    status = %f;
    xr0 = xr
    alpha = 1.0
    while ( alpha > alphamin )
        if ( hasnlcons ) then
            [ this.optbase , fr , cr , index ] = optimbase_function ( this.optbase , xr , 2 );
        else
            [ this.optbase , fr , index ] = optimbase_function ( this.optbase , xr , 2 );
        end
        if ( fr < fhigh ) then
            status = %t;
            break
        end
        alpha = alpha * this.boxineqscaling;
        xr = ( 1.0 - alpha ) * xbar + alpha * xr0;
    end
    // If the scaling for function improvement has failed,
    // we return.
    if ( ~status ) then
        fr = %nan
        return;
    end
endfunction
//
// costf_transposex --
//   Call the cost function and return the value.
//   Transpose the value of x, so that the input row vector,
//   given by optimsimplex, is transposed into a column vector,
//   as required by the cost function.
//
function [ f , this ] = costf_transposex ( x , this )
    xt = x.'
    [ f , this ] = neldermead_costf ( xt , this )
endfunction
