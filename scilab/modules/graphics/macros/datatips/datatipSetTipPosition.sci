// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function datatipSetTipPosition(ud,tip_handle,pt,k)
//datatip utility function
  point_handle=tip_handle.children(1)
  string_handle=tip_handle.children(2)

  tip_refs=point_handle.user_data
  [curve_handle,point_index]=tip_refs(:)
  formatfunction=ud.formatfunction

  if ud.style==0 then
    point_handle.data=pt
  else
    if k<size(curve_handle.data,1)
      dxy=curve_handle.data(k+1,:)-pt;
    else
      dxy=pt-curve_handle.data(k-1,:);
    end
    dxy=dxy/norm(dxy)/10000;
    point_handle.data=[pt;pt+dxy]
  end
  tip_refs(2)=k;
  point_handle.user_data=tip_refs
  string_handle.text=formatfunction(curve_handle,pt,k)
  setStringPosition(tip_handle,pt)

endfunction
