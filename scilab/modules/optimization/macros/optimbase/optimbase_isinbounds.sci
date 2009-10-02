// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

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
            this = optimbase_log ( this , sprintf ( "Component #%d/%d of x is lower than min bound %e", ...
              ix , this.numberofvariables , xmin ) )
            break
          end
          if (xix > xmax) then
            isfeasible = %f
            this = optimbase_log ( this , sprintf ( "Component #%d/%d of x is greater than max bound %e", ...
              ix , this.numberofvariables , xmax ) )
            break
          end
        end
      end
endfunction

