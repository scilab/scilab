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
// optimbase_set --
//   Set the value for the given key.
//   If the key is unknown, generates an error.
//
function this = optimbase_set ( this , key , value )
    [lhs,rhs]=argn();
    if ( rhs <> 3 ) then
        errmsg = msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "optimbase_set", 3);
        error(errmsg)
    end
    select key
    case "-iterations" then
        this.iterations = value;
    case "-xopt" then
        this.xopt = value;
    case "-fopt" then
        this.fopt = value;
    case "-historyxopt" then
        if ( ~this.storehistory ) then
            errmsg = msprintf( gettext ( "%s: History disabled ; turn on -storehistory option.") , "optimbase_set" )
            error(errmsg)
        else
            this.historyxopt = value;
        end
    case "-historyfopt" then
        if ( ~this.storehistory ) then
            errmsg = msprintf(gettext ( "%s: History disabled ; turn on -storehistory option.") , "optimbase_set" )
            error(errmsg)
        else
            this.historyfopt = value;
        end
    case "-fx0" then
        this.fx0 = value;
    case "-status" then
        this.status = value;
    else
        errmsg = msprintf(gettext ( "%s: Unknown key %s") , "optimbase_set", key )
        error(errmsg)
    end
endfunction

