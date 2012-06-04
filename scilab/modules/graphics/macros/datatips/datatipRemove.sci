// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function datatipRemove(handle,ind)
  if argn(2)<1 then
    error(msprintf(_("%s: Wrong number of input argument(s): At least %d expected.\n"),...
                   "datatipRemove",1))
  end


  if argn(2)==1 then //handle on a tip
    if type(handle)<>9|size(handle,'*')<>1|or(handle.type<>"Compound") then
      error(msprintf(_("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),...
                     "datatipRemove",1,"datatip"))
    end
    ind=[]

    ax=handle.parent
    while ax.type<>"Axes" then ax=ax.parent,end
    curve_handles=datatipGetEntities(ax)

    for k=1:size(curve_handles,'*')
      ck=curve_handles(k)
      ud=datatipGetStruct(ck);
      if typeof(ud)=='datatips' then
        ind=find(ud.tips.children==handle)
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
      error(msprintf(_("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),...
                     "datatipRemove",1,"Polyline"))
    end
  end

  ud=datatipGetStruct(curve_handle);// the curve datatips data structure
  if  typeof(ud)<>'datatips' then return,end
  tips=ud.tips.children
  if ind<=size(tips,'*') then
    if argn(2)==2
      ind = length(tips) - ind + 1;
    end
    del=tips(ind)
    if ud.selected==ind then
      ud.selected=0;
      datatipSetStruct(curve_handle,ud);
    end
    delete(del);
  end

endfunction
