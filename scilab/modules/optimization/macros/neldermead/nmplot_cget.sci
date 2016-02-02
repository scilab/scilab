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
// nmplot_cget --
//   Get the value for the given key.
//   If the key is unknown, generates an error.
//
function value = nmplot_cget (this,key)
    select key
    case "-simplexfn" then
        value = this.simplexfn;
    case "-fbarfn" then
        value = this.fbarfn;
    case "-foptfn" then
        value = this.foptfn;
    case "-sigmafn" then
        value = this.sigmafn;
    else
        this.nmbase = neldermead_configure ( this.nmbase , key , value )
    end
endfunction
