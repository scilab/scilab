//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

function []=velo1()
// "full wheel" version

  xe=[xmin;xmax;xmax;xmin;xmin]
  ye=[ymin;ymin;ymax;ymax;ymin]
  ze=[zmin;zmin;zmin;zmin;zmin];


  [n1,n2]=size(xfrontar);

  curAxe.children.thickness = 1;

  curFig = gcf();
 
  if driver()<>'Pos' then
    if ~isdef('velo_rti') then   velo_rti=0.05;end 
    realtimeinit(velo_rti);
    realtime(0)
    
    param3d(xe,ye,ze,t,p," ",[1,0],rect);
    show_pixmap();
    
    param3d(xf(:,1),yf(:,1),zf(:,1),t,p," ",[1,0],rect);
    e3 = gce();
    plot3d(xrearar(:,1),yrearar(:,1),zrearar(:,1),t,p,flag=[1,0,0]);
    e2 = gce();e2.color_mode = -1;e2.hiddencolor = -1;
    plot3d(xfrontar(:,1),yfrontar(:,1),zfrontar(:,1),t,p,flag=[1,0,0]);
    e1 = gce();e1.color_mode = -1;e1.hiddencolor = -1;
    show_pixmap();// draw the field 
    
      i=2;
  param3d(xprear(1,i-1:i),xprear(2,i-1:i),xprear(3,i-1:i),t,p," ",[0,0]);
  erear = gce();
//  param3d(xpfront(1,i-1:i),xpfront(2,i-1:i),xpfront(3,i-1:i),t,p," ",[0,0]); show_pixmap()  
//  efront = gce();

  curAxe = gca();
  curAxe.rotation_angles = [82,-3];
  curAxe.data_bounds(2,3)=min(xmax-xmin, ymax-ymin)/2;
  show_pixmap();

  for i=2:1:n2
        
    realtime(i);
    drawlater();
    e1.data.x=xfrontar(:,i);
    e1.data.y=yfrontar(:,i);
    e1.data.z=zfrontar(:,i);
    e2.data.x=xrearar(:,i);
    e2.data.y=yrearar(:,i);
    e2.data.z=zrearar(:,i);
    e3.data(:,1)=xf(:,i);
    e3.data(:,2)=yf(:,i);
    e3.data(:,3)=zf(:,i);
    reardata = erear.data;
    reardata = [reardata;xprear(1,i),xprear(2,i),xprear(3,i)];
    erear.data = reardata;
//    frontdata = efront.data;
//    frontdata = [frontdata;xpfront(1,i),xpfront(2,i),xpfront(3,i)];
//    efront.data = frontdata;
    drawnow();
    show_pixmap();

    end

  else
    if ~isdef('velo_rti') then   velo_rti=0.03;end 
    realtimeinit(velo_rti);
    realtime(0)
    for i=1:4:n2-1,
      ww=1:i+1;
      xpoly(xer,yer,'lines')
      plot2d((ct*xprear(1,ww)-st*xprear(2,ww))',...
	     (cp*(st*xprear(1,ww)+ct*xprear(2,ww))+sp*xprear(3,ww))',...
	     [1,-1],"000");
      velod(i);
      realtime(i);
    end

  end
endfunction


