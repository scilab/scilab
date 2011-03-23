// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function datatipRemoveAll(curve_handles)
//remove all the datatips for the given curves
  if argn(2)<1 then
    error(msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"),"datatipRemoveAll",1))
  end

  drawlater()

  for k=1:size(curve_handles,'*')
    ck=curve_handles(k);
    ud=datatipGetStruct(ck)// the curve datatips data structure
    if typeof(ud)=='datatips' then
      for i=1:size(ud.tips,'*'), delete(ud.tips(i));end
      ud.tips=[]
      datatipSetStruct(ck,ud)
    end
  end
  drawnow()
endfunction
