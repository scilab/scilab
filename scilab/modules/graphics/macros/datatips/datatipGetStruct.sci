// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function t=datatipGetStruct(curve)
//to be customized when a specific field will be dedicated to datatips
  if type(curve)<>9|size(curve,'*')<>1 then
    error(msprintf(_("%s: Wrong type for input argument #%d: Graphic handle expected.\n"),"datatipGetStruct",1))
  end

  u_d=get(curve,'user_data')
  if  typeof(u_d)=="datatips" then //compatibility
    t=u_d
  elseif typeof(u_d)=="st"&isfield(u_d,"datatips")
    t=u_d.datatips
  else
    t=[]
  end
endfunction
