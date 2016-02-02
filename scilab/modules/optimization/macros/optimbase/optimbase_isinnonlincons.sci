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
// optimbase_isinnonlincons --
//   Returns %t if the given point satisfies inequality constraints.
//   Returns %f if the given point does not satisfies inequality constraints.
// Arguments
//   x : the point to analyse
//   isfeasible : = %t or %f
//
function [ this , isfeasible ] = optimbase_isinnonlincons ( this , x )
    isfeasible = %t
    if ( this.nbineqconst > 0) then
        if ( this.withderivatives ) then
            [ this , f , g , c , gc , index ] = optimbase_function ( this , x , 2 );
        else
            [ this , f , c , index ] = optimbase_function ( this , x , 5 );
        end
        for ic = 1 : this.nbineqconst
            if ( c ( ic ) < 0.0 ) then
                this = optimbase_log ( this , sprintf ( "Inequality constraint #%d/%d is not satisfied for x", ...
                ic , this.nbineqconst ) )
                isfeasible = %f
                break
            end
        end
    end
endfunction
