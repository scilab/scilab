function []=velo3()
// Version roues a rayons ( qui tournent avec la roue !)
// la formule donnant l'angle de rotation est sans doute fausse
// j'ai suppose que la roue tournait avec la
// vitesse angulaire xx(6,i) pour la roue rear
// et que le temps d'integration etait de 1
// => theta (i) = xx(6,i)*i/imax
// Copyright INRIA
  ct=-cos(t);cp=cos(p);st=-sin(t);sp=sin(p);
  xe=[xmin,xmax,xmax,xmin,xmin]
  ye=[ymin,ymin,ymax,ymax,ymin]
  box=[ xe,xe; ye,ye ; zmin*ones(1,5),zmax*ones(1,5)];
  all=[ct -st 0; cp*st  cp*ct sp]*box
  //Bounds of window
  xmax=maxi(all(1,:));xmin=mini(all(1,:));
  ymax=maxi(all(2,:));ymin=mini(all(2,:));
  dx=0;dy=0;
  xb=[xmin-dx,xmax+dx]
  yb=[ymin-dy,ymax+dy]
 
  //Window definition
  isoview(xb(1),xb(2),yb(1),yb(2))
  plot2d(all(1,1:5)',all(2,1:5)',[1,-1],"000")
  // ground trajectory
 
  //plot2d((ct*xprear(1,:)-st*xprear(2,:))',...
  //     (cp*(st*xprear(1,:)+ct*xprear(2,:))+sp*xprear(3,:))',...
  //     [1,-1],"000");
 
  //plot2d((ct*xpfront(1,:)-st*xpfront(2,:))',...
  //     (cp*(st*xpfront(1,:)+ct*xpfront(2,:))+sp*xpfront(3,:))',...
  //     [1,-1],"000");
 
  [n1,n2]=size(xfrontar);
  xset("alufunction",6)
  if ~isdef('velo_rti') then   velo_rti=0.03;end 
  realtimeinit(velo_rti);
  realtime(0)
  for i=1:n2-1,velod(i);realtime(i);velod(i);
    ww=i:i+1;
    plot2d((ct*xprear(1,ww)-st*xprear(2,ww))',...
	   (cp*(st*xprear(1,ww)+ct*xprear(2,ww))+sp*xprear(3,ww))',...
	   [1,-1],"000");
    plot2d((ct*xpfront(1,ww)-st*xpfront(2,ww))',...
	   (cp*(st*xpfront(1,ww)+ct*xpfront(2,ww))+sp*xpfront(3,ww))',...
	   [1,-1],"000");
  end
  velod(n2-1);
  xset("alufunction",3);
endfunction
