function [Tree,windows]=do_navigator(scs_m,windows)
//build the tree representation
y0=0;x0=0;
path=[];larg=0;
[xx,yy,lp]=build_scs_tree(scs_m);
// open tree window
kw=find(windows(:,1)==100000)
if kw==[] then
  windows=[windows;[100000,get_new_window(windows)]]
  kw=size(windows,1)
end
xset('window',windows(kw,2))
xbasc();xset('default');xselect();
n_node=size(lp)
mnx=min(xx)-0.1;mxx=max(xx)+0.1
if n_node<=20 then
  dx=(mxx-mnx)
  mnx=mnx-dx/5
  x0=[mnx,mnx+dx/5]

else
  dx=(mxx-mnx)
  mnx=mnx-dx/2
  x0=[mnx,mnx+dx/4,mnx+dx/2]
end
bnds=[mnx,min(yy)-0.2, mxx,max(yy)+0.2];
xsetech([-1/6 -1/6 8/6 8/6],bnds)

// draw tree
xsegs(xx,yy,1)
xx=[xx(1,1);xx(2,:)'];
yy=[yy(1,1);yy(2,:)'];
plot2d(xx,yy,-9,'000')
xtitle('Navigator window')


for k=1:size(xx,1)
  xstring(xx(k),yy(k),string(k))
end

r=xstringl(x0(1),y0,'X');h=r(4);
y0=bnds(4)-h;
kx=1
xrect(x0(kx),bnds(4),x0(kx+1)-x0(kx),bnds(4)-bnds(2))
xclip([x0(kx),bnds(4),x0(kx+1)-x0(kx),bnds(4)-bnds(2)])
for k=1:size(xx,1)
  if k==1 then path=[]; else path=lp(k-1);end
  Path=list();
  for pk=path
    Path=lstcat(Path,pk,3,8);
  end
  Path=lstcat(Path,1,2,1);
  xstring(x0(kx),y0,string(k)+': '+scs_m(Path))
  y0=y0-h;
  if k==20 then 
    y0=bnds(4)-h;
    kx=kx+1
    xclip()
    xrect(x0(kx),bnds(4),x0(kx+1)-x0(kx),bnds(4)-bnds(2))
    xclip([x0(kx),bnds(4),x0(kx+1)-x0(kx),bnds(4)-bnds(2)])
  end
end
xclip()
//build data structure
Tree=tlist(['scs_tree','x','y','paths','orig'],xx,yy,lp,super_path)
xset('window',curwin)
