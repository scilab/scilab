function [f,g,ind]=rosenbrock(x,ind,p)
  f=[],g=[];
  n=size(x,'*')
  if or(ind==[2 4]) then
  global traj;  traj=[traj x(:)]
    i=2:n;
    f=1 + sum(p*(x(i)-x(i-1)^2)^2 + (1-x(i))^2)
  end
  if or(ind==[3 4]) then
    g=x;
    g(1)=-4*p*(x(2)-x(1)^2)*x(1)
    i=2:n-1
    if i<>[] then
      g(i)=2*p*(x(i)-x(i-1)^2)-4*p*(x(i+1)-x(i)^2).*x(i)- 2*(1-x(i))
    end
    g($)=2*p*(x($)-x($-1)^2) - 2*(1-x($))
  end
endfunction

function f=ros2(x,y)
  x=matrix(x,-1,1);y=matrix(y,-1,1);
  f=1 + p*(y*ones(x')-ones(y)*(x.^2)').^2 + ((1-y).^2)*ones(x')
endfunction

function showros()
global traj;traj=[]
 x=-35:0.3:35;
 y=-50:0.3:50;
 p=100
 xbasc();
 SetPosition();
 set figure_style new;xselect()
 xset('colormap',hotcolormap(512))
 a=gca();a.tight_limits="on"

 z=ros2(x,y)
 grayplot(y,x,log(z))
 
 x0=[40;10]
 [f,x,g]=optim(rosenbrock,x0);

 drawlater()
 xpoly(traj(1,:),traj(2,:),'lines')
 t=gce();t.thickness=3;t.foreground=512

 drawnow()
 clearglobal traj

 x0=[-50;-5]
 [f,x,g]=optim(rosenbrock,x0);
 drawlater()
 xpoly(traj(1,:),traj(2,:),'lines')
 t=gce();t.thickness=3;t.foreground=512
 drawnow()
 clearglobal traj

 // rotoate the figure
 a = gca() ;
 realtimeinit(0.01) ;
 for i=1:1000
   realtime(i);
   a.rotation_angles(2) = a.rotation_angles(2) + 1 ;
 end
 
endfunction
 
