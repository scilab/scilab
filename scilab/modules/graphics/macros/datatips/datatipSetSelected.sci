// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function datatipSetSelected(curve_handles,tip_indices)
//datatip utility function
//set the selected datatip(s)

  [curve,ind]=datatipGetSelected(curve_handles)
  //unselect the current datatip if any
  if ind<>[] then
    ud=datatipGetStruct(curve);
    tips=ud.tips.children
    tip_handle=tips(ind);
    datatipHilite(tip_handle) //unhilite
    ud.selected=0
    datatipSetStruct(curve,ud);
  end

  //select the new one
  if tip_indices<>[] then
    k=tip_indices(1);
    l=tip_indices(2);
    curve=curve_handles(k);
    ud=datatipGetStruct(curve);
    tips=ud.tips.children
    tip_handle=tips(l);
    datatipHilite(tip_handle)  //hilite
    ud.selected=l
    datatipSetStruct(curve,ud);
  end
endfunction

