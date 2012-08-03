// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function datatipHilite(datatip_handle)
//datatip utility function
  point_handle=datatip_handle.children(1)
  string_handle=datatip_handle.children(2)
  style=string_handle.user_data(2);
  if style(1)==1 then
    if point_handle.mark_background==point_handle.mark_foreground then
      point_handle.mark_background=addcolor([255 255 238]/255);
    else
      point_handle.mark_background=point_handle.mark_foreground;
    end
  else
    if point_handle.mark_mode=="on" then
      point_handle.mark_mode="off"
    else
      point_handle.mark_mode="on"
    end

  end
endfunction
