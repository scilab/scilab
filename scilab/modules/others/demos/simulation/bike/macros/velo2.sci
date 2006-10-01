function []=velo2()
// empty wheel : the shortest
// (geometric calculations made by param3d)
// Copyright INRIA
  t= t*180/%pi;
  p= p*180/%pi;
  xe=[xmin,xmax,xmax,xmin,xmin]
  ye=[ymin,ymin,ymax,ymax,ymin]
  //plotting frame
  param3d(xe,ye,zmin*ones(1,5),t,p," ",[1,0],rect);
  //param3d(xprear(1,:),xprear(2,:),xprear(3,:),  t,p," ",[0,0])
  //param3d(xpfront(1,:),xpfront(2,:),xpfront(3,:), t,p," ",[0,0])
  xset("alufunction",6)
  /////////////////////////////////////
  [n1,n2]=size(xfrontar);
  deff('[]=velod(i)',['param3d(xf(:,i),yf(:,i),zf(:,i),t,p,'' '',[0,0]);';
		      'param3d(xfrontar(:,i),yfrontar(:,i),zfrontar(:,i),t,p,'' '',[0,0]);';
		      'param3d(xrearar(:,i),yrearar(:,i),zrearar(:,i),t,p,'' '',[0,0]);';])
  if ~isdef('velo_rti') then   velo_rti=0.03;end 
  realtimeinit(velo_rti);
  realtime(0)
  for i=1:1:n2-1,velod(i);realtime(i);velod(i);
    ww=i:i+1;
    param3d(xprear(1,ww),xprear(2,ww),xprear(3,ww),  t,p," ",[0,0])
    param3d(xpfront(1,ww),xpfront(2,ww),xpfront(3,ww), t,p," ",[0,0])
  end
  velod(n2-1);
  xset("alufunction",3);
endfunction



