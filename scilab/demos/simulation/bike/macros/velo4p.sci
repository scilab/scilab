function []=velo4p()
// version with full wheels with  xgeom & xfpoly
// ( geom performs the geometric calculations)
// Copyright INRIA
  t= t*180/%pi;
  p= p*180/%pi;
  xe=[xmin,xmax,xmax,xmin,xmin]
  ye=[ymin,ymin,ymax,ymax,ymin]
  param3d(xe,ye,zmin*ones(1,5),t,p," ",[1,0],rect);
  // Rajoutons nous des petits poteaux
  poteau(xmin,(ymin+ymax)/2,0.4,0.4)
  arbre(xmax,ymax,0.1,0.1)
  arbre((xmax+xmin)/2,(ymin+ymax)/2,0.1,0.1)
  xset("alufunction",6)
  [xnt,ynt]=geom3d(xf,yf,zf);
  [xnf,ynf]=geom3d(xfrontar,yfrontar,zfrontar);
  [xnr,ynr]=geom3d(xrearar,yrearar,zrearar);
  /////////////////////////////////////
  [n1,n2]=size(xfrontar);
  deff('[]=velod(i)',['xpoly(xnt(:,i),ynt(:,i),''lines'')';
		      'xfpoly(xnr(:,i),ynr(:,i))';
		      'xfpoly(xnf(:,i),ynf(:,i))']);

  deff('[]=traj(i)',['j=i-1;';
		     'param3d(xprear(1,j:i),xprear(2,j:i),xprear(3,j:i),t,p,'" "',[0,0])';
		     'param3d(xpfront(1,j:i),xpfront(2,j:i),xpfront(3,j:i),t,p,'" "',[0,0])']);

  //Caution: start at i=2
  if ~isdef('velo_rti') then   velo_rti=0.03;end 
  realtimeinit(velo_rti);
  realtime(0)
  for i=2:1:n2;velod(i);
    traj(i);
    realtime(i);
    velod(i);end
    velod(n2);
    xset("alufunction",3);
endfunction

function []=poteau(xcenter,ycenter,xep,yep)
// rajoute un poteau de centre xcenter,ycenter)
// d'epaisseur xep et yp 
//!
// Comment rajouter un petit cube
  xc=[xcenter-xep,xcenter-xep,xcenter+xep,xcenter+xep];
  yc=[ycenter-yep,ycenter-yep,ycenter+yep,ycenter+yep];
  zmat=zmin*ones(4,4);
  zmat(2:3,2:3)=zmax*ones(2,2);
  // grise du poteau 
  potg=10
  plot3d(xc,yc,zmat,t,p," ",[potg,0,0]);
endfunction


function []=arbre(xcenter,ycenter,xep,yep)
// rajoute un poteau de centre xcenter,ycenter)
// d'epaisseur xep et yp 
//!
// Comment rajouter un petit cube
  xc=[xcenter-xep,xcenter-xep,xcenter+xep,xcenter+xep];
  yc=[ycenter-yep,ycenter-yep,ycenter+yep,ycenter+yep];
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
  [x,y]=geom3d(x,y,z);
  xsegs(x,y);
endfunction


