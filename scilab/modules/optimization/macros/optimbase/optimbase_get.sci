// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
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
// optimbase_get --
//   Get the value for the given key.
//   If the key is unknown, generates an error.
//   This command corresponds with options which are not
//   available directly to the user interface, but are computed internally.
//
function value = optimbase_get (this,key)
    [lhs,rhs]=argn();
    if ( rhs <> 2 ) then
        errmsg = msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "optimbase_get", 2);
        error(errmsg)
    end
    select key
    case "-funevals" then
        value = this.funevals;
    case "-iterations" then
        value = this.iterations;
    case "-xopt" then
        value = this.xopt;
    case "-fopt" then
        value = this.fopt;
    case "-historyxopt" then
        if ( ~this.storehistory ) then
            errmsg = msprintf(gettext("%s: History disabled ; enable -storehistory option."),"optimbase_get")
            error(errmsg)
        else
            value = this.historyxopt;
        end
    case "-historyfopt" then
        if ( ~this.storehistory ) then
            errmsg = msprintf(gettext("%s: History disabled ; enable -storehistory option."),"optimbase_get")
            error(errmsg)
        else
            value = this.historyfopt;
        end
    case "-fx0" then
        value = this.fx0;
    case "-status" then
        value = this.status;
    case "-logstartup" then
        value = this.logstartup;
    else
        errmsg = msprintf(gettext("%s: Unknown key %s"),"optimbase_get",key)
        error(errmsg)
    end
endfunction

