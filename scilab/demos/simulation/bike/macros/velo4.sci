function []=velo4()
// version with full wheels with  xgeom & xfpoly
// ( geom performs the geometric calculations)
// Copyright INRIA
  t= t*180/%pi;
  p= p*180/%pi;
  xe=[xmin,xmax,xmax,xmin,xmin]
  ye=[ymin,ymin,ymax,ymax,ymin]
  param3d(xe,ye,zmin*ones(1,5),t,p," ",[1,0],rect);
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
  
  // Attention demarrer a i=2
  if ~isdef('velo_rti') then   velo_rti=0.03;end 
  realtimeinit(velo_rti);
  realtime(0)
  for i=2:1:n2,traj();velod(i);realtime(i);velod(i);end
  velod(n2);
xset("alufunction",3);


