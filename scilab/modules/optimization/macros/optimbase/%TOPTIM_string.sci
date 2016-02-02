// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
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
// %TOPTIM_string --
//   Returns the string containing the Optim Base component.
//
function str = %TOPTIM_string ( this )
    str = []
    k = 1
    str(k) = sprintf("Optim Base Object:\n")
    k = k + 1
    str(k) = sprintf("==================")
    k = k + 1
    str(k) = sprintf("Number of variables : %s\n", string(this.numberofvariables));
    k = k + 1
    x0 = optimbase_cget (this,"-x0")
    str(k) = sprintf("Initial Guess : [%s]\n" , _strvec(x0) );
    k = k + 1
    str(k) = sprintf("Initial Function Value :%s\n",_strvec(this.fx0));
    k = k + 1
    str(k) = sprintf("Number of Inequality Constraints :%d\n",this.nbineqconst);
    k = k + 1
    str(k) = sprintf("Bounds Mininimum : [%s]\n", _strvec(this.boundsmin));
    k = k + 1
    str(k) = sprintf("Bounds Maxinimum :[%s]\n", _strvec(this.boundsmax));
    k = k + 1
    str(k) = sprintf("Optimum Parameters : [%s]\n" , _strvec(this.xopt));
    k = k + 1
    str(k) = sprintf("Optimum Function Value :%s\n",string(this.fopt));
    k = k + 1
    str(k) = sprintf("Number of iterations : %d\n", this.iterations);
    k = k + 1
    str(k) = sprintf("Maximum number of iterations : %s\n", string(this.maxiter));
    k = k + 1
    str(k) = sprintf("Number function evaluations : %d\n", this.funevals);
    k = k + 1
    str(k) = sprintf("Maximum number of function evaluations : %s\n", string(this.maxfunevals));
    k = k + 1
    str(k) = sprintf("Termination Method on function value : %s\n", string(this.tolfunmethod));
    k = k + 1
    str(k) = sprintf("Termination Absolute Tolerance on function value : %s\n", string(this.tolfunabsolute));
    k = k + 1
    str(k) = sprintf("Termination Relative Tolerance on function value : %s\n", string(this.tolfunrelative));
    k = k + 1
    str(k) = sprintf("Termination Method on x : %s\n", string(this.tolxmethod));
    k = k + 1
    str(k) = sprintf("Termination Absolute Tolerance on x : %s\n", string(this.tolxabsolute));
    k = k + 1
    str(k) = sprintf("Termination Relative Tolerance on x : %s\n", string(this.tolxrelative));
    k = k + 1
    str(k) = sprintf("Optimization Status : %s\n", this.status);
    k = k + 1
    str(k) = sprintf("Verbose logging : %s\n", string(this.verbose));
    k = k + 1
    str(k) = sprintf("Verbose Termination : %s\n", string(this.verbosetermination));
    k = k + 1
    str(k) = sprintf("Verbose Log File : %s\n", this.logfile );
    k = k + 1
    str(k) = sprintf("Verbose Log File Startup Up: %s\n", string(this.logstartup) );
    k = k + 1
    str(k) = sprintf("Store History : %s\n", string(this.storehistory));
endfunction

//
// _strvec --
//  Returns a string for the given vector.
//
function str = _strvec ( x )
    str = strcat(string(x)," ")
endfunction


