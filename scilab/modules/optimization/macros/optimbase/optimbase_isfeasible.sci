// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
            this = optimbase_log ( this , sprintf ( "Component #%d/%d of x is lower than min bound %e", ...
              ix , this.numberofvariables , xmin ) )
            break
          end
          if (xix > xmax) then
            isfeasible = 0
            this = optimbase_log ( this , sprintf ( "Component #%d/%d of x is greater than max bound %e", ...
              ix , this.numberofvariables , xmax ) )
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

