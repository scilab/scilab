function []=portrait(fch,odem,xdim,npts,farrow,pinit)
// phase portrait 
// Copyright INRIA
xselect();
ncnl=lines();
lines(0);
//addtitle(fch);
[lhs,rhs]=argn(0);
// minimal calling sequence 
if rhs<=1,odem='default';end
// Interactive version 
if odem == 'discrete'; style_d=x_choose(['trait continu','points'],['option de dessin']);
	style_d=mini(-style_d,1);end
if rhs <= 2,
  if ~isdef('p_xdim');p_xdim=['-1';'-1';'1';'1'];end
  rep=x_mdialog('Graphic boundaries',...
            ['xmin';'ymin';'xmax';'ymax'],...
            p_xdim);
  if rep<>[] ;p_xdim=rep;end
  xdim=evstr(p_xdim');
  // Test sur le cadre
  if xdim(3) <= xdim(1),
  write(%io(2),'Error:  xmin < xmax '),lines(ncnl(1));return,end
  if xdim(4) <= xdim(2),
  write(%io(2),'Error:  ymin < ymax '),lines(ncnl(1));return,end
end
res=x_choose(['yes';'no'],'Do you also want to draw the vector field')
if res==1;
  if ~isdef('p_nxx');p_nxx=['10';'10'];end;
  rep=x_mdialog('Number of grid points',...
            ['Nx';'Ny'],p_nxx);
  if rep<>[] then p_nxx=rep ;end
  nxx=evstr(p_nxx);
  nx=maxi(nxx(1),2)
  ny=maxi(nxx(2),2)
  nx=(0:(nx-1))/(nx-1)
  ny=(0:(ny-1))/(ny-1)
  nx=xdim(1)*(ones(nx)-nx)+xdim(3)*nx;
  ny=xdim(2)*(ones(ny)-ny)+xdim(4)*ny;
  fchamp(fch,0,nx,ny,1.0,xdim);
  style="000";
else 
  p_nxx=['10';'10'];
  style="051";
end

plot2d(0,0,0,style," ",xdim);
if rhs<=3,
  if ~isdef('p_npts');p_npts=['100';'0.1'];end;
  rep=x_mdialog('Requested points and step ',...
            ['n points';'step'],p_npts);
  if rep <> [] then p_npts=rep;end
  npts=evstr(p_npts');
end
ylast=(1/2)*[xdim(3)+xdim(1),xdim(4)+xdim(2)]';
if rhs<=4,farrow='f';end;
if rhs<=5
// Loop on Initial points 
  go_on=1
  while go_on==1,
       ftest=1;
       while ftest==1,
	  n=x_choose(['New initial point';'Continue ode';'Quit'],"Choose ");
	  n=n-1;
          if n==-1,go_on=0;lines(ncnl(1));
	    [p_xdim,p_npts,p_nxx]=resume(p_xdim,p_npts,p_nxx);
	  end
          if n==2,go_on=0;lines(ncnl(1));
	    [p_xdim,p_npts,p_nxx]=resume(p_xdim,p_npts,p_nxx);
	  end
	  if n==0,
	    while %t
	      [i,x,y]=xclick(); 
	      if i==-100 then return,end
	      if or(i==[0 1 2 3 4 5]) then break,end
	    end
	    x0=[x,y];
	  end;
          if n==1,x0=ylast';end;
          ftest=desorb(odem,x0',npts,fch,farrow,xdim);
          if ftest==1;x_message('Initial value out of boundaries'),end
       end
  end
else
// No question mode 
res=desorb(odem,pinit,npts,fch,farrow,xdim);
if res==1,write(%io(2),'Points hors du cadre elimines ');end;
end
lines(ncnl(1));
[p_xdim,p_npts,p_nxx]=resume(p_xdim,p_npts,p_nxx);


endfunction
function []=addtitle(fch)
// Adds know titles 
//!
if type(fch)<>11& type(fch)<>13 then return;end;
if fch==linear,xtitle("Systeme lineaire"," "," ",0);end
if fch==linper,xtitle("Systeme lineaire perturbe "," "," ",0);end
if fch==cycllim,xtitle("Systeme avec cycle limite "," "," ",0);end
if fch==bioreact,xtitle("Bioreacteur ","biomasse ","sucre ",0);end
if fch==lincom,xtitle("Systeme lineaire commande "," "," ",0);end
if fch==p_p,xtitle("Modele proie-predateur ","proies ","predateurs ",0);end
if fch==compet,xtitle("Modele de competition ","population 1 "...
,"population2 ",0);end
if fch=='bcomp',xtitle("Modele de competition observe-comtrole ",...
    "population 1 ","population2 ",0);end
if fch=='lcomp',xtitle("Modele de competition linearise observe-comtrole ",...
    "population 1 ","population2 ",0);end


endfunction
function [res]=desorb(odem,x0,n1,fch,farrow,xdim);
// Used by portrait 
//!
res=0
[nn1,n2]=size(x0);
style=1;
if odem=='discrete', style=style_d;end
for i=1:n2,
    ftest=1;
    if x0(1,i) > xdim(3), ftest=0;end
    if x0(1,i) < xdim(1), ftest=0;end
    if x0(2,i) > xdim(4), ftest=0;end
    if x0(2,i) < xdim(2), ftest=0;end
    if ftest==0;res=1,ylast=x0,else
       write(%io(2),'Calling ode')
       if odem=='default' then 
        y=ode([x0(1,i);x0(2,i)],0,n1(2)*(0:n1(1)),fch);
       else
        y=ode(odem,[x0(1,i);x0(2,i)],0,n1(2)*(0:n1(1)),fch);
       end;
       [nn1,n11]=size(y);
       // on coupe la trajectoire au temps d'arret T
       // T d'atteinte du bord du cadre
       [mi1,ki1]=mini(y(1,:),xdim(3)*ones(1,n11));
       [ma1,ka1]=maxi(y(1,:),xdim(1)*ones(1,n11));
       k1=maxi(ki1,ka1);
 
       [mi2,ki2]=mini(y(2,:),xdim(4)*ones(1,n11));
       [ma2,ka2]=maxi(y(2,:),xdim(2)*ones(1,n11));
       k2=maxi(ki2,ka2);
 
       [m11,k11]=maxi(k1);
       [m22,k22]=maxi(k2);
       if k11==1,k11=n1(1);end
       if k22==1,k22=n1(1);end
       kf=mini(k11,k22);
       if kf==1, kf=n1(1),end
       if farrow=='t',
          plot2d4("gnn",y(1,1:kf)',y(2,1:kf)',style,"000"," ",xdim);
       else
          plot2d(y(1,1:kf)',y(2,1:kf)',style,"000"," ",xdim);
       end,
       ylast=y(:,kf);
    end
end
[ylast]=resume(ylast)
endfunction
