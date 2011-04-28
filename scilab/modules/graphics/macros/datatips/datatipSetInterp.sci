// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function datatipSetInterp(curve_handle,m)
//changes the tips interpolation modes for a given curve
// curve_handle : a handle on a polyline
// m            : %t (interpolation active) or %f
  if argn(2)<1 then
    error(msprintf(_("%s: Wrong number of input argument(s): At least %d expected.\n"),...
                   "datatipSetInterp",1))
  end

  if argn(2)==1 then m=%f,end
  if type(curve_handle)<>9|size(curve_handle,'*')<>1|or(curve_handle.type<>"Polyline") then
    error(msprintf(_("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),...
                   "datatipSetInterp",1,"Polyline"))
  end

  ud=datatipGetStruct(curve_handle)
  if typeof(ud)<>"datatips" then;
    datatipInitStruct(curve_handle)
    ud=datatipGetStruct(curve_handle)
  end
  ud.interpolate=m
  datatipSetStruct(curve_handle,ud)
endfunction
