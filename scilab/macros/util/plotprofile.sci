function plotprofile(fun) 
  sep='='
sep=part(sep,ones(1,72))

if type(fun)==13 then
  lst=macr2lst(fun)
  count=profile(lst);count(:,3)=count(:,3).*count(:,1);
  txt=fun2string(lst,'fun')
  m=min(size(count,1),size(txt,1))
  txt=txt(1:m);count=count(1:m,:)
  txt(txt=='')=' '
  bnd=[];nf=0;
elseif type(fun)==10 then
  nf=size(fun,'*')
  txt=[];bnd=[];count=[];
  for k=1:nf
    execstr('lst=macr2lst('+fun(k)+')')
    c=profile(lst);c(:,3)=c(:,3).*c(:,1);
    t=fun2string(lst,fun(k))
    m=min(size(c,1),size(t,1))
    t=t(1:m);c=c(1:m,:)
    t(t=='')=' '
    if txt<>[] then
      txt=[txt;'=============================================']
      bnd=[bnd,size(txt,1)]
      count=[count;[0 0 0]]
    end
    txt=[txt;t]
    count=[count;c]
  end
end
n=size(count,1)


win=sum(winsid())+1
xset('window',win)
step=ceil(n/15)
nn=ceil((n+1)/step)
nm=nn*step
mx=max(count(:,1))+1
nc=size(count,1)

xsetech([0,0,1,1/3]);
plot2d3(1:size(count,1),count(:,1),style=1, frameflag=1,axesflag=1,..
	rect=[0,0,nm,mx],nax=[step nn 1 3])

xsegs(ones(2,1)*bnd,[0;mx]*ones(bnd),5*ones(bnd))
xp=[0 bnd];yp=mx*1.05;for k=1:nf,xstring(xp(k),yp,fun(k));end  
legends('# calls',1,1)

mx=max(1,max(count(:,3))+1)

xsetech([0,1/3,1,1/3]);
plot2d3(1:size(count,1),count(:,3),style=2, frameflag=1,axesflag=1,..
	rect=[0,0,nm,mx],nax=[step nn 1 3])
xsegs(ones(2,1)*bnd,[0;mx]*ones(bnd),5*ones(bnd))
legends('Complexity',2,1)

mx=max(count(:,2))+1

xsetech([0,2/3,1,1/3]);
plot2d3(1:size(count,1),count(:,2),style=3, frameflag=1,axesflag=1,..
	rect=[0,0,nm,mx],nax=[step nn 1 3])

xsegs(ones(2,1)*bnd,[0;mx]*ones(bnd),5*ones(bnd))
legends('Time',3,1)

if ~MSDOS then
  delmenu(win,'3D Rot.')
else
  delmenu(win,'3D &Rot.')
end   
addmenu(win,'Exit');str='execstr(Exit_'+string(win)+'(1))'
xinfo('click to get corresponding line')
ok=%t

[h,M]=dispfuntxt(txt,1,0,%f)



while ok
  [c_i,c_x,c_y,cw,cm]=xclick()
  if (c_i <0&cm==str)|c_i==-100 then break,end
  if cw==win then
    k=min(n,max(1,round(c_x)))
    // show source code in another window
    [h,M]=dispfuntxt(txt,k,h,M)
  end
end
endprof()

endfunction
function endprof()
if or(winsid()==(win+1)) then xdel(win+1);end
if or(winsid()==(win)) then xdel(win);end

endfunction

function [h,M]=dispfuntxt(txt,k,h,M)
  lbl=string(1:size(txt,1))';lbl=part(lbl,1:max(length(lbl)));
  t=lbl+': '+txt;
  if ~or(winsid()==(win+1)) then
    
    xset('window',win+1);xset('wpdim',400,600);
    xset('wdim',400,600);xset('wresize',0);
    curwin=win+1
    if ~MSDOS then
      delmenu(curwin,'3D Rot.')
      delmenu(curwin,'UnZoom')
      delmenu(curwin,'Zoom')
      delmenu(curwin,'File')
    else
      delmenu(curwin,'3D &Rot.')
      delmenu(curwin,'&UnZoom')
      delmenu(curwin,'&Zoom')
      delmenu(curwin,'&File')
    end     
    
    xsetech(wrect=[0 0 1 1],frect=[0 0 400 600],arect=[0.1 0 0 0])
    w=xstringl(0,0,t);h=w(4);w=max(400,w(3))
    if h>600 then
      xset('wdim',w,h)
      xsetech(wrect=[0 0 1 1],frect=[0 0 w h],arect=[0.1 0 0 0])
      M=%t
    elseif h<300 then
      xset('wpdim',400,300)
      xset('wdim',w,h)
      xsetech(wrect=[0 0 1 1],frect=[0 0 w h],arect=[0.1 0 0 0])
      M=%t
    else
      M=%f
    end
  else
    xset('window',win+1);
  end
  xclear();
  x=0;y=h;
  if k>1 then
    t1=t(1:k-1);
    w=xstringl(0,0,t1);w=w(4);
    y=y-w;xstring(x,y,t1);
  end


  xset('dashes',5)
  w=xstringl(0,0,t(k));w=w(4);
  y=y-w;xstring(x,y,t(k));
  yp=y;
  xset('dashes',33)


  if k<n then
    t1=t(k+1:$);
    w=xstringl(0,0,t1);w=w(4);
    y=y-w;xstring(x,y,t1)
  end


  if M then xset('viewport',x,-300+(k*(h/size(txt,1)))),end


  xset('window',win)
  xpause(10000)
  
endfunction
