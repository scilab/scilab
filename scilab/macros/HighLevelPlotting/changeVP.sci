function [vp,ech,cbech,xlech,ylech,titech]=changeVP(state,cb)    
xlech=[];
ylech=[];
titech=[];
    
fact=state('margin');
vp=state('viewport');

x=vp(1);y=vp(2);
w=vp(3);h=vp(4);
dy=h*fact;
dx=w*fact;
dxl=dx;
dyl=dy*1.2;

X=x+dxl;
Y=y+dy;
LX=w-dx-dxl;
LY=h-dy-dyl;
cbech=[];

if cb=="left"
	X=x+2*dxl+w/16;
	LX=w-2*dxl-dx-w/16;
	LY=h-dy-dyl;
	cbech=[x+dxl y+dy w/16 LY];	
elseif cb=="right"
	X=x+dxl
	Y=y+dy;
	LX=w-2*dxl-dx-w/16;
	LY=h-dy-dyl;
	cbech=[x+LX+2*dxl y+dy w/16 LY];	
elseif cb=="top"
	X=x+dxl;
	Y=y+2*dy+h/16;
	LY=h-2*dy-h/16-dyl;
	cbech=[X y+dy LX h/16];
elseif cb=="bot"
	X=x+dxl;
	Y=y+dy;
	LY=h-2*dy-h/16-dyl;
	cbech=[X y+dy+dyl+LY LX h/16];
end

ech=[X Y LX LY];
xlech=[X Y+LY+dyl/3 LX 2/3*dyl];
titech=[X Y-dy LX dy];
