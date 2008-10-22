
// Copyright INRIA
clc;
clf();
curFig = gcf();
drawlater();
mode(1);
ncolor=228;

// First plot

subplot(2,2,1);

h=hotcolormap(ncolor);
curFig.color_map = h;
grayplot();


// Second plot

subplot(2,2,2);
h1=((1:ncolor)'/ncolor)*[1 0 0];
curFig.color_map = h1;
plot3d1();

// Third plot

subplot(2,2,3);
h2=graycolormap(ncolor);
curFig.color_map = h2;
grayplot();

// Fourth plot

subplot(2,2,4);
deff('[x,y,z]=knot(u,v)',['vv=ones(v)';'uu=ones(u);';
    'x=(5.*cos(u)+cos(u).*cos(v))';
    'y=(5.*sin(u)+sin(u).*cos(v))';
    'z=(uu.*sin(v))';])
nx=60;Nx=(0:nx)/nx ;ny=20;Ny=(0:ny)/ny;
[xx,yy,zz]=eval3dp(knot,2*%pi*Nx,2*%pi*Ny);
XXX=[-xx xx];YYY=[-yy zz];ZZZ=[-zz yy];
kk1=[1:size(zz,2)];

ncolor=60;
hh=hotcolormap(ncolor);
curFig.color_map = hh;
kk1=modulo(kk1,60);kk2=kk1;
KKK=list(ZZZ,[kk1 kk2]);
plot3d(XXX,YYY,KKK,35,70," @ @ ",[2,1,4],[-6,6,-6,6,-6,6]);
a=gca(); a.title.text='misc'; a.title.font_size=3;,
drawnow();

// Change the window colormap
xpause(2.d6);
curFig.color_map = h;
xbasr(xget('window'));
xpause(2d6);
curFig.color_map = h1;
xbasr(xget('window'));
xpause(2d6);
curFig.color_map = h2;
xbasr(xget('window'));
xpause(2d6);
curFig.color_map = hh;
