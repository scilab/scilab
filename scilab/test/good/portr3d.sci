function []=portr3d(fch,odem,xdim,npts,pinit)
//[]=portr3d(f,odem,[xdim,npts,pinit])
// Copyright INRIA
if ~isdef('p3d_xdim');p3d_xdim=['0';'1';'0';'1';'0';'1'];end
if ~isdef('p3d_npts');p3d_npts=['100';'0.1'];end
ncnl=lines();
lines(0);
xselect();
[lhs,rhs]=argn(0);
// appel minimal
if rhs<=1,odem='default';end
//Version interactive
if rhs <= 2,
  rep=x_mdialog('Graphic boundaries',...
            ['xmin';'xmax';'ymin';'ymax';'zmin';'zmax'],...
	    p3d_xdim);
  if rep<>[] ;p3d_xdim=rep;end
  xdim=evstr(p3d_xdim);xdim=xdim';
  // Test sur le cadre
  if xdim(2) <= xdim(1),
  write(%io(2),'Erreur:  xmin < xmax '),lines(ncnl(1));return,end
  if xdim(4) <= xdim(3),
  write(%io(2),'Erreur:  ymin < ymax '),lines(ncnl(1));return,end
  if xdim(6) <= xdim(5),
  write(%io(2),'Erreur:  zmin < zmax '),lines(ncnl(1));return,end
end
if rhs<=3,

  rep=x_mdialog('Requested points and step ',...
            ['n points';'step'],...
            p3d_npts);
  if rep<>[] then p3d_npts=rep;end
  npts=evstr(p3d_npts);npts=npts';
end
if isdef('p3d_ylast') then ylast=p3d_ylast;
else
ylast=(1/2)*[xdim(2)+xdim(1),xdim(4)+xdim(3),xdim(6)+xdim(5)]';
end
if rhs<=4
// Boucle sur les points de depart
  go_on=1
  while go_on==1,
       ftest=1;
       while ftest==1,
	  n=x_choose(['New initial point';'Continue ode';'Quit'],"Choose ");
	  n=n-1;
          if n==-1,go_on=0;lines(ncnl(1));
		[p3d_xdim,p3d_npts]=resume(p3d_xdim,p3d_npts);return;end
          if n==2,go_on=0;lines(ncnl(1));
		[p3d_xdim,p3d_npts]=resume(p3d_xdim,p3d_npts);return;end
          if n==0;x0=[string(ylast(1));string(ylast(2));string(ylast(3))];
		rep=x_mdialog(['Initial value inside the boundaries ';...
                               '['+ strcat(string(xdim)," ")+']'],...
                              ['x0';'y0';'z0'],x0);
		     if rep<>[] then x0=rep;end
                     x0=evstr(x0);end
          if n==1,x0=ylast;end
          ftest=desorb3d(odem,x0,npts,fch,xdim);
          if ftest==1;x_message('Initial value out of boundaris'),end
       end
  end
else
// Version sans poser de question
res=desorb3d(odem,pinit,npts,fch,xdim);
if res==1,write(%io(2),'Points hors du cadre elimines ');end;
end
lines(ncnl(1));
[p3d_xdim,p3d_npts]=resume(p3d_xdim,p3d_npts);



endfunction
function [res]=desorb3d(odem,x0,n1,fch,xdim);
//[res]=desorb3d(odem,x0,n1,fch,xdim);
// Calcule des orbites pour des points de
// depart donn\'es dans x0 et les dessine
// v\'erifie que les points de d\'epart sont a l'int\'erieur du
// cadre. Si l'un des points est a l'exterieur la valeur renvoy\'ee
// est 1
// renvoit aussi une valeur dans xlast ( le dernier point de la derniere
//  trajectoire)
//!
res=0
[nn1,n2]=size(x0);
if odem=='discret', style=[0], else style=-1;end
for i=1:n2,
    ftest=1;
    if x0(1,i) > xdim(2), ftest=0;end
    if x0(1,i) < xdim(1), ftest=0;end
    if x0(2,i) > xdim(4), ftest=0;end
    if x0(2,i) < xdim(3), ftest=0;end
    if x0(3,i) > xdim(6), ftest=0;end
    if x0(3,i) < xdim(5), ftest=0;end
    if ftest==0;res=1,ylast=x0,else
       write(%io(2),'Running ode ')
       if odem=='default' then 
        y=ode([x0(1,i);x0(2,i);x0(3,i)],0,n1(2)*(0:n1(1)),fch);
       else
        y=ode(odem,[x0(1,i);x0(2,i);x0(3,i)],0,n1(2)*(0:n1(1)),fch);
       end;
       [nn1,n11]=size(y);
       // on coupe la trajectoire au temps d'arret T
       // T d'atteinte du bord du cadre
       [mi1,ki1]=mini(y(1,:),xdim(2)*ones(1,n11));
       [ma1,ka1]=maxi(y(1,:),xdim(1)*ones(1,n11));
       k1=maxi(ki1,ka1);
 
       [mi2,ki2]=mini(y(2,:),xdim(4)*ones(1,n11));
       [ma2,ka2]=maxi(y(2,:),xdim(3)*ones(1,n11));
       k2=maxi(ki2,ka2);

       [mi3,ki3]=mini(y(3,:),xdim(6)*ones(1,n11));
       [ma3,ka3]=maxi(y(3,:),xdim(5)*ones(1,n11));
       k3=maxi(ki3,ka3);
 
       [m11,k11]=maxi(k1);
       [m22,k22]=maxi(k2);
       [m33,k33]=maxi(k3)
       if k11==1,k11=n1(1);end
       if k22==1,k22=n1(1);end
       if k33==1,k33=n1(1);end
       kf=mini(k11,k22,k33);
       if kf==1, kf=n1(1),end
       param3d(y(1,1:kf),y(2,1:kf),y(3,1:kf),60,45,"X@Y@Z",[1,3],xdim);
       ylast=y(:,kf);
    end
end
[ylast]=resume(ylast)
endfunction
