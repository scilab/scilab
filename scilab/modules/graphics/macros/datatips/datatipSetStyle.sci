// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function datatipSetStyle(curve_handle,t,boxed,labeled)
//changes the tips interpolation modes for a given curve
// curve_handle : a handle on a polyline
// t            : 1 "square mark" or 2 "Directional arrow"
// boxed        : a boolean
// labeled      : a boolean
  if argn(2)<1 then
    error(msprintf(_("%s: Wrong number of input argument(s): At least %d expected.\n"),...
                   "datatipSetType",1))
  end
  if argn(2)<4 then labeled=%t,end
  if argn(2)<3 then boxed=%t,end

  if type(curve_handle)<>9|size(curve_handle,'*')<>1|or(curve_handle.type<>"Polyline") then
    error(msprintf(_("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),...
                   "datatipSetType",1,"Polyline"))
  end

  if argn(2)==1 then
    items=[_("Square mark, boxed label")
           _("Square mark, simple label")
           _("Square mark, no label")
           _("Directional arrow, boxed label")
           _("Directional arrow, simple label")
           _("Directional arrow, no label")
          ];
    sel=x_choose(items,_("Select tip style"))
    if sel==0 then return,end
    t=floor(sel/3)+1
    boxed=(modulo(sel,3)==1)
    labeled=~(modulo(sel,3)==0)
  else
    if type(t)<>1|size(t,'*')<>1 then
      error(msprintf(_("%s: Wrong type for input argument #%d: A scalar expected.\n"),...
                     "datatipSetType",2))
    end
    if and(t<>[1 2]) then
      error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),...
                     "datatipSetType",2,"1,2"))
    end
    if type(boxed)<>4 |size(t,'*')<>1 then
      error(msprintf(_("%s: Wrong type for input argument #%d: A boolean scalar expected.\n"),...
                     "datatipSetType",3))
    end
    if type(labeled)<>4 |size(labeled,'*')<>1 then
      error(msprintf(_("%s: Wrong type for input argument #%d: A boolean scalar expected.\n"),...
                     "datatipSetType",4))
    end

  end
  style=[t bool2s(boxed) bool2s(labeled)]

  ud=datatipGetStruct(curve_handle)
  if typeof(ud)<>'datatips' then;
    datatipInitStruct(curve_handle)
    ud=datatipGetStruct(curve_handle)
  end
  if or(ud.style<>style) then

    fig=curve_handle.parent
    while fig.type<>"Figure" then fig=fig.parent,end
    id=fig.immediate_drawing;
    fig.immediate_drawing="off"
    tips=ud.tips.children
    for i=1:size(tips,'*')
      datatipSetTipStyle(tips(i),style)
    end
    fig.immediate_drawing=id
    ud.style=style
    datatipSetStruct(curve_handle,ud)
  end

endfunction
