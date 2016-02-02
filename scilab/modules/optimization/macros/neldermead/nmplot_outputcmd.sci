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
// nmplot_outputcmd --
//  This command is called back by the Nelder-Mead
//  algorithm.
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
//  this : the current nmplot object
// stop: set to true when the algorithm must stop
//
function stop = nmplot_outputcmd ( state , data , this )
    nmplot_log ( this , "nmplot_outputcmd (1)")
    iter = data.iteration
    // Print simplex
    x = optimsimplex_getallx ( data.simplex )
    if this.simplexfn <> "" then
        nbve = optimsimplex_getnbve ( data.simplex )
        n = optimsimplex_getn ( data.simplex )
        mfprintf ( this.simplexhandle , "// Iteration #%d\n", iter )
        mfprintf ( this.simplexhandle , "history($+1) = [\n" )
        for ive = 1:nbve
            mfprintf ( this.simplexhandle , "// Vertex #%d\n", ive )
            for ix = 1:n
                mfprintf ( this.simplexhandle , "%e ", x(ive,ix))
            end
            mfprintf ( this.simplexhandle , "\n")
        end
        mfprintf ( this.simplexhandle , "]\n" )
    end
    // Function value average
    if this.fbarfn <> "" then
        fbar = optimsimplex_fvmean ( data.simplex )
        mfprintf ( this.fbarhandle , "%d %e\n", iter , fbar )
    end
    // Minimum function value
    if this.foptfn <> "" then
        fopt = data.fval
        mfprintf ( this.fopthandle , "%d %e\n", iter , fopt )
    end
    // Sigma
    if this.sigmafn <> "" then
        sigma = optimsimplex_size ( data.simplex , "sigmaplus" )
        mfprintf ( this.sigmahandle , "%d %e\n", iter , sigma )
    end
    stop = %f
endfunction

