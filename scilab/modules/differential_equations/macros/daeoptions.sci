// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Scilab Enterprises - Paul Bignier : added daskr options
// Copyright (C) 2008 - INRIA - Sabine GAUZERE
// ...
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [%DAEOPTIONS] = daeoptions(%DAEOPTIONS)

    //%DAEOPTIONS=list(tstop,imode,band,maxstep,stepin,nonneg,isest)
    // This function displays the command line
    // %DAEOPTIONS=[...] to be executed
    // for defining the variable %DAEOPTIONS
    // This variables sets a number of optional parameters
    // for the lsod* fortran routines.

    //options = [1,0,0,%inf,0,2,500,12,5,0,-1,-1];
    options = list ([],0,[],[],[],0,0);
    //default = [string(options(1:10)),sci2exp(options(11:12))]
    default = ["0","[]","0","[]","[]","[]","0","0","0","[]","0","0","[]","[]","1"];
    //default(find(default=='Inf'))="%inf"

    if argn(2)>0 then
        options=%DAEOPTIONS
    end

    //lab_=[string(options(1:10)),sci2exp(options(11:12))]
    lab_= ["0","[]","0","[]","[]","[]","0","0","0","[]","0","0","[]","[]","1"];
    //lab_(find(lab_=="[]"))="[]"


    chapeau=["Defining %DAEOPTIONS variable";
    "*****************************";
    "Meaning of solver:";
    "solver is a real scalar which selects the solver";
    "0 : use dassl/dasrt";
    "1 : use daskr";
    " ";
    "Meaning of tstop:";
    "tstop is a real scalar which gives the maximum time";
    "for which g is allowed to be evaluated";
    "[] : if no limits imposed for time";
    " ";
    "Meaning of imode:";
    "0 : if only the user specified time point values";
    "1 : if dae returns its intermediate computed values";
    " ";
    "Meaning of band:";
    " ";
    "Meaning of maxstep:";
    " ";
    "Meaning of stepin:";
    " ";
    "Meaning of nonneg:";
    "0 : if the solution is known to be negative";
    "1 : if the solution is known to be non negative";
    " ";
    "Meaning of isest:";
    "0 : if g(t0,y0,ydot0)=0";
    "1 : if ydot0 is just an estimation and you want to use dassl/dasrt";
    "[+-1,...,+-1]: if ydot0 is just an estimation and you want to use daskr, with:";
    "1 if y(i) is a differential variable and";
    "-1 if y(i) is an algebraic variable";
    "(if its derivatives do not appear explicitly in the function g(t, y, ydot)).";
    " "
    "The following values are only used by daskr (solver = 1)";
    " ";
    "Meaning of method:";
    "0 : use GMRes Krylov method and provide a psol routine in dae";
    "1 : use direct method";
    " ";
    "Meaning of Kry_params:";
    "Treat as dummy argument if you have set method=0. Otherwise, set :";
    "[] : default parameters";
    "[maxl kmp nrmax epli] : where";
    "- maxl  = maximum number of iterations in the GMRes algorithm (default min(5, neq)),";
    "- kmp   = number of vectors on which orthogonalization is done in the GMRes algorithm (default maxl),";
    "- nrmax = maximum number of restarts of the GMRes algorithm per nonlinear iteration (default 5),";
    "- epli  = convergence test constant in GMRes algorithm (default 0.05).";
    " ";
    "Meaning of init:";
    "Treat as dummy argument if you have set isest=0. Otherwise, set :";
    "0 : stop after initial values computation";
    "1 : proceed to integration";
    " ";
    "Meaning of precond:";
    "Treat as dummy argument if you have set method=0. Otherwise, set :";
    "0 : specify a specific pjac routine in dae()";
    "1 : use the default";
    " ";
    "Meaning of control:";
    "[] : if you wish to control errors locally on all the variables then set to [].";
    "[+-1,...,+-1] :  1 if y(i) is a differential variable and -1 if y(i) is an algebraic variable";
    "(if its derivatives do not appear explicitly in the function g(t, y, ydot)).";
    " ";
    "Meaning of heuristic:";
    "Treat as dummy argument if you have set isest=0. Otherwise, set :";
    "[] : default parameters";
    "[mxnit mxnj mxnh lsoff stptol epinit] : where";
    "mxnj   = maximum number of Jacobian or preconditioner evaluations (default 6 if info(8)=0, 2 otherwise),";
    "lsoff  = flag to turn off the linesearch algorithm (lsoff = 0 means linesearch is on, lsoff = 1 means it is turned off) (default 0)";
    "mxnj   = maximum number of Jacobian or preconditioner evaluations (default 6 if info(8)=0, 2 otherwise),";
    "stptol = minimum scaled step in linesearch algorithm (default (unit roundoff)^(2/3)),";
    "epinit = swing factor in the Newton iteration convergence test (default 0.01).";
    " ";
    "Meaning of verbosity:";
    "0 : standard printing";
    "1 : minimal printing";
    "2 : full printing";
    " ";
    "Default values are given in square brackets"
    "If the function is called without argument, default values"+ ...
    " are used"
    ]

    dims = list("vec",1,"vec",-1,"vec",1,"vec",-1,"vec",-1,"vec",-1,..
    "vec",1,"vec",-1,"vec",1,"vec",-1,"vec",1,"vec",1,"vec",-1,"vec",-1,"vec",1);


    labels = ["solver (assumes solver = 0 or 1) ",...
    "tstop (maximum time) ","imode (assumes imode = 0 or 1)",...
    "band ()",...
    "maxstep (max step size)","stepin (initial step size)",...
    "nonneg (assumes nonneg = 0 or 1)",...
    "isest (assumes isest = 0, 1 or [+-1,...])",...
    "method (assumes method = 0 or 1)","Kry_params",...
    "init (assumes init = 0 or 1)","precond (assumes precond = 0 or 1)",...
    "control","heuristic","verbosity (assumes verbosity = 0 or 1)"] +"    ["+default+"]";



    [solver,ok,tstop,imode,band,maxstep,stepin,nonneg,isest,...
    method,Kry_params,init,precond,control,heuristic,verbosity] = getvalue(chapeau,labels,dims,lab_);
    //ml = mlmu(1);
    //mu = mlmu(2);

    if solver == 0 then
        DAEOPTIONS = list(tstop,imode,band,maxstep,stepin,nonneg,isest);
    else
        DAEOPTIONS = list(tstop,imode,band,maxstep,stepin,nonneg,isest,...
        method,Kry_params,init,precond,control,heuristic,verbosity);
    end

    if DAEOPTIONS<>list() then
        %DAEOPTIONS=DAEOPTIONS
    else
        %DAEOPTIONS=options
    end

endfunction
