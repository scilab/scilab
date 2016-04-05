// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
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
function [x,fval,exitflag,output] = fminsearch ( varargin )
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
    function stop = fminsearch_outputfun ( state , data , fmsdata )
        //
        // Compute procedure
        //
        select data.step
        case "init" then
            if ( data.iteration == 0 ) then
                procedure = "";
            else
                procedure = "initial simplex";
            end
        case "done" then
            procedure = ""
        case "reflection" then
            procedure = "reflect"
        case "expansion" then
            procedure = "expand"
        case "insidecontraction" then
            procedure = "contract inside"
        case "outsidecontraction" then
            procedure = "contract outside"
        case "shrink" then
            procedure = "shrink"
        else
            errmsg = msprintf(gettext("%s: Unknown step %s"), "fminsearch", data.step)
            error(errmsg)
        end
        //
        // Display a message
        //
        if ( fmsdata.Display == "iter" ) then
            if ( data.step <> "done" ) then
                mprintf ( "%6s        %5s     %12s         %-20s\n", ...
                string(data.iteration) , string(data.funccount) , string(data.fval) , procedure )
            else
                mprintf ( "\n" )
            end
        end
        //
        // Process output functions
        //
        stop = %f
        optimValues = struct(...
        "funccount" ,data.funccount , ...
        "fval" ,data.fval , ...
        "iteration" , data.iteration , ...
        "procedure" , procedure ...
        );
        if ( fmsdata.OutputFcn <> [] ) then
            if (or(type ( fmsdata.OutputFcn ) == [11 13])) then
                // The output function is a macro
                stop = fmsdata.OutputFcn ( data.x , optimValues , state );
                //
                // Backward-compatibility: define the stop variable
                //
                if ( exists("stop")==0 ) then
                    fms_warnheaderobsolete ( "outputfun(x,optimValues , state )" , "stop=outputfun(x,optimValues , state )", "5.4.1" )
                    stop = %f
                end
            elseif ( type ( fmsdata.OutputFcn ) == 15 ) then
                // The output function is a list of macros
                for i = 1:length(fmsdata.OutputFcn)
                    stop = fmsdata.OutputFcn(i) ( data.x , optimValues , state );
                end
                //
                // Backward-compatibility: define the stop variable
                //
                if ( exists("stop")==0 ) then
                    fms_warnheaderobsolete ( "outputfun(x,optimValues , state )" , "stop=outputfun(x,optimValues , state )", "5.4.1" )
                    stop = %f
                end
            else
                // The user did something wrong...
                errmsg = msprintf(gettext("%s: The value of the ''OutputFcn'' option is neither a function nor a list."), "fminsearch")
                error(errmsg)
            end
        end
        // Process plot functions
        if ( fmsdata.PlotFcns <> [] ) then
            if (or(type ( fmsdata.PlotFcns ) == [11 13] )) then
                // The output function is a macro
                fmsdata.PlotFcns ( data.x , optimValues , state );
            elseif ( type ( fmsdata.PlotFcns ) == 15 ) then
                // The output function is a list of macros
                for i = 1:length(fmsdata.PlotFcns)
                    fmsdata.PlotFcns(i) ( data.x , optimValues , state );
                end
            else
                // The user did something wrong...
                errmsg = msprintf(gettext("%s: The value of the ''PlotFcns'' option is neither a function nor a list."), "fminsearch")
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
        funtype = typeof(fmsfundata.Fun)
        if ( funtype == "function" ) then
            __fminsearch_f__ = fmsfundata.Fun
            __fminsearch_args__ = list()
        else
            __fminsearch_f__ = fmsfundata.Fun(1)
            __fminsearch_args__ = list(fmsfundata.Fun(2:$))
        end
        f = __fminsearch_f__ ( x , __fminsearch_args__(:))
    endfunction

    function fms_warnheaderobsolete ( oldheader , newheader , removedVersion )
        warnMessage = msprintf(_("Syntax %s is obsolete."),oldheader)
        warnMessage = [warnMessage, msprintf(_("Please use %s instead."),newheader)]
        warnMessage = [warnMessage, msprintf(_("This feature will be permanently removed in Scilab %s"), removedVersion)]
        warning(warnMessage);
    endfunction

    function errMessage = fms_errheaderobsolete (oldheader, newheader)
        errMessage = msprintf(_("Calling sequence %s is obsolete."),oldheader)
        errMessage = [errMessage, msprintf(_("Please use %s instead."),newheader)]
    endfunction


    function assert_typecallable ( var , varname , ivar )
        // Check that var is a function or a list
        if ( and ( type ( var ) <> [11 13 15] ) ) then
            errmsg = msprintf(gettext("%s: Expected function or list for variable %s at input #%d, but got %s instead."),"assert_typecallable", varname , ivar , typeof(var) );
            error(errmsg);
        end
        if ( type ( var ) == 15 ) then
            // Check that var(1) is a function
            if ( and ( type ( var(1) ) <> [11 13] ) ) then
                errmsg = msprintf(gettext("%s: Expected function for variable %s(1) at input #%d, but got %s instead."),"assert_typecallable", varname , ivar , typeof(var) );
                error(errmsg);
            end
        end
    endfunction
    // Generates an error if the given variable is not of type real
    function assert_typereal ( var , varname , ivar )
        if ( type ( var ) <> 1 ) then
            errmsg = msprintf(gettext("%s: Expected real variable for variable %s at input #%d, but got %s instead."),"assert_typereal", varname , ivar , typeof(var) );
            error(errmsg);
        end
    endfunction

    [lhs,rhs]=argn();
    if rhs<>2 & rhs<>3 then
        errmsg = msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), "fminsearch", 2,3);
        error(errmsg)
    end
    fun = varargin(1);
    x0 = varargin(2);
    // Get x0 and change it into a column vector
    x0t = size(x0,"*");
    x0 = matrix(x0,x0t,1);
    defaultoptions = optimset ("fminsearch");
    msg="";
    if rhs==2 then
        // No options on the command line
        // Set default values
        options = defaultoptions;
    elseif rhs==3 then
        // One options struc on the command line : use it !
        options = varargin(3);
    end
    // Compute options from the options struct
    numberofvariables = size(x0,"*");
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

    //check OutputFcn format
    if or(type(OutputFcn) == [11 13]) then
        macroInfo = macrovar(OutputFcn);
        if size(macroInfo(2), "*") <> 1 then
            errMessage = fms_errheaderobsolete("outputfun(x,optimValues , state )", "stop=outputfun(x,optimValues , state )");
            error(errMessage);
        end
    elseif type(OutputFcn) == 15 then
        for i = 1 : size(OutputFcn)
            if or(type(OutputFcn(i)) == [11 13]) then
                macroInfo = macrovar(OutputFcn(i));
                if size(macroInfo(2), "*") <> 1 then
                    errMessage = fms_errheaderobsolete("outputfun(x,optimValues , state )", "stop=outputfun(x,optimValues , state )");
                    error(errMessage);
                end
            end
        end
    elseif (OutputFcn <> [])
        // The user did something wrong...
        errmsg = msprintf(gettext("%s: The value of the ''OutputFcn'' option is neither a function nor a list."), "fminsearch");
        error(errmsg)
    end
    //
    // Check input arguments
    assert_typecallable ( fun , "costf" , 1)
    assert_typereal ( x0 , "x0" , 2 );
    //
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
    nm = neldermead_configure(nm,"-x0",x0);
    nm = neldermead_configure(nm,"-numberofvariables",numberofvariables);
    nm = neldermead_configure(nm,"-simplex0method","pfeffer");
    nm = neldermead_configure(nm,"-simplex0deltausual",0.05);
    nm = neldermead_configure(nm,"-simplex0deltazero",0.0075);
    nm = neldermead_configure(nm,"-method","variable");
    nm = neldermead_configure(nm,"-function",list(fminsearch_function,fmsfundata));
    nm = neldermead_configure(nm,"-maxiter",MaxIter);
    nm = neldermead_configure(nm,"-maxfunevals",MaxFunEvals);
    nm = neldermead_configure(nm,"-tolxmethod",%f);
    nm = neldermead_configure(nm,"-tolfunmethod",%f);
    nm = neldermead_configure(nm,"-tolssizedeltafvmethod",%t);
    nm = neldermead_configure(nm,"-tolsimplexizemethod",%f);
    nm = neldermead_configure(nm,"-toldeltafv",TolFun);
    nm = neldermead_configure(nm,"-tolsimplexizeabsolute",TolX);
    nm = neldermead_configure(nm,"-checkcostfunction",%f);
    nm = neldermead_configure(nm,"-outputcommand",list(fminsearch_outputfun,fmsdata));
    //nm = neldermead_configure(nm,"-verbose",1);
    //nm = neldermead_configure(nm,"-verbosetermination",1);
    nm = neldermead_search(nm, "off");
    x = neldermead_get(nm,"-xopt").';
    fval = neldermead_get(nm,"-fopt");
    status = neldermead_get(nm,"-status");
    select status
    case "maxiter" then
        if ( ( Display == "notify" ) | ( Display == "iter" ) | ( Display == "final" ) ) then
            msg = "%s: Exiting: Maximum number of iterations has been exceeded\n" + ...
            "         - increase MaxIter option.\n" + ...
            "         Current function value: %s\n"
            mprintf(gettext(msg) , "fminsearch" , string(fval) )
        end
        exitflag = 0;
    case "maxfuneval" then
        if ( ( Display == "notify" ) | ( Display == "iter" ) | ( Display == "final" ) ) then
            msg = "%s: Exiting: Maximum number of function evaluations has been exceeded\n" + ...
            "          - increase MaxFunEvals option.\n" + ...
            "         Current function value: %s\n"
            mprintf(gettext(msg) , "fminsearch" , string(fval) )
        end
        exitflag = 0;
    case "tolsizedeltafv" then
        exitflag = 1;
        msg = sprintf("%s\n%s %s\n%s %s", "Optimization terminated:",...
        " the current x satisfies the termination criteria using OPTIONS.TolX of",...
        string(TolX),...
        " and F(X) satisfies the convergence criteria using OPTIONS.TolFun of",...
        string(TolFun));
    case "userstop" then
        msg = sprintf("%s\n%s\n%s", "Optimization terminated:",...
        " ",...
        " User Stop");
        exitflag = -1;
    else
        errmsg = msprintf(gettext("%s: Unknown status %s"), "fminsearch", status)
        error(errmsg)
    end
    output = struct(...
    "algorithm" ,[],...
    "funcCount" ,[],...
    "iterations" ,[],...
    "message" , []);
    output.algorithm = "Nelder-Mead simplex direct search";
    output.funcCount = neldermead_get(nm,"-funevals");
    output.iterations = neldermead_get(nm,"-iterations");
    output.message = msg;
    if ( ( Display == "final" ) | ( Display == "iter" ) ) then
        if ( ( exitflag == 1 ) ) then
            mprintf( "%s\n" , output.message(1) );
            mprintf( "%s\n" , output.message(2) );
            mprintf( "%s\n" , output.message(3) );
        end
    end
    nm = neldermead_destroy(nm);
endfunction
