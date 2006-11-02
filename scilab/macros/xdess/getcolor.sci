function c=getcolor(Title,cini)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==0 then
  Title="";
  cini=1;
elseif rhs==1 then 
  if type(Title)<>10 then
    error("getcolor: argument must be a string") 
  end
  cini=1;
elseif rhs==2 then
  if (type(Title)<>10 | type(cini)<>1 | size(cini,"*")<>1) then
    error("getcolor: arguments must be a string and an integer")
  end
else
  error("function ""color"" must have 1 or 3 arguments")
end

if winsid()<>[] then
  if get("figure_style")=="old" then
    cmap=xget("colormap");
  else
    cmap=get(gcf(),"color_map");
  end
  curwin=xget("window");
else
  cmap=[]
  curwin=[]
end
win=max(winsid()+1);
xset("window",win);

if get("figure_style")=="old" then
  set("figure_style","new");
end
sdf(); sda(); f=gcf();
if cmap<>[] then f.color_map=cmap;else cmap=f.color_map;end

N=size(cmap,1);
wdim=[1,1];
r=(wdim(1)/wdim(2));
n=round(sqrt(N/r));
m=int(n*r);
H=m*30;
W=n*30;
f.figure_size=[H,W];

if ~MSDOS then
  delmenu(win,"3D Rot.")
  delmenu(win,"UnZoom")
  delmenu(win,"Zoom")
  delmenu(win,"Edit")
  delmenu(win,"File")
  delmenu(win,"Insert")
else
  hidetoolbar(win)
   // French
  delmenu(win,"&Fichier")
  delmenu(win,"&Editer")
  delmenu(win,"&Outils")
  delmenu(win,"&Inserer")
  // English
  delmenu(win,"&File")
  delmenu(win,"&Edit")
  delmenu(win,"&Tools")
  delmenu(win,"&Insert")
end

dx=wdim(1)/m;dy=wdim(2)/n;
x=-dx; y=wdim(2);
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

if Title<>"" then
  xsetech([-1/8 0 1+1/6 1+1/6-1/8],[0 0 wdim(1),wdim(2)]);
else
  xsetech([-1/8 -1/8 1+1/6 1+1/6],[0 0 wdim(1),wdim(2)]);
end
// rectangles with the colors
xrects(rects,1:N);
// frame around the colors
r=m*n-N;
ddx=0.05*dx; ddy=0.05*dy;
if r==0 then
  xpoly([-ddx,1,1,-ddx],[0,0,1+ddy,1+ddy],"lines",1);
else
  xpoly([-ddx,1-1/n,1-1/n,1,1,-ddx],[0,0,r/m,r/m,1+ddy,1+ddy],"lines",1);
end

// Title
if Title<>"" then
  xtitle(Title);
  t=gce(); t.font_size=4; t.font_style=6;
end

k1=min(max(cini,1),N);
xrects(rects(:,k1),-k1);
e=gce(); rector=e.children; rector.thickness=4;
rector.foreground=color(255*(1-cmap(k1,1)),255*(1-cmap(k1,2)),255*(1-cmap(k1,3)));

//add a menu and its callback
done=%f;
addmenu(win,"File",["Ok","Cancel"]);execstr("File_"+string(win)+"=[""done=%t;k=k1;"";""done=%t;k=[]""]")
cmdok="execstr(File_"+string(win)+"(1))";
cmdcancel="execstr(File_"+string(win)+"(2))";

c_i=0;
c=cini;
while %t
  [c_i,cx,cy,cw,str]=xclick();
  if c_i==-2 then
    if str==cmdok then k=k1;c=k;break,end
    if str==cmdcancel then k=[];c=[];break,end
  end
  if c_i==-100 then k=[];break, end
  mc=int(cx/dx)+1;nc=n-int(cy/dy);
  k=((mc-1)*n+nc);
  if or(c_i==[0 3])&k<=N&k>0 then
    if k1<>0 then
      move(rector,[rects(1,k)-rects(1,k1),rects(2,k)-rects(2,k1)]);
      rector.foreground=color(255*(1-cmap(k,1)),255*(1-cmap(k,2)),255*(1-cmap(k,3)));
    end
    k1=k;
    name=rgb2name(cmap(k,:)*255);
    xinfo("Color number "+string(k)+": R="+string(floor(cmap(k,1)*255))+ ..
	" G="+string(floor(cmap(k,2)*255))+" B="+string(floor(cmap(k,3)*255))+ ..
	" Name="""+name(1)+"""");
  end
end

xdel(win);
if curwin<>[] then xset("window",curwin);end

endfunction
