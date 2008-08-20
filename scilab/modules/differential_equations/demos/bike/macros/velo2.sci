//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

function []=velo4p()
// version with full wheels with  xgeom & xfpoly
// ( geom performs the geometric calculations)

  curFig = gcf();
  curAxe = gca();

  curFig.pixmap = "on";

  t= t*180/%pi;
  p= p*180/%pi;
  xe=[xmin,xmax,xmax,xmin,xmin]
  ye=[ymin,ymin,ymax,ymax,ymin]

  param3d(xe,ye,zmin*ones(1,5),t,p," ",[1,0],rect);
  show_pixmap();

  // Rajoutons nous des petits poteaux
  poteau(xmin,(ymin+ymax)/2,0.4,0.4)
  arbre(xmax,ymax,0.1,0.1)
  arbre((xmax+xmin)/2,(ymin+ymax)/2,0.1,0.1)

  
  [n1,n2]=size(xfrontar);
  
//Caution: start at i=2
  if ~isdef('velo_rti') then   velo_rti=0.03; end 
  realtimeinit(velo_rti);
  realtime(1)
  param3d(xf(:,1),yf(:,1),zf(:,1),t,p," ",[1,0],rect);
  e3 = gce();
  plot3d(xrearar(:,1),yrearar(:,1),zrearar(:,1),t,p,flag=[1,0,0]);
  e2 = gce();e2.color_mode = -1;e2.hiddencolor = -1;
  plot3d(xfrontar(:,1),yfrontar(:,1),zfrontar(:,1),t,p,flag=[1,0,0]);
  e1 = gce();e1.color_mode = -1;e1.hiddencolor = -1;
  i=2;;
   param3d(xprear(1,i-1:i),xprear(2,i-1:i),xprear(3,i-1:i),t,p," ",[0,0]);
  erear = gce();
   param3d(xpfront(1,i-1:i),xpfront(2,i-1:i),xpfront(3,i-1:i),t,p," ",[0,0]); show_pixmap()  
  efront = gce();
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
    frontdata = efront.data;
    frontdata = [frontdata;xpfront(1,i),xpfront(2,i),xpfront(3,i)];
    efront.data = frontdata;
    drawnow();
    show_pixmap();

  end

endfunction

function []=poteau(xcenter,ycenter,xep,yep)
// rajoute un poteau de centre xcenter,ycenter)
// d'epaisseur xep et yep 
//!
// Comment rajouter un petit cube
  xc=[xcenter-xep*1.001,xcenter-xep*0.999,xcenter+xep*0.999,xcenter+xep*1.001];
  yc=[ycenter-yep*1.001,ycenter-yep*0.999,ycenter+yep*0.999,ycenter+yep*1.001];
  zmat=zmin*ones(4,4);
  zmat(2:3,2:3)=zmax*ones(2,2);
  // grise du poteau 
  potg=10
  plot3d(xc,yc,zmat,t,p," ",[potg,0,0]);
  show_pixmap();
endfunction


function []=arbre(xcenter,ycenter,xep,yep)
// rajoute un poteau de centre xcenter,ycenter)
// d'epaisseur xep et yep 
//!
// Comment rajouter un petit cube
  xc=[xcenter-xep*1.001,xcenter-xep*0.999,xcenter+xep*0.999,xcenter+xep*1.001];
  yc=[ycenter-yep*1.001,ycenter-yep*0.999,ycenter+yep*0.999,ycenter+yep*1.001];
  zmat=zmin*ones(4,4);
  zmat(2:3,2:3)=zmax*ones(2,2);
  // grise du poteau 
  potg=10
  plot3d(xc,yc,zmat,t,p," ",[potg,0,0]);
  rand('uniform');
  teta=%pi*rand(1,100);
  phi=2*%pi*rand(1,100);
  r=[zmax-zmin];
  x=[xcenter*ones(1,100);xcenter*ones(1,100)+r*cos(teta).*cos(phi)];
  y=[ycenter*ones(1,100);ycenter*ones(1,100)+r*cos(teta).*sin(phi)];
  z=[zmax*ones(1,100);zmax*ones(1,100)+r*sin(teta)];
  param3d(x,y,z,t,p," ",[1,0],rect)
  show_pixmap();
  curAxe = gca();
  curAxe.box = 'off';
endfunction


