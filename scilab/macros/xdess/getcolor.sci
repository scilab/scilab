function c=getcolor(title,cini)
// Copyright INRIA
  [lhs,rhs]=argn(0)
  if rhs<2 then cini=xget('pattern'),end
  if rhs<1 then title="",end
if MSDOS then
  c=getcolor_dos(title,cini)
else
  [lhs,rhs]=argn(0)
  if rhs<2 then cini=xget('pattern'),end
  if rhs<1 then title="",end
  colors=string(1:xget("lastpattern"))
  m=size(cini,'*')
  ll=list()
  for k=1:m
    ll(k)=list('colors',cini(k),colors);
  end
  c=x_choices(title,ll); 
end


endfunction
function k=getcolor_dos(title,cini)
[lhs,rhs]=argn(0)
curwin=xget('window')
if rhs<2 then cini=xget('pattern'),end
cmap=xget('colormap');
N=size(cmap,1);

win=max(winsid()+1)


wdim=[1,1]
r=(wdim(1)/wdim(2))
n=round(sqrt(N/r))
m=int(n*r)

xset('window',win);

xset('colormap',cmap)
H=m*30;
W=n*30;
xset('wdim',H,W)

if ~MSDOS then
  delmenu(win,'3D Rot.')
  delmenu(win,'UnZoom')
  delmenu(win,'Zoom')
  delmenu(win,'File')
else
  delmenu(win,'3D &Rot.')
  delmenu(win,'&UnZoom')
  delmenu(win,'&Zoom')
  delmenu(win,'&File')
end

dx=wdim(1)/m;dy=wdim(2)/n
x=-dx;y=wdim(2);
R=[0;0;dx*0.95;dy*0.95];
rects=[];
for k=1:N
  if modulo(k,n)==1 then
    x=x+dx;
    y=wdim(2);
  end
  rects=[rects R+[x;y;0;0]];
  y=y-dy;
end
driver('Rec');xset('pixmap',0),xset('alufunction',3)
xsetech([-1/8 -1/8 1+1/6 1+1/6],[0 0 wdim(1),wdim(2)])

xrects(rects,1:N)
Black=xget('lastpattern')+1;xset('pattern',Black)
if rhs>0 then xset('font',2,2);xtitle(title),xset('font',2,1);end
for k=1:N
  xstringb(rects(1,k),rects(2,k)-dy,string(k),rects(3,k),rects(4,k),"fill")
end

c_i=0;
xset('alufunction',6)
xset('thickness',4)
k1=min(max(cini,1),N)

xrects(rects(:,k1),-1),
//add a menu and its callback
done=%f;
addmenu(win,'File',['Ok','Cancel']);execstr('File_'+string(win)+'=[''done=%t;k=k1;'';''done=%t;k=[]'']')
//addmenu(win,'Cancel');execstr('Cancel_'+string(win)+'=''done=%t;k=[];''')

cmdok='execstr(File_'+string(win)+'(1))'
cmdcancel='execstr(File_'+string(win)+'(2))'
while %t
  [c_i,cx,cy,cw,str]=xclick();
  if c_i==-2 then
    if str==cmdok then k=k1;break,end
    if str==cmdcancel then k=[];break,end
  end
  if c_i==-100 then k=[];break, end
  
  mc=int(cx/dx)+1;nc=n-int(cy/dy);
  k=((mc-1)*n+nc);
  if c_i==0&k<=N&k>0 then
    if k1<>0 then  xrects(rects(:,k1),-1),end
    xrects(rects(:,k),-1);k1=k;
    xinfo('You have choosen color number: '+string(k))
  end
end
xdel(win)
xset('window',curwin)
endfunction
