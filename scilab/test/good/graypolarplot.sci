function graypolarplot(theta,rho,z,varargin)
[lhs,rhs]=argn(0)
fstyle=get('figure_style')
if rhs<=0 then
  rho=1:0.2:4;theta=(0:0.02:1)*2*%pi;
  z=30+round(theta'*(1+rho^2));
  xbasc();
  if fstyle=='new' then 
    f=gcf();
    f.color_map=hotcolormap(128);
    f.background= 128;
    f.foreground=1;
    a=gca();
    a.background= 128;
    a.foreground=1;
  else
    xset('colormap',hotcolormap(128))
    xset('background',xget('white'))
  end
  graypolarplot(theta,rho,z)
  return
end


R=max(rho)
nv=size(varargin)
if nv>=1 then strf=varargin(2),else  strf='030',end
if nv>=2 then rect=varargin(4),else  rect=[-R -R R R]*1.1,end
if fstyle=='new' then 
  drawlater()
end
plot2d(0,0,1,strf,' ',rect)

[rho,k]=sort(rho);z=z(:,k);

nt=size(theta,'*');theta=matrix(theta,1,-1)*180/%pi
for k=1:size(rho,'*')-1
  r=rho(k)
  xfarcs([-r*ones(1,nt-1);
           r*ones(1,nt-1)
           2*r*ones(1,nt-1)
           2*r*ones(1,nt-1)
           theta(1:$-1)*64;
           (theta(2:$)-theta(1:$-1))*64
	   ],(z(1:$-1,k)+z(2:$,k)+z(1:$-1,k+1)+z(2:$,k+1))/4)
end
r=rho($)
if r<>0 then
  xfarcs([-r;r;2*r;2*r;0;360*64],xget('background'))
end

step=R/5
r=step;dr=0.02*r;
for k=1:4
  xset("dashes",3)
  xarc(-r,r,2*r,2*r,0,360*64)
  xset("dashes",1)
  xstring((r+dr)*cos(5*%pi/12),(r+dr)*sin(5*%pi/12),string(round(10*r)/10))
  r=r+step
end
xset("dashes",1)
xarc(-r,r,2*r,2*r,0,360*64)
xstring((r+dr)*cos(5*%pi/12),(r+dr)*sin(5*%pi/12),string(round(10*r)/10))

rect=xstringl(0,0,'360');w=rect(3);h=rect(4);d=sqrt(w^2+h^2)/1.8
r=R+d
for k=0:11
  xset("dashes",3)
  xsegs([0;R*cos(k*(%pi/6))],[0;R*sin(k*(%pi/6))])
  xset("dashes",1)
  xstring(r*cos(k*(%pi/6))-w/2,r*sin(k*(%pi/6))-h/2,string(k*30))
end
xset("dashes",1)
if fstyle=='new' then drawnow(),end
endfunction
