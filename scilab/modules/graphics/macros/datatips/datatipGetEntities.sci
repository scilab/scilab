// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function curve_handles=datatipGetEntities(ax)
//Collects all entities supporting datatips in a given axes
  curve_handles=[];
  if argn(2)<1 then
    ax=gca(),
  else
    if type(ax)<>9|size(ax,'*')<>1|and(ax.type<>["Axes" "Compound"]) then
      error(msprintf(_("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),"datatipGetEntities",1,"Axes"))
    end
  end
  for k=1:size(ax.children,'*')
    ck=ax.children(k);
    select ck.type
    case 'Compound' then
      curve_handles=[curve_handles datatipGetEntities(ck)]
    case 'Polyline' then
      if size(ck.data,1)>2 then
        curve_handles=[curve_handles ck];
      end
    case 'Plot3d' then
      //TBD
      //the surface are ignored
    else
      //the other type of entities are ignored
    end
  end
endfunction
