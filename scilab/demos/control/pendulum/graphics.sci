function []=anim(x,teta)
//anim(x,teta) animated plot
//!
// Copyright INRIA
lc=l/4,
hc=l/5,
maxp=maxi(x+l*sin(teta));minp=mini(x+l*sin(teta));
maxx=maxi([x,maxp]);minx=mini([minp,x])
rect=[minx,-l,maxx,l]
xset("thickness",2);
[m,n]=size(x);
isoview(minx,maxx,-l,l+hc);
realtimeinit(0.06)
if driver()<>'Pos' then
  xset("alufunction",6);
  for k=1:n, dpnd1(x(k),teta(k));realtime(k);dpnd1(x(k),teta(k));end;
  xset("alufunction",3);
else
  xset('pixmap',1)
  rect=[minx,-l,maxx,l+hc]
  for k=1:3:n,
    xpoly(rect([1 3 3 1]),rect([2,2,4,4]),'lines',1)
    dpnd1(x(k),teta(k))
    xset('wshow')
  end
  xset('pixmap',0)
end
xset("thickness",1);


function []=dpnd()
//dpnd() scheme of experiment
//!
isoview(0,100,0,100);
lc=20,
hc=10,
lb=40,
teta=.25,
xg=40,
y1=25,
y2=y1+hc,
yg=y1+hc/2,
x1=xg-lc/2;x2=xg+lc/2,
xpoly([x1 x2 x2 x1 x1],[y1,y1,y2,y2,y1],"lines",1),
xsegs([xg,xg+lb*sin(teta)],[y2,y2+lb*cos(teta)]),
xarc(x1+lc/10-2.5,y1-2.5+2.5,5,5,0,360*64);
xarc(x2-5+lc/10-2.5,y1-2.5+2.5,5,5,0,360*64);
xsegs([10 90],[y1-5 y1-5]); 
xarrows([x2 x2+10],[yg yg],3.0);
xstring(x2+20,yg,'u (force)',0,0);
xset("dashes",1);xsegs([xg xg],[y2 y2+lb]);xstring(xg,y2+lb,' teta',0,0);
xset("default");

function []=dpnd1(x,theta)
//dpnd() scheme
//!
lc=l/4,
hc=l/5,
xg=x,
y1=0,
y2=y1+hc,
yg=y1+hc/2,
x1=xg-lc/2;x2=xg+lc/2,
xpoly([x1 x2 x2 x1 x1],[y1,y1,y2,y2,y1],"lines",1),
xsegs([xg,xg+l*sin(theta)],[y2,y2+l*cos(theta)]),
r=lc/4
xarc(x1+lc/10,y1,r,r,0,360*64);
xarc(x2-r-lc/10,y1,r,r,0,360*64);

function draw(job)
[xxr,yyr]=xgetech();
wdim=xget('wdim');
[xxf]=xget("font");
xbasc();
xset("font",2,0);
xset("wdim",850,650);
hc=l/5,
if job==0 then
  if driver()<>'Pos' then
    xsetech([0,0,0.5,0.5]);
    plot2d1("enn",1,y(1,:)');xtitle(' ','t',' position');
    xsetech([0.5,0,0.5,0.5]);
    plot2d1("enn",1,y(2,:)');xtitle(' ','t',' theta');
    xsetech([0,0.5,1.0,0.5]);
    anim(100*y(1,:),100*y(2,:)),
  else	
    xset('pixmap',1)
    x=100*y(1,:);teta=100*y(2,:);
    maxp=maxi(x+l*sin(teta));minp=mini(x+l*sin(teta));
    maxx=maxi([x,maxp])+hc;minx=mini([minp,x])
    [m,n]=size(x);
    for k=1:3:n,
      xsetech([0,0,0.5,0.5]);
      plot2d1("enn",1,y(1,:)');xtitle(' ','t',' position');
      xsetech([0.5,0,0.5,0.5]);
      plot2d1("enn",1,y(2,:)');xtitle(' ','t',' theta');
      xsetech([0,0.5,1.0,0.5]);
      isoview(minx,maxx,-l,l);
      dpnd1(x(k),teta(k))
      xset('wshow')
    end
    xset('pixmap',0)
  end	
else
  if driver()<>'Pos' then
    xsetech([0,0,0.5,0.5]);
    plot2d(t1',(kr*yd(5:8,:))');xtitle(' ','time','control');
    xsetech([0.5,0,0.5,0.5]);
    plot2d([t1;t1]',yd([3,7],:)',[1,2],"121",'theta@theta estimated');
    xsetech([0,0.5,1.0,0.5]);
    anim(yd(1,:),yd(3,:))
  else
    xset('pixmap',1)
    x=100*yd(1,:);teta=100*yd(2,:);
    maxp=maxi(x+l*sin(teta));minp=mini(x+l*sin(teta));
    maxx=maxi([x,maxp])+hc;minx=mini([minp,x])
    [m,n]=size(x);
    for k=1:3:n,
      xsetech([0,0,0.5,0.5]);
      plot2d(t1',(kr*yd(5:8,:))');xtitle(' ','time','control');
      xsetech([0.5,0,0.5,0.5]);
      plot2d([t1;t1]',yd([3,7],:)',[1,2],"121",'theta@theta estimated');
      xsetech([0,0.5,1.0,0.5]);
      isoview(minx,maxx,-l,l);
      dpnd1(x(k),teta(k))
      xset('wshow')
    end
    xset('pixmap',0)
  end
end
xsetech(xxr); xset("font",xxf(1),xxf(2));
