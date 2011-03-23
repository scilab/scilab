// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function datatipRedraw(curve_handles)
//Recomputes the datatip text box position after a rotation
  
//These coordinates must be expressed in user coordinates units but their
//computation takes into account the pixel dimensions of the text boxes
//when they are not located in the upper right position. So during
//rotation or zoom the  text box positions has to be recomputed.
  
    drawlater()
    if argn(2)<>1 then //search for curves in the current axes
      curve_handles=datatipGetEntities()
    end
    if size(curve_handles,"*")==1&curve_handles.type=="axes" then
      //the argument is a handle on an axes, search for curves in it
      curve_handles=datatipGetEntities(curve_handles)
    end
    for k=1:size(curve_handles,'*')
      ud=datatipGetStruct(curve_handles(k))
      for l=1:size(ud.tips,'*')
        tip_handle=ud.tips(l)
        point_handle=tip_handle.children(1)
        string_handle=tip_handle.children(2)
        setStringPosition(string_handle,point_handle.data)
      end
    end
    drawnow()
endfunction
