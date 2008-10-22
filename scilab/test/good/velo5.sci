function []=velo5()
//  full wheels using xgeom et xfpoly
// computations made by geom)
// Copyright INRIA
  t= t*180/%pi;
  p= p*180/%pi;
  xe=[xmin,xmax,xmax,xmin,xmin]
  ye=[ymin,ymin,ymax,ymax,ymin]

  //dr=driver();
  //driver('X11');
  //xset('pixmap',1);
  param3d(xe,ye,zmin*ones(1,5),t,p," ",[1,0],rect);
  param3d(xprear(1,:),xprear(2,:),xprear(3,:),  t,p," ",[0,0])
  param3d(xpfront(1,:),xpfront(2,:),xpfront(3,:), t,p," ",[0,0])
  [xnt,ynt]=geom3d(xf,yf,zf);
  [xnf,ynf]=geom3d(xfrontar,yfrontar,zfrontar);
  [xnr,ynr]=geom3d(xrearar,yrearar,zrearar);
  /////////////////////////////////////
  [n1,n2]=size(xfrontar);
  deff('[]=velod(i)',['xpoly(xnt(:,i),ynt(:,i),''lines'')';
		      'xpoly(xnr(:,i),ynr(:,i),''lines'')';
		      'xpoly(xnf(:,i),ynf(:,i),''lines'');velodp(i);']);
  if ~isdef('velo_rti') then   velo_rti=0.03;end 
  realtimeinit(velo_rti);
  realtime(0)
  xset('alufunction',6);
  for i=1:1:n2,velod(i);realtime(i);velod(i);end
  //driver(dr);
  xset('alufunction',3);
  //xset('pixmap',0);
  //velod(n2);
endfunction

