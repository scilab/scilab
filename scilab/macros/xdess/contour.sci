function contour(x,y,z,nz,theta,alpha,leg,flag,ebox,zlev)
  
  rhs=argn(2);
  if rhs<4, error('contour requires at least 4 arguments'),end;
  levels=[]

  opts=[]
  if exists('theta','local')==1 then opts=[opts,'theta=theta'],end
  if exists('alpha','local')==1 then opts=[opts,'alpha=alpha'],end
  if exists('leg'  ,'local')==1 then opts=[opts,'leg=leg']    ,end
  if exists('ebox' ,'local')==1 then opts=[opts,'ebox=ebox']  ,end
  if exists('flag' ,'local')==0 then flag=[0 1 0],end
  if exists('zlev' ,'local')==0 then zlev=0  ,end
  job=flag(1)
  if rhs==4|job==2 then  
    contour2d(x,y,z,nz);
    return,
  end
  if  get('figure_style')=='old' then 
    oldcontour(x,y,z,nz,theta,alpha,leg,flag,ebox,zlev),
    return,
  end
    
  if size(nz,'*')==1 then
    style=1:nz,
  else
    style=1:size(nz,'*')
  end
  
  opts=[opts,'flag=flag(2:3)'];

  [xc,yc]=contour2di(x,y,z,nz);


  fig=gcf();
  autoc=fig.auto_clear;
  if autoc=="on" then, xbasc(),end
  a=gca();
  fg=a.foreground
  v=fig.immediate_drawing;
  fig.immediate_drawing="off"
  fig.auto_clear="off"
  cnt=0

  fpf=xget("fpf");if fpf=='' then fpf='%.2g',end
  k=1;n=yc(k); c=0; level = %inf;
  while k < length(xc)
    n = yc(k)
    if xc(k) ~= level then 
      c = c+1; level = xc(k),levels=[level levels];
      a.foreground=c;
      if cnt>0 then glue(a.children(1:cnt)),cnt=0,end
    end
    
    if job==1 then zz=zlev,else zz=level,end
    execstr('param3d(xc(k+(1:n)),yc(k+(1:n)),zz*ones(1,n),'+opts+')')
    
    cnt = cnt+1
    if stripblanks(fpf)<>'' then
      xstring(xc(k+1+n/2),yc(k+1+n/2)," "+msprintf(fpf,level))
      e=gce();e.data(3)=zz
      cnt=cnt+1
    end
    k=k+n+1;
  end
  if cnt>0 then glue(a.children(1:cnt)),cnt=0,end
  set('current_obj',a);
  a.foreground=fg
  if flag(3)<2 then
    a.axes_visible='off';
    a.box='off';
  elseif flag(3)==3 then
    a.axes_visible='on';
    a.box='off';
   elseif flag(3)==4 then
    a.axes_visible='on';
    a.box='on';
  end
  fig.immediate_drawing=v;
  fig.auto_clear=autoc;
endfunction

