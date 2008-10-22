
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function  ge_update_edges(karcs,arcs_handle)
//update edge display when a connected node is moved
  if argn(2)<2 then
    ge_axes_handle=gca()
    n=size(ge_axes_handle.children,'*')
    gindex=ge_axes_handle.user_data
    arcs_handle=ge_axes_handle.children(n+1-gindex.edge(karcs))
  end
  
  //  [xp,yp,xa,ya,xl,yl,ang]=ge_arcs_coordinates(karcs,'update')
  r=%pi/180
  for k=1:size(karcs,'*')
    [xyc,xya,xyl,ang]=ge_profile2real(karcs(k))
    ak=get(arcs_handle(k),'children')
    set(ak(3),'data',xyc)//path
    set(ak(2),'data',xya);//arrow
    if  ang>90&ang<270 then 
      w=get(ak(1),'user_data')
      set(ak(1),'data',[xyl(1)+w*cos(r*ang),xyl(2)-w*sin(r*ang)]);//label pos
      set(ak(1),'font_angle',ang+180);//label angle
    else
      set(ak(1),'data',[xyl(1),xyl(2)]);//label pos
      set(ak(1),'font_angle',ang);//label angle
    end
  end
endfunction

