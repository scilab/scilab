// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function datatip_handle=datatipCreate(curve,index,ax)
//add a datatip at the given location of the given curve
//curve : handle on the curve
//index : index of the mesh point of the curve
//ax    : optional (the axes which contains the curve;
  if argn(2)<2 then
    error(msprintf(_("%s: Wrong number of input argument(s): At least %d expected.\n"),"datatipCreate",2))
  end
  
  bg=addcolor([255 255 238]/255);
  if type(curve)<>9|and(curve.type<>["Polyline"]) then
    error(msprintf(_("%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),"datatipCreate",1,"Polyline"))
  end

  ud=datatipGetStruct(curve);
  if typeof(ud)<>'datatips' then
    //no datatips structure yet, initialize it
    if ~datatipInitStruct(curve) then
      datatip_handle=[]
      return
    end
    ud=datatipGetStruct(curve)
  end

  formatfunction=ud.formatfunction;
  if argn(2)<3 then
    ax=curve.parent
    while ax.type<>"Axes" then ax=ax.parent,end
  else
    if type(ax)<>9|or(ax.type<>"Axes") then
      error(msprintf(_( "%s: Wrong type for input argument #%d: A ''%s'' handle expected.\n"),"datatipCreate",3,"Axes"))
    end
  end
  if curve.type=="Polyline" then
    if size(index,'*')==1 then
      pt = curve.data(index,:);
    else
      [d,pt,ind,c]=orthProj(curve.data,index)
      if ~ud.interpolate then
        if c<0.5 then 
          pt=curve.data(ind,:)
        else
          pt=curve.data(ind+1,:)
        end
      end
      index=[]
    end
  elseif curve.type=="Plot3d" then
    if size(index,'*')==2 then
      pt = [curve.data.x(index(1)) curve.data.y(index(2)) curve.data.z(index(1),index(2))]
    else
      pt=matrix(index,1,-1)
      index=[]
    end
  end
  drawlater()
  //create the curve marker
  xpoly(pt(1),pt(2));point_handle=gce();
  if size(pt,'*')==3 then point_handle.data(3)=pt(3),end
  point_handle.mark_mode='on';
  point_handle.mark_style=11;;
  point_handle.mark_size_unit='point';
  point_handle.mark_size=6;
  point_handle.mark_background=bg;
  point_handle.mark_foreground=curve.foreground;
  point_handle.user_data=list(curve,index)
  //create the text box
  xstring(pt(1),pt(2),formatfunction(curve,pt,index))
;
  string_handle=gce();
  if size(pt,'*')==3 then string_handle.data(3)=pt(3),end
  string_handle.background=bg;
  string_handle.foreground=color('gray');
  string_handle.box='on';
  string_handle.fill_mode='on';
  string_handle.clip_state='off';
  string_handle.user_data=0; //automatic orientation
  setStringPosition(string_handle,pt)
  drawnow()
  datatip_handle= glue([string_handle,point_handle])

  ud.tips=[ud.tips datatip_handle]
  //store the datatips data structure into the curve data structure
  datatipSetStruct(curve,ud)
endfunction
