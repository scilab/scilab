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
// nmplot_configure --
//   Configure nmplot and returns the modified object.
//
function this = nmplot_configure (this,key,value)
    select key
    case "-simplexfn" then
        this.simplexfn = value;
    case "-fbarfn" then
        this.fbarfn = value;
    case "-foptfn" then
        this.foptfn = value;
    case "-sigmafn" then
        this.sigmafn = value;
    case "-outputcommand" then
        errmsg = msprintf(gettext("%s: Unknown key %s"), "nmplot_configure", key)
        error(errmsg)
    else
        this.nmbase = neldermead_configure ( this.nmbase , key , value )
    end
endfunction
