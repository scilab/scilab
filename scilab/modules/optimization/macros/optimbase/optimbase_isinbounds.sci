// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
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
// optimbase_isinbounds --
//   Returns %t if the given point satisfies bounds constraints.
//   Returns %f if the given point is not in the bounds.
// Arguments
//   x : the point to analyse
//   isfeasible : = %t, %f
//
function [ this , isfeasible ] = optimbase_isinbounds ( this , x )
    isfeasible = %t
    [ this , hasbounds ] = optimbase_hasbounds ( this );
    if ( hasbounds ) then
        for ix = 1 : this.numberofvariables
            xmin = this.boundsmin ( ix )
            xmax = this.boundsmax ( ix )
            xix = x ( ix )
            if ( xix < xmin ) then
                isfeasible = %f
                this = optimbase_log ( this , sprintf ( "Component #%d/%d of x is lower than min bound %s", ...
                ix , this.numberofvariables , string(xmin) ) )
                break
            end
            if (xix > xmax) then
                isfeasible = %f
                this = optimbase_log ( this , sprintf ( "Component #%d/%d of x is greater than max bound %s", ...
                ix , this.numberofvariables , string(xmax) ) )
                break
            end
        end
    end
endfunction

