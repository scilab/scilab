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
// optimbase_destroy --
//   Destroy an object.
//
function this = optimbase_destroy (this)
    [lhs,rhs]=argn();
    if ( rhs <> 1 ) then
        errmsg = msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "optimbase_destroy", 1);
        error(errmsg)
    end
    this.historyfopt = [];
    this.historyxopt = [];
    if ( this.logstartup ) then
        this = optimbase_logshutdown ( this );
    end
endfunction

