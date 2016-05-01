// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
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
// optimbase_terminate --
//   Returns %t if the algorithm terminates.
//   Returns %f if the algorithm must continue.
// Arguments, input
//   this : the current object
//   previousfopt : the previous value of the objective function
//   currentfopt : the current value of the objective function
//   previousxopt : the previous value of x
//   currentxopt : the current value of x
//   terminate : 1 if the algorithm terminates, 0 if the algorithm must continue.
// Arguments, output
//   status : termination status
//     status = "continue"
//     status = "maxiter"
//     status = "maxfuneval"
//     status = "tolf"
//     status = "tolx"
//
function [ this , terminate , status ] = optimbase_terminate (this , ...
    previousfopt , currentfopt , previousxopt , currentxopt )
    terminate = %f;
    status = "continue";
    if ( this.verbose == 1 ) then
        this = optimbase_stoplog (this,sprintf("  > Termination ?"));
    end
    //
    // Criteria #1 : maximum number of iterations
    //
    if ( ~terminate ) then
        if ( this.verbose == 1 ) then
            this = optimbase_stoplog (this,sprintf("  > iterations=%d >= maxiter=%d",this.iterations, this.maxiter));
        end
        if ( this.iterations >= this.maxiter ) then
            terminate = %t;
            status = "maxiter";
            if warning("query") =="on" then
                msg = "%s: Exiting: Maximum number of iterations has been exceeded\n" + ...
                "         - increase MaxIter option.\n";
                mprintf(gettext(msg) , "optimbase_terminate" )
            end
        end
    end
    //
    // Criteria #2 : maximum number of call to function
    //
    if ( ~terminate ) then
        if ( this.verbose == 1 ) then
            this = optimbase_stoplog (this,sprintf("  > funevals=%d >= maxfunevals=%d",this.funevals, this.maxfunevals));
        end
        if ( this.funevals >= this.maxfunevals ) then
            terminate = %t;
            status = "maxfuneval";
            if warning("query") =="on" then
                msg = "%s: Exiting: Maximum number of function evaluations has been exceeded\n" + ...
                "          - increase MaxFunEvals option.\n"
                mprintf(gettext(msg) , "optimbase_terminate" )
            end
        end
    end
    //
    // Criteria #3 : tolerance on function
    // Note :
    //   This termination criteria works well in the special case where the function
    //   value at optimum is several order of magnitude smaller
    //   than the initial function value (ie f(x0)).
    //   This is the case when the function value at optimum is zero.
    //   When the function value at optimum is non-zero, or if the
    //   initial function value is strictly positive (e.g. f(x0)=10)
    //   and the optimum function value is strictly negative (e.g. f(x*)=-10),
    //   that criteria fails miserably.
    //
    if ( ~terminate ) then
        if ( this.tolfunmethod )
            tolfr = this.tolfunrelative;
            tolfa = this.tolfunabsolute;
            acfopt = abs(currentfopt);
            apfopt = abs(previousfopt);
            if ( this.verbose == 1 ) then
                this = optimbase_stoplog (this,sprintf("  > abs(currentfopt)=%s < tolfunrelative * abs(previousfopt) + tolfunabsolute=%s",...
                string(acfopt), string(tolfr * apfopt + tolfa)));
            end
            if ( acfopt < tolfr * apfopt + tolfa ) then
                terminate = %t;
                status = "tolf";
            end
        end
    end
    //
    // Criteria #4 : tolerance on x
    // Note
    // What means a relative error on x ?
    // Notes: if xn and xn+1 are very close to xopt and xopt different from 0,
    // the relative error between xn and xn+1 is small.
    // But if xopt, xn and xn+1 are close to 0, the relative error may be a
    // completely wrong criteria. The absolute tolerance should be used in this case.
    //
    if ( ~terminate ) then
        if ( this.tolxmethod ) then
            normdelta = norm(currentxopt - previousxopt);
            normold = norm(currentxopt);
            tolxr = this.tolxrelative;
            tolxa = this.tolxabsolute;
            if ( this.verbose == 1 ) then
                this = optimbase_stoplog (this,sprintf("  > e(x)=%s < %s * %s + %s",...
                string(normdelta), string(tolxr) , string(normold) , string(tolxa) ));
            end
            if ( normdelta < tolxr * normold + tolxa ) then
                terminate = %t;
                status = "tolx";
            end
        end
    end
    if ( this.verbose == 1 ) then
        this = optimbase_stoplog (this,sprintf("  > Terminate = %s, status = %s",...
        string(terminate) , status ));
    end
endfunction
