// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function datatip2

  my_handle = scf(100001);
  clf(my_handle,"reset");
  fig=gcf();
 
  title(_("datatips on 3D curves"),"fontsize",3);
  
 
  r=1;
  k=tan(%pi/27);
  t=linspace(-40,40,1000);
  x=r*cos(t)./cosh(k*t);
  y=r*sin(t)./cosh(k*t);
  z=r*tanh(k*t);
  param3d(x,y,z);c=gce();c.thickness=2;C.foreground=color('blue')
  
  ax=gca();ax.rotation_angles=[70 50];ax.axes_visible='off';
  drawnow()
  
  datatipInitStruct(c,'t',t)
  datatipSetStyle(c,2,%f) //directional arrow no box
  
  function s=mydisp(curve,pt,index),
    ud=datatipGetStruct(curve);
    if index<>[] then
      t=ud.t(index);
    else //interpolated
      [d,ptp,i,c]=orthProj(curve.data,pt);
      t=ud.t(i)+(ud.t(i+1)-ud.t(i))*c;
    end
    s=msprintf("%.2g", t);
  endfunction

  datatipSetDisplay(c,mydisp) // Change the default datatip label
  for k=[350 400 450 500 520  550 600]
    dt=datatipCreate(c,k);
  end

  demo_viewCode("datatip2.sce");

endfunction

datatip2();
clear datatip2;
