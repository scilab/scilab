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
// optimbase_histget --
//   Returns the history value for the given key.
//   If the key is unknown, generates an error.
// Arguments
//   iter : the iteration for which the data is to store
//   key : the name of the data to store
//
function value = optimbase_histget ( this , iter , key )
    if ( ~this.storehistory ) then
        errmsg = msprintf(gettext("%s: History disabled ; turn on -storehistory option.") , "optimbase_histget" )
        error(errmsg)
    end
    if iter < 1 then
        errmsg = msprintf(gettext("%s: Negative iteration index %d is not allowed.") , "optimbase_histget" , iter )
        error(errmsg)
    end
    select key
    case "-xopt" then
        value = this.historyxopt ( iter );
    case "-fopt" then
        value = this.historyfopt ( iter );
    else
        errmsg = msprintf(gettext("%s: Unknown key %s") , "optimbase_histget" ,key)
        error(errmsg)
    end
endfunction

