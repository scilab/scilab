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
// optimbase_isfeasible --
//   Returns 1 if the given point satisfies bounds constraints and inequality constraints.
//   Returns 0 if the given point is not in the bounds.
//   Returns -1 if the given point does not satisfies inequality constraints.
// Arguments
//   x : the point to analyse
//   isfeasible : = 1, 0 or -1 (see above)
//
function [ this , isfeasible ] = optimbase_isfeasible ( this , x )
    isfeasible = 1
    //
    // Check if the point is in the bounds.
    //
    if ( isfeasible == 1 ) then
        [ this , hasbounds ] = optimbase_hasbounds ( this );
        if ( hasbounds ) then
            for ix = 1 : this.numberofvariables
                xmin = this.boundsmin ( ix )
                xmax = this.boundsmax ( ix )
                xix = x ( ix )
                if ( xix < xmin ) then
                    isfeasible = 0
                    this = optimbase_log ( this , sprintf ( "Component #%d/%d of x is lower than min bound %s", ...
                    ix , this.numberofvariables , string(xmin) ) )
                    break
                end
                if (xix > xmax) then
                    isfeasible = 0
                    this = optimbase_log ( this , sprintf ( "Component #%d/%d of x is greater than max bound %s", ...
                    ix , this.numberofvariables , string(xmax) ) )
                    break
                end
            end
        end
    end
    //
    // Check inequality constraints
    //
    if ( isfeasible == 1 ) then
        if ( this.nbineqconst > 0) then
            if ( this.withderivatives ) then
                [ this , f , g , c , gc , index ] = optimbase_function ( this , x , 5 );
            else
                [ this , f , c , index ] = optimbase_function ( this , x , 5 );
            end
            for ic = 1 : this.nbineqconst
                if ( c ( ic ) < 0.0 ) then
                    this = optimbase_log ( this , sprintf ( "Inequality constraint #%d/%d is not satisfied for x", ...
                    ic , this.nbineqconst ) )
                    isfeasible = -1
                    break
                end
            end
        end
    end
endfunction

