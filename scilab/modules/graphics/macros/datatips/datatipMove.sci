// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function datatipMove(tip_handle,ax)
//moves a datatip along the associated curve
// tip_handle : handle on the given datatip compound;
  if argn(2)<1 then
    error(msprintf(_("%s: Wrong number of input argument(s): At least %d expected.\n"),"datatipMove",1))
  end

  if type(tip_handle)<>9|or(tip_handle.type<>"Compound") then
    error(msprintf(_("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),"datatipMove",1,"datatip"))
  end

  if argn(2)==1 then
    ax=tip_handle.parent
    while ax.type<>"Axes" then ax=ax.parent,end
  else
    if type(ax)<>9|or(ax.type<>"Axes") then
      error(msprintf(_( "%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),"datatipMove",2,"Axes"))
    end
  end
  point_handle=tip_handle.children(1)
  string_handle=tip_handle.children(2)
  tip_refs=point_handle.user_data
  [curve_handle,point_index]=tip_refs(:)
  data=curve_handle.data //the curve data points
  ud=datatipGetStruct(curve_handle);
  formatfunction=ud.formatfunction;
  interpolate=ud.interpolate;
  np=size(data,1)
  if ax.view=="3d"&curve_handle.type=="Polyline" then
    [xx,yy]=geom3d(data(:,1),data(:,2),data(:,3))
    data=[xx,yy]
  end
  rep=[0 0 -1];
  if ~interpolate then //position restricted to knots
    while rep(3)==-1
      rep=xgetmouse([%t %t])
      //find data point which is at minimum distance to the pointer
      [m,k]=min(sum((data-ones(np,1).*.rep(1:2)).^2,2))
      pt=curve_handle.data(k,:)
      lab=formatfunction(curve_handle,pt,k)
      r=xstringl(0,0,lab);r=r(3:4)'
      drawlater()
      point_handle.data=pt
      tip_refs(2)=k;
      point_handle.user_data=tip_refs
      string_handle.text=lab
      setStringPosition(string_handle,pt)
      drawnow()
    end
  else //interpolated position
    while rep(3)==-1
      rep=xgetmouse([%t %t])
      //find data point which is at minimum distance to the pointer
      [d,pt,k,c]=orthProj(data,rep(1:2))
      if pt<>[] then
        if ax.view=="3d"&curve_handle.type=="Polyline" then
          pt=curve_handle.data(k,:)+c*(curve_handle.data(k+1,:)-curve_handle.data(k,:))
        end
        lab=formatfunction(curve_handle,pt,k)
        r=xstringl(0,0,lab);r=r(3:4)'
        drawlater()
        point_handle.data=pt
        tip_refs(2)=k;
        point_handle.user_data=tip_refs
        string_handle.text=lab
        setStringPosition(string_handle,pt)
        drawnow()
      end
    end
  end
endfunction
