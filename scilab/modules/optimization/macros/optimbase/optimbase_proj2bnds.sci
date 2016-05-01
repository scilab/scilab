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
// optimbase_proj2bnds --
//   Returns a point, which is the projection
//   of the given point into the bounds.
// Arguments
//   this : the current object
//   x : the point to project
//   p : the projected point
//
function [ this , p ] = optimbase_proj2bnds ( this ,  x )
    [ this , hasbounds ] = optimbase_hasbounds ( this )
    if ( ~hasbounds ) then
        p = x
        return
    end
    p = x
    for ix = 1:this.numberofvariables
        xmin = this.boundsmin ( ix )
        xmax = this.boundsmax ( ix )
        pix = p ( ix )
        if (pix > xmax) then
            this = optimbase_log ( this , sprintf ( "Projecting p(%d) = %s on max bound %s" , ix , string(pix) , string(xmax) ))
            p ( ix ) = xmax
        elseif ( pix < xmin) then
            this = optimbase_log ( this , sprintf ( "Projecting p(%d) = %s on min bound %s" , ix , string(pix) , string(xmin) ))
            p ( ix ) = xmin
        end
    end
endfunction

