function processLegend(mat,typ)

numberOfLegends=size(mat,1);

if numberOfLegends==0
	return
end

blanc=' ';
mat=blanc(ones(numberOfLegends,1))+mat;

ind=find(mat==' ');
indcol=1:numberOfLegends;
indcol(ind)=[];
mat(ind)=[];

nl=length(indcol);

if nl==0
	return
end

win=gcw()
winH=gcf();

state=loadGraphicState(win);
listOfPlots=state('listOfPlots');
fact=state('margin');
vp=state('viewport');
ech=state('ech');
a=state.mainAxis;
frect=[a.data_bounds(1) a.data_bounds(3) a.data_bounds(2) a.data_bounds(4)];

fst=a.font_style;
fsz=a.font_size;
xset('font',fst,fsz);
rect=xstringl(0,0,mat);
x=ech(1);y=ech(2);w=ech(3);h=ech(4);
hl=rect(4)/(frect(4)-frect(2))*h*1.7;
wl=rect(3)/(frect(3)-frect(1))*w*1.1;

select typ
case -1
case 1
xl=x+w-wl-w/10;
rect=[xl-w/40 y+h/40 wl+w/10 hl];
case 2
rect=[x+w/40 y+h/40 wl+w/10 hl];
case 3
yl=y+h-hl;
rect=[x+w/40 yl-h/40 wl+w/10 hl];
case 4
xl=x+w-wl-w/10;
yl=y+h-hl;
rect=[xl-w/40 yl-h/40 wl+w/10 hl];
case 5
end

xsetech(wrect=rect,arect=[0 0 0 0]);
xfrect(0,1,1,1)
r=gce();
r.foreground=a.background;
xrect(0,1,1,1);
r=gce();
r.foreground=a.foreground;
for i=1:nl
	xstringb(0,1-i/nl,mat(i,:),wl/(wl+w/10),1/nl)
	t=get('hdl');
	t.font_size=fsz;
	t.font_style=fst;
      [col,markerId,markersize,lineStyle]=listOfPlots(indcol(i))(3:6);
      xset('line style',lineStyle);
      xset('thickness',markersize);
      x=wl/(wl+w/10);
	  X=linspace(wl/(wl+w/10)+0.05,0.90,3)';
	  step=1/nl;
      Y=(1-step/2-(i-1)*step)*[ 1 1 1]';
      xpolys(X,Y,col);
	  h=gce();h=h.children;
	  if markerId~=[]
	  	if lineStyle>0
			xpolys(X,Y,col);
			h=gce();h=h.children;
		end
		h.mark_mode='on';
		h.mark_style=markerId;
		h.mark_size=markerSize;
	  end
end

a.font_style=fst;
a.font_size=fsz;
