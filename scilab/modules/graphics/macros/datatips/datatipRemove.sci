// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function datatipRemove(handle,ind)
;
  drawlater()
  if argn(2)==1 then //handle on a tip
    if type(handle)<>9|or(handle.type<>"Compound") then
      error(msprintf(_("%s: Wrong size for input argument #%d: A ''%s'' handle expected.\n"),"datatipRemove",1,"datatip"))
    end
    ind=[]
    curve_handles=datatipGetEntities(handle.parent)
    for k=1:size(curve_handles,'*')
      ck=curve_handles(k)
      ud=datatipGetStruct(ck);
      if typeof(ud)=='datatips' then
        ind=find(ud.tips==handle)
        if ind<>[] then
          curve_handle=ck
          break
        end
      end
    end
    if ind==[] then return,end
  else
    curve_handle=handle;
    if type(curve_handle)<>9|or(curve_handle.type<>"Polyline") then
      error(msprintf(_("%s: Wrong size for input argument #%d: A ''%s'' handle expected.\n"),"datatipRemove",1,"Polyline"))
    end
  end

  ud=datatipGetStruct(curve_handle);// the curve datatips data structure
  if  typeof(ud)<>'datatips' then return,end
  if ind<=size(ud.tips,'*') then
    tips=ud.tips;
    del=tips(ind)
    tips(ind)=[]
    ud.tips=tips
    delete(del);
    datatipSetStruct(curve_handle,ud)
  end
  drawnow()
endfunction
