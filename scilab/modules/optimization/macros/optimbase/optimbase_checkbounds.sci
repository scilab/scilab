// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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

