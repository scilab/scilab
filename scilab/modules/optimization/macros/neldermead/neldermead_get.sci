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
// neldermead_get --
//   Get the value for the given key.
//   If the key is unknown, generates an error.
//
function value = neldermead_get ( this , key )
    select key
    case "-historysimplex" then
        storehistory = optimbase_cget ( this.optbase , "-storehistory" );
        if ( ~storehistory ) then
            errmsg = msprintf(gettext("%s: History disabled ; turn on -storehistory option."), "neldermead_get")
            error(errmsg)
        else
            value = this.historysimplex;
        end
    case "-simplexopt" then
        value = this.simplexopt;
    case "-simplex0" then
        value = this.simplex0;
    case "-restartnb" then
        value = this.restartnb;
    else
        // Delegate to optbase
        value = optimbase_get ( this.optbase , key );
    end
endfunction

