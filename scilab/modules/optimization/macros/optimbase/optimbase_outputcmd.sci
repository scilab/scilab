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
// optimbase_outputcmd --
//   Calls back user's output command
// Arguments
//   this : the current object
//   state : the state of the algorithm,
//     "init", "done", "iter"
//   data : the data to pass to the client output command
//   stop : set to true to stop the algorithm
//
function stop = optimbase_outputcmd ( this , ...
    state , data )
    if ( this.outputcommand <> "" ) then
        //
        // Setup the callback and its arguments
        //
        funtype = typeof(this.outputcommand)
        if ( funtype == "function" ) then
            __optimbase_f__ = this.outputcommand
            //
            // Backward-compatibility: process the outputcommandarg field
            //
            if ( typeof(this.outputcommandarg) == "string" ) then
                __optimbase_args__ = list()
            else
                __optimbase_args__ = list(this.outputcommandarg)
            end
        else
            __optimbase_f__ = this.outputcommand(1)
            __optimbase_args__ = list(this.outputcommand(2:$))
        end
        //
        // Callback the output
        //
        stop = __optimbase_f__ ( state , data , __optimbase_args__(1:$) )
        //
        // Backward-compatibility: define the stop variable
        //
        if ( exists("stop")==0 ) then
            optb_warnheaderobsolete ( "outputfun(state,data)" , "stop=outputfun(state,data)", "5.4.1" )
            stop = %f
        end
    end
endfunction

function optb_warnheaderobsolete ( oldheader , newheader , removedVersion )
    warnMessage = msprintf(_("Syntax %s is obsolete."),oldheader)
    warnMessage = [warnMessage, msprintf(_("Please use %s instead."),newheader)]
    warnMessage = [warnMessage, msprintf(_("This feature will be permanently removed in Scilab %s"), removedVersion)]
    warning(warnMessage);
endfunction


