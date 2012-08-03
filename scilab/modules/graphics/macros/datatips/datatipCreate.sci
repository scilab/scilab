// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function tip_handle=datatipCreate(curve,index,ax)
//add a datatip at the given location of the given curve
//curve : handle on the curve
//index : index of the mesh point of the curve
//ax    : optional (the axes which contains the curve;
  if argn(2)<2 then
    error(msprintf(_("%s: Wrong number of input argument(s): At least %d expected.\n"),"datatipCreate",2))
  end

  if type(curve)<>9|size(curve,'*')<>1|and(curve.type<>["Polyline"]) then
    error(msprintf(_("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),"datatipCreate",1,"Polyline"))
  end

  ud=datatipGetStruct(curve);
  if typeof(ud)<>'datatips' then
    //no datatips structure yet, initialize it
    if ~datatipInitStruct(curve) then
      tip_handle=[]
      return
    end
    ud=datatipGetStruct(curve)
  end

  if argn(2)<3 then
    ax=curve.parent
    while ax.type<>"Axes" then ax=ax.parent,end
  else
    if type(ax)<>9|size(ax,'*')<>1|or(ax.type<>"Axes") then
      error(msprintf(_( "%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),"datatipCreate",3,"Axes"))
    end
  end
  fig=ax.parent;
  if curve.type=="Polyline" then
    if size(index,'*')==1 then
      pt = curve.data(index,:);
      l=index
    else
      [d,pt,ind,c]=orthProj(curve.data,index)
      if ~ud.interpolate then
        if c<0.5 then
          pt=curve.data(ind,:)
          l=ind
        else
          pt=curve.data(ind+1,:)
          l=ind+1
        end
      else
        l=ind
      end
      index=[]
    end
  elseif curve.type=="Plot3d" then //not implemented yet
    if size(index,'*')==2 then
      pt = [curve.data.x(index(1)) curve.data.y(index(2)) curve.data.z(index(1),index(2))]
    else
      pt=matrix(index,1,-1)
      index=[]
    end
  end

  id=fig.immediate_drawing;
  fig.immediate_drawing="off"
  curax=gca();sca(ax)
  //create the curve marker
  xpoly(0,0);point_handle=gce();
  point_handle.user_data=list(curve,l)
  xstring(0,0,'');string_handle=gce();
  string_handle.user_data=[0,ud.style];
  tip_handle= glue([string_handle,point_handle])
  datatipSetTipStyle(tip_handle,ud.style)
  if ud.tips==[] then
    //create a compound that will contains all the curve tips
    ud.tips=glue(tip_handle)
    datatipSetStruct(curve,ud)
  else
    relocate_handle(tip_handle,ud.tips)
  end

  datatipSetTipPosition(ud,tip_handle,pt,l)
  fig.immediate_drawing=id
  sca(curax)

endfunction
