// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function setStringPosition(string_handle,pt)
//Computes the position of the lower left corner of the string box. 
//
//These coordinates must be expressed in user coordinates units but their
//computation takes into account the pixel dimensions of the text boxes
//when they are not located in the upper right position
  ax=string_handle.parent
  while ax.type<>"Axes" then ax=ax.parent,end

  
  r=xstringl(0,0,string_handle.text);r=r(3:4)';
  d=mark_usersize(point_handle);
  orient=string_handle.user_data;
  if orient==0 then //automatic
    if ax.zoom_box==[] then
      t=[pt(1:2)+r-ax.data_bounds(2,1:2)<0; //a l'interieur des max
         pt(1:2)-r-ax.data_bounds(1,1:2)>0] //a l'interieur des min
    else
      t=[pt(1:2)+r-ax.zoom_box([3 4])<0; //a l'interieur des max
         pt(1:2)-r-ax.zoom_box([1 2])>0] //a l'interieur des min
    end
    if and(t(1,:)) then //les deux sont inf aux max (upper right)
      orient=2;//1
    elseif and(t(2,:)) //les deux sont sup aux min (lower left)
      orient=3;//2
    elseif t(1,1)&t(2,2) //x est sup au min y est inf au max (upper left)
      orient=1;//3
    else//if t(1,2)*t(2,1) //y est sup au min   et x est inf au max (lower right)
      orient=4;
    end
  end
  select orient
  case 1 then //upper left
    dx=(-d(1)-r(1));dy=d(2);
  case 2 then //upper right
    dx=d(1);dy=d(2)
  case 3 then //lower left
    dx=-d(1)-r(1);dy=-d(2)-r(2)
  case 4 then //lower right
    dx=d(1);dy=-d(2)-r(2)
  end
  
  if ax.view=="3d" then
    angles=ax.rotation_angles*%pi/180;
    t=angles(2);a=angles(1);
    st=sin(t);ct=cos(t);sa=sin(a);ca=cos(a)
    //apply inverse transformation matrix to [dx,dy,0]
    //the transformation matrix is R=[-st ct 0;-ct*ca -st*sa sa;ct*sa st*sa ca] 
    //and the inserve is R'
    pos=pt+[-st*dx-ct*ca*dy,ct*dx-st*ca*dy,sa*dy]
  else
    pos=pt+[dx dy]
  end
  string_handle.data=pos
endfunction
function usize=mark_usersize(m)
  pix_size=10*(m.mark_size+1)
  [x1,y1]=xchange([0 pix_size],[0 pix_size],'i2f');
  usize=abs([x1(2)-x1(1) y1(2)-y1(1)])/20;
endfunction
