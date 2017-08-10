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
// optimbase_log --
//   If verbose logging is enabled, prints the given message in the console.
//   If verbose logging is disabled, does nothing.
//
function this = optimbase_log (this,msg)
    [lhs,rhs]=argn();
    if ( rhs <> 2 ) then
        errmsg = msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "optimbase_log", 2);
        error(errmsg)
    end
    if ( this.verbose == 1 ) then
        if ( this.logfile <> "" ) then
            mfprintf ( this.logfilehandle , "%s\n" , msg );
        else
            mprintf("%s\n",msg);
        end
    end
endfunction

