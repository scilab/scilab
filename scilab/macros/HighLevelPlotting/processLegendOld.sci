function processLegendOld(mat,typ)

numberOfLegends=size(mat,1);

savedThickness=xget('thickness');
state=loadGraphicState(gcw());
listOfPlots=state('listOfPlots');
fact=state('margin');
vp=state('viewport');
x=vp(1);y=vp(2);w=vp(3);h=vp(4);
x=x+fact*w;
y=y+fact*h;
w=w-w*2*fact;
h=h-h*2.2*fact;

rect=xstringl(1,1,mat);

wb=rect(3);
hb=rect(4);

[wrect,frect,logflag,arect]=xgetech();

wf=wb/(frect(3)-frect(1));
hf=hb/(frect(4)-frect(2));

xsetech(wrect=[x+w*(1-wf) y w*wf h*hf],frect=[0 0 1 1],arect=[0 0 0 0]);
xrect(0,1,1,1);
xstring(0,0,mat)
