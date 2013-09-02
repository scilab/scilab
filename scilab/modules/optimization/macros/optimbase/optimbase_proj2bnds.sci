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

