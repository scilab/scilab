function plotprofile(fun)
lst=macr2lst(fun)
sep='='
sep=part(sep,ones(1,72))
count=profile(lst);count(:,3)=count(:,3).*count(:,1);
txt=fun2string(lst,'fun')
m=min(size(count,1),size(txt,1))
txt=txt(1:m);count=count(1:m,:)
txt(txt=='')=' '
n=size(count,1)
win=xget('window');xbasc()
step=ceil(n/15)
nn=ceil((n+1)/step)
nm=nn*step
xsetech([0,0,1,1/3]);plot2d3('enn',0,count(:,1),1,'011',' ',..
    [0,0,nm,max(count(:,1))+1],[step nn 1 3])
xsetech([0,1/3,1,1/3]);plot2d3('enn',0,count(:,2),2,'011',' ',..
    [0,0,nm,max(1,max(count(:,2)))],[step nn 1 3])
xsetech([0,2/3,1,1/3]);plot2d3('enn',0,count(:,3),3,'011',' ',..
    [0,0,nm,max(count(:,3))+1],[step nn 1 3])
delmenu(win,'3D Rot.')
addmenu(win,'Exit');str='execstr(Exit_'+string(win)+'(1))'
xinfo('click to get corresponding line')
ok=%t
dispfuntxt(txt,1)



while ok
  [c_i,c_x,c_y,cw,cm]=xclick()
  if c_i <0&cm==str then break,end
  if cw==win then
    k=min(n,max(1,round(c_x)))
    // show source code in another window
    dispfuntxt(txt,k)
  end
end
endprof()

function endprof()
xdel(win+1);xdel(win)

function dispfuntxt(txt,k)
    lbl=string(1:size(txt,1))';lbl=part(lbl,1:max(length(lbl)));
    t=lbl+': '+txt;


    xset('window',win+1);xbasc();xbasr();xset('wpdim',400,600);
    xset('wdim',400,600);xset('wresize',0);
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
    
