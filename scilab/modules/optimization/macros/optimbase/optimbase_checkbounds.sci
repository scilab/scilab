// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
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
// optimbase_checkbounds --
//   Check if the bounds are consistent and puts an error message if not.
//   One could generate an error, but errors are not testable
//   with the current system.
//
function this = optimbase_checkbounds ( this )
    maxl = length ( this.boundsmax )
    minl = length ( this.boundsmin )
    if ( maxl > 0 | minl > 0 ) then
        if ( this.numberofvariables <> maxl ) then
            errmsg = sprintf(gettext("%s: The number of variables %d does not match the number of max bounds: %d.\n") , ...
            "optimbase_checkbounds",this.numberofvariables , maxl )
            error(errmsg)
        end
        if ( this.numberofvariables <> minl ) then
            errmsg = sprintf(gettext("%s: The number of variables %d does not match the number of min bounds: %d.\n") , ...
            "optimbase_checkbounds",this.numberofvariables , minl )
            error(errmsg)
        end
        for ix = 1 : this.numberofvariables
            xmin = this.boundsmin ( ix )
            xmax = this.boundsmax ( ix )
            if ( xmax < xmin ) then
                errmsg = sprintf(gettext("%s: The max bound %s for variable #%d is lower than the min bound %s.\n"), ...
                "optimbase_checkbounds",string(xmax) , ix , string(xmin) )
                error(errmsg)
            end
        end
    end
endfunction

