// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimbase_checkbounds --
//   Check if the bounds are consistent and puts an error message if not.
//   One could generate an error, but errors are not testable 
//   with the current system.
// Arguments
//   isok : %T or %F
//
function [ this , isok , errmsg ] = optimbase_checkbounds ( this )
    maxl = length ( this.boundsmax )
    minl = length ( this.boundsmin )
    isok = %T
    errmsg = ""
    if ( maxl > 0 | minl > 0 ) then
      if ( isok & this.numberofvariables <> maxl ) then
        errmsg = sprintf("The number of variables %d does not match the number of max bounds %d from ["+...
          strcat(string(this.boundsmax)," ")+"]\n" , ...
          this.numberofvariables , maxl )
        isok = %F
      end
      if ( isok & this.numberofvariables <> minl ) then
        errmsg = sprintf("The number of variables %d does not match the number of min bounds %d from ["+...
          strcat(string(this.boundsmin)," ")+"]\n" , ...
          this.numberofvariables , minl )
        isok = %F
      end
      if ( isok ) then
        for ix = 1 : this.numberofvariables
          xmin = this.boundsmin ( ix )
          xmax = this.boundsmax ( ix )
          if ( xmax < xmin ) then
            errmsg = sprintf("The max bound %e for variable #%d is lower than the min bound %e.\n", ...
              xmax , ix , xmin )
            isok = %F
            break
          end
        end
      end
    end
endfunction

